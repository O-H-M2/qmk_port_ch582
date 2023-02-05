import asyncio
import functools
import logging
import concurrent.futures
from . import APIError, Parameters


logger = logging.getLogger(__name__)


def _ctypes_callback_wrapper(callback):
    """
    Propagate exception to asyncio loop and avoid "Exception ignored on calling ctypes callback function" warning
    """

    @functools.wraps(callback)
    def wrapper(self, *args, **kwargs):
        try:
            callback(self, *args, **kwargs)
        except Exception as e:
            if self._loop:
                self._loop.call_soon_threadsafe(self._fatal_error, e, "Exception in C callback")
            else:
                # Fallback if RTTTransport is already closed
                self.logger.exception(e)

    return wrapper


class RTTTransport(asyncio.Transport):
    """
    RTT bidirectional transport.

    A transport class abstracts a communication channel.
    A transport is not instantiated directly, `LowLevel.rtt_asyncio_create_connection()` or
    `LowLevel.rtt_asyncio_open_connection()`utility functions should be called instead.

    .. seealso:: asyncio.Transport functions documentation
    """

    def __init__(self, protocol, loop, api, channel_index, encoding="utf-8"):
        self._api = api
        self._snr = self._api.read_connected_emu_snr()

        super().__init__(extra={"snr": self._snr, "channel_index": channel_index})

        self._protocol = protocol
        self._loop = loop
        self._channel_index = channel_index
        self._closing = False
        self.encoding = encoding

        self._unflushed_num_bytes = 0
        """
        For how many bytes write has not been confirmed yet. 
        
        When write(data) is called, the data length is added.
        When write confirmation callback is received from the DLL, the data length is subtracted.
        """

        self.logger = Parameters.LoggerAdapter(
            logger, {"id": self._api._logger.extra["id"], "channel_index": channel_index}
        )

    def __repr__(self):
        return f"<{self.__class__.__name__}({self._protocol}, {self._loop}, {self._channel_index})>"

    def is_closing(self):
        return self._closing

    def close(self):
        """
        Close the transport gracefully
        """
        self._close()

    def abort(self):
        """
        Close the transport gracefully.
        Due to the underlying RTT implementation, it's not possible to immediately close the RTT connection
        """
        self._close()

    def write(self, data):
        """
        Non-blocking write
        """
        if not self._closing:
            try:
                self._api.rtt_async_write(self._channel_index, data, encoding=self.encoding)
                self._unflushed_num_bytes += len(data)
            except APIError.APIError as e:
                self._fatal_error(e, "rtt_async_write error")

    def can_write_eof(self):
        # RTT does not support EOF
        return False

    def pause_reading(self):
        raise NotImplementedError("Pause rtt_async_read (flow control) is not implemented")

    def set_write_buffer_limits(self, high=None, low=None):
        raise NotImplementedError("RTT write watermarks (flow control) are not implemented")

    def get_write_buffer_size(self):
        return self._unflushed_num_bytes

    def _fatal_error(self, exc, message):
        """
        Propagate `exc` to the event loop and abort the transport
        """
        self._loop.call_exception_handler(
            {
                "message": message,
                "exception": exc,
                "protocol": self._protocol,
                "transport": self,
            }
        )
        self._close(exc)

    def _flushed(self):
        return self._unflushed_num_bytes == 0

    def _close(self, exc=None):
        """
        Close the transport gracefully, nrfjprogdll makes sure the close is performed gracefully.
        """
        self._closing = True
        try:
            if self._protocol is not None:
                self._protocol.connection_lost(exc)
        finally:

            def blocking_close():
                try:
                    self._api.rtt_async_callback_stop(self._channel_index)
                except APIError.APIError:
                    self.logger.exception("rtt_async_callback_stop returned an api error")
                except Exception:
                    self.logger.exception("rtt_async_callback_stop encountered an unknown error")
                self._api = None

            self._protocol = None
            self._loop = None
            self._unflushed_num_bytes = 0

            if self._api is not None:
                executor = concurrent.futures.ThreadPoolExecutor(
                    max_workers=1, thread_name_prefix=self.__class__.__name__
                )
                _ = executor.submit(blocking_close)
                executor.shutdown(wait=False)

    @_ctypes_callback_wrapper
    def rtt_asyncio_read_callback(self, _channel_index, data, _callback_param):
        if data and not self._closing:
            self._loop.call_soon_threadsafe(self._protocol.data_received, data)

    @_ctypes_callback_wrapper
    def rtt_asyncio_write_callback(self, _channel_index, num_bytes_flushed, result, _callback_param):
        if result != APIError.NrfjprogdllErr.SUCCESS:
            # This will be handled by _ctypes_callback_wrapper
            raise APIError.APIError(result, "rtt write failed", error_data=[self._api, self._snr, self._channel_index])

        self._unflushed_num_bytes -= num_bytes_flushed
        assert self._unflushed_num_bytes >= 0
