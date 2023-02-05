import warnings

try:
    from . import LowLevel
    from .Parameters import *
    from . import APIError
    from . import JLink
except Exception:
    import LowLevel
    from Parameters import *
    import APIError
    import JLink


warnings.warn("MultiAPI module will be deprecated in version 11, use LowLevel module instead.", PendingDeprecationWarning)


class MultiAPI(LowLevel.API):
    """
    API class that allows multiple instantiations
    """

    def __init__(self, device_family, jlink_arm_dll_path=None, log=False, log_str=None, log_file_path=None, api_lock_factory=None):
        self._terminated = False
        super(MultiAPI, self).__init__(device_family, jlink_arm_dll_path=jlink_arm_dll_path, log=log, log_str=log_str, log_file_path=log_file_path)

    def is_alive(self):
        """ Deprecated function, use is_open() instead. """
        return not self._terminated

    def terminate(self):
        """ Deprecated function, use close() instead. """
        self._terminated = True
