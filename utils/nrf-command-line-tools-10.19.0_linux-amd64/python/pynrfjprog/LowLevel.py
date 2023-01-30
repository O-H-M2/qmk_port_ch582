"""
This module serves as a Python wrapper around the nrfjprog DLL.

Note: Please look at the nrfjprogdll.h file provided with the tools for a more elaborate description of the API functions and their side effects.
"""

from __future__ import print_function

import weakref
from builtins import int

import ctypes
import os
import sys
import datetime
import logging
from pathlib import Path
from threading import Lock as _Lock
import asyncio
import json

import tomli_w

try:
    from .Parameters import *
    from .APIError import *
    from . import RTTAsyncIO
except Exception:
    from Parameters import *
    from APIError import *
    import RTTAsyncIO


def logger_cb(msg, logger):
    logging.getLogger(decode_string(logger).strip()).debug(decode_string(msg).strip())


"""
Deprecated: Do not use, use log parameter in API constructor instead.
"""
DEBUG_OUTPUT = False
QSPIIniFile = Path(__file__).parent / "QspiDefault.ini"


class API(object):
    """
    Main class of the module. Instance the class to get access to nrfjprog.dll functions in Python.

    Note: A copy of nrfjprog.dll must be found in the working directory.
    """

    _DEFAULT_JLINK_SPEED_KHZ = 2000

    def __init__(
        self,
        device_family=DeviceFamily.AUTO,
        jlink_arm_dll_path=None,
        log_str_cb=None,
        log=False,
        log_str=None,
        log_file_path=None,
        log_stringio=None,
    ):
        """
        Constructor.

        @param enum, str or int device_family: The series of device pynrfjprog will interact with.
        @param (optional) str jlink_arm_dll_path: Absolute path to the JLinkARM DLL that you want nrfjprog to use. Must be provided if your environment is not standard or your SEGGER installation path is not the default path. See JLink.py for details. Does not support unicode paths.
        @param (optional) callable object log_str_cb: If present, the log_str_cb will be called to receive log and error information. The log_str_cb object should be callable, expect to receive a string as the only parameter and does not need to return anything.
        @param (optional) bool log: If present and true, will enable logging to sys.stderr with the default log string appended to the beginning of each debug output line.
        @param (optional) str log_str: If present, will enable logging to sys.stderr with overwriten default log string appended to the beginning of each debug output line.
        @param (optional) str log_file_path: If present, will enable logging to log_file specified. This file will be opened in write mode in API.__init__() and api.open(), and closed when api.close() is called.
        @param (optional) str log_stringio: If present, will enable logging to open file-like object specified.
        """
        self._device_family = None
        self._jlink_arm_dll_path = None
        self._handle = ctypes.c_void_p(None)

        self._rtt_callbacks = dict()  # Used to store rtt callbacks so they do not go out of scope.

        self._logger = None
        self._log = log
        self._log_str_cb = log_str_cb
        self._log_str = log_str
        self._log_file_path = log_file_path
        self._log_stringio = log_stringio

        # Make a default "dead" finalizer. We'll initialize this in self.open.
        self._finalizer = weakref.finalize(self, lambda: None)

        self._device_family = decode_enum(device_family, DeviceFamily)
        if self._device_family is None:
            raise ValueError("Parameter device_family must be of type int, str or DeviceFamily enumeration.")

        if not isinstance(jlink_arm_dll_path, str) and not jlink_arm_dll_path is None:
            raise ValueError("Parameter jlink_arm_dll_path must be a string.")

        self._jlink_arm_dll_path = (
            os.path.abspath(jlink_arm_dll_path).encode("ascii") if jlink_arm_dll_path is not None else None
        )

        # Redirect writeable log endpoints to log_stringio
        if hasattr(log_file_path, "write") and log_stringio is None:
            self._log_stringio = log_file_path
            self._log_file_path = None

        os_name = sys.platform.lower()

        if os_name.startswith("win"):
            nrfjprog_dll_name = "nrfjprog.dll"
        elif os_name.startswith("linux"):
            nrfjprog_dll_name = "libnrfjprogdll.so"
        elif os_name.startswith("dar"):
            nrfjprog_dll_name = "libnrfjprogdll.dylib"
        else:
            raise ValueError("Unsupported OS")

        nrfjprog_dll_path = os.path.join(find_lib_dir(), nrfjprog_dll_name)

        if os.path.exists(nrfjprog_dll_path):
            try:
                self._lib = ctypes.cdll.LoadLibrary(nrfjprog_dll_path)
            except Exception as error:
                raise RuntimeError("Could not load the NRFJPROG DLL: '{}'.".format(error))
        else:
            try:
                self._lib = ctypes.cdll.LoadLibrary(nrfjprog_dll_name)
            except Exception as error:
                raise RuntimeError("Failed to load the NRFJPROG DLL by name: '{}.'".format(error))

    """
    nrfjprog.DLL functions.

    """

    def version(self):
        """
        Returns the nrfjprogdll version.
        Note the pynrfjprog python package exports the __version__ variable that will tell you the version of the python API.

        @return (int,int,int): Tuple containing major, minor, and micro revision of the nrfjprog dll.
        """
        major = ctypes.c_uint32()
        minor = ctypes.c_uint32()
        micro = ctypes.c_uint32()

        result = self._lib.NRFJPROG_version(
            ctypes.byref(major),
            ctypes.byref(minor),
            ctypes.byref(micro),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result)
        return major.value, minor.value, micro.value

    def dll_version(self):
        """
        Returns the JLinkARM.dll version.

        @return (int, int, str): Tuple containing the major, minor and revision of the dll.
        """
        major = ctypes.c_uint32()
        minor = ctypes.c_uint32()
        revision = ctypes.c_uint8()

        result = self._lib.NRFJPROG_dll_version_inst(
            self._handle,
            ctypes.byref(major),
            ctypes.byref(minor),
            ctypes.byref(revision),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())
        return major.value, minor.value, chr(revision.value)

    def find_jlink_path(self):
        """
        Searches for newest installation of JLink shared library (DLL/SO/dylib). The JLink path returned by this function
        will be the same found by the internal auto-detection used when no default JLink install location is provided.
        (See parameter jlink_arm_dll_path in function DebugProbe.__init__ for an example.)

        On unix-like systems the function may also return a library name compatible with dlopen if no library file is
        found in the default search path.

        @return (str): Path to JLink shared library.
        """
        buffer_len = ctypes.c_uint32(0)
        result = self._lib.NRFJPROG_find_jlink_path(None, ctypes.c_uint32(0), ctypes.byref(buffer_len))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors(), log=self._logger.error)

        buffer = (ctypes.c_char * buffer_len.value)(0)
        result = self._lib.NRFJPROG_find_jlink_path(buffer, buffer_len, ctypes.byref(buffer_len))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors(), log=self._logger.error)
        return buffer.value.decode("utf-8")

    def is_open(self):
        """
        Checks if the JLinkARM.dll is open.

        @return bool: True if open.
        """
        opened = ctypes.c_bool()

        result = self._lib.NRFJPROG_is_dll_open_inst(self._handle, ctypes.byref(opened))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())
        return opened.value

    def open(self):
        """
        Opens the JLinkARM.dll and sets the log callback. Prepares the dll for work with an nRF device.

        """
        # No need to encode self._jlink_arm_dll_path since it is an ASCII string and that is what is expected by ctypes.
        # Function self._log_str_cb has already been encoded in __init__() function.
        device_family = ctypes.c_int(self._device_family.value)

        _logger = logging.getLogger(__name__)
        self._logger = Parameters.LoggerAdapter(
            _logger,
            None,
            log=self._log,
            log_str_cb=self._log_str_cb,
            log_str=self._log_str,
            log_file_path=self._log_file_path,
            log_stringio=self._log_stringio,
        )

        result = self._lib.NRFJPROG_open_dll_inst(
            ctypes.byref(self._handle),
            self._jlink_arm_dll_path,
            self._logger.log_cb,
            None,
            device_family,
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        # Make sure that api is closed before api is destroyed
        self._finalizer = weakref.finalize(self, self.close)

    def close(self):
        """
        Closes and frees the JLinkARM DLL.

        """
        self._lib.NRFJPROG_close_dll_inst(ctypes.byref(self._handle))

        if self._logger is not None:
            handlers = self._logger.logger.handlers[:]
            for handler in handlers:
                handler.close()
                self._logger.logger.removeHandler(handler)

        # Disable the api finalizer, as it's no longer necessary when the api is closed.
        self._finalizer.detach()

    def configure(self, configuration, config_type=FileInputType.FILE_INPUT_PATH):
        """
        Configures nrfjprogdll using the provided configuration file or dict.

        @param dict|path|str configuration: Configuration to load into nrfjprogdll, either a dict, path to .toml file, or toml-formatted configuration string.
        @param FileInputType config_type: Payload type, defaults to path. If configuration is dict, a toml string will be generated, and config_type will be FILE_INPUT_BUFFER
        """

        if isinstance(configuration, dict):
            configuration = tomli_w.dumps(configuration)
            config_type = FileInputType.FILE_INPUT_BUFFER

        configuration = str(configuration).encode("utf-8")
        config_type = ctypes.c_int(config_type)

        result = self._lib.NRFJPROG_configure_inst(self._handle, configuration, config_type)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def get_errors(self):
        """
        Gets last logged error messages from the nrfjprog dll.
        Used to fill in APIError messages.

        @Return list of error strings.
        """
        return self._logger.get_errors() if self._logger is not None else ""

    def enum_emu_com_ports(self, serial_number):
        """
        Finds all comports currently associated with the serial number.

        @param int serial_number: Serial number of the debug probe to find the com port of.

        @Return list of ComPortInfo
        """

        if not is_u32(serial_number):
            raise ValueError("The serial_number parameter must be an unsigned 32-bit value.")

        serial_number = ctypes.c_uint32(serial_number)
        com_ports_len = ctypes.c_uint32(NRFJPROG_COM_PER_JLINK)
        num_com_ports = ctypes.c_uint32()
        com_ports = (ComPortInfoStruct * NRFJPROG_COM_PER_JLINK)()

        result = self._lib.NRFJPROG_enum_emu_com_inst(
            self._handle,
            serial_number,
            ctypes.byref(com_ports),
            com_ports_len,
            ctypes.byref(num_com_ports),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return [ComPortInfo(comport) for comport in com_ports[0 : num_com_ports.value]]

    def enum_emu_snr(self):
        """
        Enumerates the serial numbers of connected USB J-Link emulators.

        @return [int]: A list with the serial numbers.
        """
        serial_numbers_len = ctypes.c_uint32(127)
        serial_numbers = (ctypes.c_uint32 * serial_numbers_len.value)()
        num_available = ctypes.c_uint32()

        result = self._lib.NRFJPROG_enum_emu_snr_inst(
            self._handle,
            ctypes.byref(serial_numbers),
            serial_numbers_len,
            ctypes.byref(num_available),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        snr = [int(serial_numbers[i]) for i in range(0, min(num_available.value, serial_numbers_len.value))]

        if len(snr) == 0:
            return None
        else:
            return snr

    def enum_emu_con_info(self, list_usb_emus=True, list_ip_emus=True):
        """
        Enumerates connection information of discoverable J-Link emulators.

        @param boolean list_usb_emus: Get list of USB connected emulators
        @param boolean list_ip_emus: Get list of network connected emulators

        @return [Parameters.EMUConInfo]: A list with emulator connection info.
        """
        if not is_bool(list_usb_emus):
            raise ValueError("The list_usb_emus parameter must be boolean.")
        if not is_bool(list_ip_emus):
            raise ValueError("The list_ip_emus parameter must be boolean.")

        num_available = ctypes.c_uint32(0)
        list_usb_emus = ctypes.c_bool(list_usb_emus)
        list_ip_emus = ctypes.c_bool(list_ip_emus)

        read_size = 127
        attempt = 2
        while attempt > 0:
            emu_infos_len = ctypes.c_uint32(read_size)
            emu_infos = (EMUConInfoStruct * emu_infos_len.value)()
            result = self._lib.NRFJPROG_enum_emu_con_info_inst(
                self._handle,
                ctypes.byref(emu_infos),
                emu_infos_len,
                ctypes.byref(num_available),
                list_usb_emus,
                list_ip_emus,
            )
            if result != NrfjprogdllErr.SUCCESS:
                raise APIError(result, error_data=self.get_errors())

            # Read again if the buffer was too small
            attempt -= 1
            if emu_infos_len.value >= num_available.value:
                break
            read_size = num_available.value

        result = [EMUConInfo(emu_info) for emu_info in emu_infos[0 : min(num_available.value, emu_infos_len.value)]]
        if len(result) == 0:
            return None
        else:
            return result

    def is_connected_to_emu(self):
        """
        Checks if the emulator has an established connection with Segger emulator/debugger.

        @return boolean: True if connected.
        """
        is_connected_to_emu = ctypes.c_bool()

        result = self._lib.NRFJPROG_is_connected_to_emu_inst(self._handle, ctypes.byref(is_connected_to_emu))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return is_connected_to_emu.value

    def connect_to_emu_with_snr(self, serial_number, jlink_speed_khz=_DEFAULT_JLINK_SPEED_KHZ):
        """
        Connects to a given emulator/debugger.

        @param int serial_number: Serial number of the emulator to connect to.
        @param int jlink_speed_khz: SWDCLK speed [kHz].
        """
        if not is_u32(serial_number):
            raise ValueError("The serial_number parameter must be an unsigned 32-bit value.")

        if not is_u32(jlink_speed_khz):
            raise ValueError("The jlink_speed_khz parameter must be an unsigned 32-bit value.")

        self._set_logger_id(serial_number)

        serial_number = ctypes.c_uint32(serial_number)
        jlink_speed_khz = ctypes.c_uint32(jlink_speed_khz)

        result = self._lib.NRFJPROG_connect_to_emu_with_snr_inst(self._handle, serial_number, jlink_speed_khz)
        if result != NrfjprogdllErr.SUCCESS:
            self._set_logger_id(None)
            raise APIError(result, error_data=self.get_errors())

    def connect_to_emu_with_ip(
        self,
        hostname="",
        port=0,
        serial_number=0,
        jlink_speed_khz=_DEFAULT_JLINK_SPEED_KHZ,
    ):
        """
        Connects to a given emulator/debugger.

        @param string hostname: ip address/hostname/tunnel-string of the emulator to connect to.
        @param int port: port to use for connection. Only applies if hostname is set.
        @param int serial_number: serial number of debugger to connect to. Ignored if hostname is set.
        @param int jlink_speed_khz: SWDCLK speed [kHz].
        """
        if not is_u32(jlink_speed_khz):
            raise ValueError("The jlink_speed_khz parameter must be an unsigned 32-bit value.")
        if not is_u32(serial_number):
            raise ValueError("The serial_number parameter must be an unsigned 32-bit value.")
        if not is_u16(port):
            raise ValueError("The serial_number parameter must be an unsigned 16-bit value.")

        hostname = ctypes.c_char_p(hostname.encode("utf-8"))
        port = ctypes.c_uint16(port)
        serial_number = ctypes.c_uint32(serial_number)
        jlink_speed_khz = ctypes.c_uint32(jlink_speed_khz)

        result = self._lib.NRFJPROG_connect_to_emu_with_ip_inst(
            self._handle, hostname, port, serial_number, jlink_speed_khz
        )
        if result != NrfjprogdllErr.SUCCESS:
            self._set_logger_id(None)
            raise APIError(result, error_data=self.get_errors())

    def connect_to_emu_without_snr(self, jlink_speed_khz=_DEFAULT_JLINK_SPEED_KHZ):
        """
        Connects to an emulator/debugger.

        @param int jlink_speed_khz: SWDCLK speed [kHz].
        """
        if not is_u32(jlink_speed_khz):
            raise ValueError("The jlink_speed_khz parameter must be an unsigned 32-bit value.")

        jlink_speed_khz = ctypes.c_uint32(jlink_speed_khz)

        result = self._lib.NRFJPROG_connect_to_emu_without_snr_inst(self._handle, jlink_speed_khz)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        self._set_logger_id(self.read_connected_emu_snr())

    def reset_connected_emu(self):
        """
        Resets the connected emulator.
        """
        result = self._lib.NRFJPROG_reset_connected_emu_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def replace_connected_emu_fw(self):
        """
        Replaces the firmware of the connected emulator.
        """
        result = self._lib.NRFJPROG_replace_connected_emu_fw_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def read_connected_emu_snr(self):
        """
        Reads the serial number of the emu connected to.

        @return int: emu serial number.
        """
        snr = ctypes.c_uint32()

        result = self._lib.NRFJPROG_read_connected_emu_snr_inst(self._handle, ctypes.byref(snr))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return snr.value

    def read_connected_emu_fwstr(self):
        """
        Reads the firmware identification string of the connected emulator.

        @return str: firmware identification string.
        """
        buffer_size = ctypes.c_uint32(255)
        fwstr = ctypes.create_string_buffer(buffer_size.value)

        result = self._lib.NRFJPROG_read_connected_emu_fwstr_inst(self._handle, fwstr, buffer_size)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return fwstr.value if sys.version_info[0] == 2 else fwstr.value.decode("utf-8")

    def disconnect_from_emu(self):
        """
        Disconnects from an emulator.

        """
        result = self._lib.NRFJPROG_disconnect_from_emu_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def select_family(self, family):
        """
        Select device family
        @param DeviceFamily family: Target family for further api calls.
        """
        if not is_enum(family, DeviceFamily):
            raise ValueError("family Parameter must be of type int, str or DeviceFamily enumeration.")

        family = decode_enum(family, DeviceFamily)
        family = ctypes.c_int(family)

        result = self._lib.NRFJPROG_select_family_inst(self._handle, family)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def is_coprocessor_enabled(self, coprocessor):
        """
        Checks if chosen coprocessor is enabled.
        @param CoProcessor coprocessor: Target coprocessor for connect_to_device() call.
        @return bool: True if held in reset.
        """
        if not is_enum(coprocessor, CoProcessor):
            raise ValueError("CoProcessor Parameter must be of type int, str or CoProcessor enumeration.")

        coprocessor = decode_enum(coprocessor, CoProcessor)
        coprocessor = ctypes.c_int(coprocessor)

        held = ctypes.c_bool()

        result = self._lib.NRFJPROG_is_coprocessor_enabled_inst(self._handle, coprocessor, ctypes.byref(held))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return held.value

    def enable_coprocessor(self, coprocessor):
        """
        Enables chosen coprocessor
        @param CoProcessor coprocessor: Target coprocessor for connect_to_device() call.
        """
        if not is_enum(coprocessor, CoProcessor):
            raise ValueError("CoProcessor Parameter must be of type int, str or CoProcessor enumeration.")

        coprocessor = decode_enum(coprocessor, CoProcessor)
        coprocessor = ctypes.c_int(coprocessor)

        result = self._lib.NRFJPROG_enable_coprocessor_inst(self._handle, coprocessor)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def disable_coprocessor(self, coprocessor):
        """
        Disables chosen coprocessor
        @param CoProcessor coprocessor: Target coprocessor for connect_to_device() call.
        """
        if not is_enum(coprocessor, CoProcessor):
            raise ValueError("CoProcessor Parameter must be of type int, str or CoProcessor enumeration.")

        coprocessor = decode_enum(coprocessor, CoProcessor)
        coprocessor = ctypes.c_int(coprocessor)

        result = self._lib.NRFJPROG_disable_coprocessor_inst(self._handle, coprocessor)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def select_coprocessor(self, coprocessor):
        """
        Select coprocessor
        @param CoProcessor coprocessor: Target coprocessor for connect_to_device() call.
        """
        if not is_enum(coprocessor, CoProcessor):
            raise ValueError("CoProcessor Parameter must be of type int, str or CoProcessor enumeration.")

        coprocessor = decode_enum(coprocessor, CoProcessor)
        coprocessor = ctypes.c_int(coprocessor)

        result = self._lib.NRFJPROG_select_coprocessor_inst(self._handle, coprocessor)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def recover(self):
        """
        Recovers the device.

        """
        result = self._lib.NRFJPROG_recover_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def is_connected_to_device(self):
        """
        Checks if the connected emulator has an established connection with an nRF device.

        @return boolean: True if connected.
        """
        is_connected_to_device = ctypes.c_bool()

        result = self._lib.NRFJPROG_is_connected_to_device_inst(self._handle, ctypes.byref(is_connected_to_device))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return is_connected_to_device.value

    def connect_to_device(self):
        """
        Connects to the nRF device.

        """
        result = self._lib.NRFJPROG_connect_to_device_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def disconnect_from_device(self):
        """
        Disconnects from the device.

        """
        result = self._lib.NRFJPROG_disconnect_from_device_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def readback_protect(self, desired_protection_level):
        """
        Protects the device against read or debug.

        @param int, str, or ReadbackProtection(IntEnum) desired_protection_level: Readback protection level for the target.
        """
        if not is_enum(desired_protection_level, ReadbackProtection):
            raise ValueError(
                "Parameter desired_protection_level must be of type int, str or ReadbackProtection enumeration."
            )

        desired_protection_level = decode_enum(desired_protection_level, ReadbackProtection)
        if desired_protection_level is None:
            raise ValueError(
                "Parameter desired_protection_level must be of type int, str or ReadbackProtection enumeration."
            )

        desired_protection_level = ctypes.c_int(desired_protection_level.value)

        result = self._lib.NRFJPROG_readback_protect_inst(self._handle, desired_protection_level)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def readback_status(self):
        """
        Returns the status of the readback protection.

        @return str: Readback protection level of the target.
        """
        status = ctypes.c_int()

        result = self._lib.NRFJPROG_readback_status_inst(self._handle, ctypes.byref(status))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return ReadbackProtection(status.value).name

    def enable_eraseprotect(self):
        """
        Protects the device against erasing.
        """
        result = self._lib.NRFJPROG_enable_eraseprotect_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def is_eraseprotect_enabled(self):
        """
        Returns the status of the erase protection.

        @return bool: True if erase protection is enabled.
        """
        status = ctypes.c_bool()

        result = self._lib.NRFJPROG_is_eraseprotect_enabled_inst(self._handle, ctypes.byref(status))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return status.value

    def read_region_0_size_and_source(self):
        """
        Returns the region 0 size and source of protection if any for nRF51 devices.

        @return (int, str): Region size and configuration source of protection (either UICR of FICR).
        """
        size = ctypes.c_uint32()
        source = ctypes.c_int()

        result = self._lib.NRFJPROG_read_region_0_size_and_source_inst(
            self._handle, ctypes.byref(size), ctypes.byref(source)
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return size.value, Region0Source(source.value).name

    def debug_reset(self):
        """
        Executes a soft reset using the CTRL-AP for nRF52 and onward devices.

        """
        result = self._lib.NRFJPROG_debug_reset_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def sys_reset(self):
        """
        Executes a system reset request.

        """
        result = self._lib.NRFJPROG_sys_reset_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def pin_reset(self):
        """
        Executes a pin reset. If your device has a configurable pin reset, in order for the function execution to have the desired effect the pin reset must be enabled in UICR.PSELRESET[] registers.

        """
        result = self._lib.NRFJPROG_pin_reset_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def hard_reset(self):
        """
        Executes a hard reset.

        """
        result = self._lib.NRFJPROG_hard_reset_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def is_bprot_enabled(self, address_start, length):
        """
        Detects if memory block protection is enabled.
        Any memory covered by the range [address_start : address_start + length] that is not non-volatile code memory is assumed not block protected.

        @param int address_start: Query address range start.
        @param int length: Query address range length.

        @return boolean: True if bprot is enabled for address range.
        """
        if not is_u32(address_start):
            raise ValueError("The address_start parameter must be an unsigned 32-bit value.")

        if not is_u32(length):
            raise ValueError("The length parameter must be an unsigned 32-bit value.")

        address_start = ctypes.c_uint32(address_start)
        length = ctypes.c_uint32(length)
        bprot_enabled = ctypes.c_bool(False)

        result = self._lib.NRFJPROG_is_bprot_enabled_inst(
            self._handle, ctypes.byref(bprot_enabled), address_start, length
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return bprot_enabled.value

    def disable_bprot(self):
        """
        Disables BPROT, ACL or NVM protection blocks where appropriate depending on device.

        """
        result = self._lib.NRFJPROG_disable_bprot_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def erase_all(self):
        """
        Erases all code and UICR flash.

        """
        result = self._lib.NRFJPROG_erase_all_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def erase_page(self, addr):
        """
        Erases a page of code flash which contains address addr.

        @param int addr: Address in the page of code flash to erase.
        """
        if not is_u32(addr):
            raise ValueError("The addr parameter must be an unsigned 32-bit value.")

        addr = ctypes.c_uint32(addr)

        result = self._lib.NRFJPROG_erase_page_inst(self._handle, addr)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def erase_uicr(self):
        """
        Erases UICR info page.

        """
        result = self._lib.NRFJPROG_erase_uicr_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def write_u32(self, addr, data, control):
        """
        Writes one uint32_t data into the given address.

        @param int addr: Address to write.
        @param int data: Value to write.
        @param boolean control: True for automatic control of NVMC by the function.
        """
        if not is_u32(addr):
            raise ValueError("The addr parameter must be an unsigned 32-bit value.")

        if not is_u32(data):
            raise ValueError("The data parameter must be an unsigned 32-bit value.")

        if not is_bool(control):
            raise ValueError("The control parameter must be a boolean value.")

        addr = ctypes.c_uint32(addr)
        data = ctypes.c_uint32(data)
        control = ctypes.c_bool(control)

        result = self._lib.NRFJPROG_write_u32_inst(self._handle, addr, data, control)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def read_u32(self, addr):
        """
        Reads one uint32_t from the given address.

        @param  int addr: Address to read.
        @return int: Value read.
        """
        if not is_u32(addr):
            raise ValueError("The addr parameter must be an unsigned 32-bit value.")

        addr = ctypes.c_uint32(addr)
        data = ctypes.c_uint32()

        result = self._lib.NRFJPROG_read_u32_inst(self._handle, addr, ctypes.byref(data))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return data.value

    def write(self, addr, data, control):
        """
        Writes data from the array into the device starting at the given address.

        @param int addr: Start address of the memory block to write.
        @param sequence data: Data to write. Any type that implements the sequence API (i.e. string, list, bytearray...) is valid as input.
        @param boolean control: True for automatic control of NVMC by the function.
        """
        if not is_u32(addr):
            raise ValueError("The addr parameter must be an unsigned 32-bit value.")

        if not is_valid_buf(data):
            raise ValueError("The data parameter must be a sequence type with at least one item.")

        if not is_bool(control):
            raise ValueError("The control parameter must be a boolean value.")

        addr = ctypes.c_uint32(addr)
        data_len = ctypes.c_uint32(len(data))
        data = (ctypes.c_uint8 * data_len.value)(*data)
        control = ctypes.c_bool(control)

        result = self._lib.NRFJPROG_write_inst(self._handle, addr, ctypes.byref(data), data_len, control)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def read(self, addr, data_len):
        """
        Reads data_len bytes from the device starting at the given address.

        @param int addr: Start address of the memory block to read.
        @param int data_len: Number of bytes to read.
        @return [int]: List of values read.
        """
        if not is_u32(addr):
            raise ValueError("The addr parameter must be an unsigned 32-bit value.")

        if not is_u32(data_len):
            raise ValueError("The data_len parameter must be an unsigned 32-bit value.")

        addr = ctypes.c_uint32(addr)
        data_len = ctypes.c_uint32(data_len)
        data = (ctypes.c_uint8 * data_len.value)()

        result = self._lib.NRFJPROG_read_inst(self._handle, addr, ctypes.byref(data), data_len)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return list(data)

    def is_halted(self):
        """
        Checks if the device CPU is halted.

        @return boolean: True if halted.
        """
        is_halted = ctypes.c_bool()

        result = self._lib.NRFJPROG_is_halted_inst(self._handle, ctypes.byref(is_halted))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return is_halted.value

    def halt(self):
        """
        Halts the device CPU.

        """
        result = self._lib.NRFJPROG_halt_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def run(self, pc, sp):
        """
        Starts the device CPU with the given pc and sp.

        @param int pc: Value for the program counter.
        @param int sp: Value for the stack pointer.
        """
        if not is_u32(pc):
            raise ValueError("The pc parameter must be an unsigned 32-bit value.")

        if not is_u32(sp):
            raise ValueError("The sp parameter must be an unsigned 32-bit value.")

        pc = ctypes.c_uint32(pc)
        sp = ctypes.c_uint32(sp)

        result = self._lib.NRFJPROG_run_inst(self._handle, pc, sp)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def go(self):
        """
        Starts the device CPU.

        """
        result = self._lib.NRFJPROG_go_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def step(self):
        """
        Runs the device CPU for one instruction.

        """
        result = self._lib.NRFJPROG_step_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def read_ram_sections_count(self):
        """
        Reads the number of RAM sections in the device.

        @return int: number of RAM sections in the device
        """
        count = ctypes.c_uint32()

        result = self._lib.NRFJPROG_read_ram_sections_count_inst(self._handle, ctypes.byref(count))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return count.value

    def read_ram_sections_size(self):
        """
        Reads the size in bytes of the RAM sections in the device.

        @return [int]: List containing the size of each RAM power section. The length of the list is equal to the number of ram sections in the device.
        """
        sections_size_list_size = ctypes.c_uint32(self.read_ram_sections_count())
        sections_size = (ctypes.c_uint32 * sections_size_list_size.value)()

        result = self._lib.NRFJPROG_read_ram_sections_size_inst(
            self._handle, ctypes.byref(sections_size), sections_size_list_size
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return list(sections_size)

    def read_ram_sections_power_status(self):
        """
        Reads the RAM sections power status.

        @return [str]: List containing the power status of each RAM power section. The length of the list is equal to the number of ram sections in the device.
        """
        status_size = ctypes.c_uint32(self.read_ram_sections_count())
        status = (ctypes.c_uint32 * status_size.value)()

        result = self._lib.NRFJPROG_read_ram_sections_power_status_inst(self._handle, ctypes.byref(status), status_size)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return [RamPower(elem).name for elem in list(status)]

    def power_ram_all(self):
        """
        Powers up all RAM sections of the device.

        """
        result = self._lib.NRFJPROG_power_ram_all_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def unpower_ram_section(self, section_index):
        """
        Powers down a RAM section of the device.

        @param int section_index: RAM block index to power off.
        """
        if not is_u32(section_index):
            raise ValueError("The section_index parameter must be an unsigned 32-bit value.")

        section_index = ctypes.c_uint32(section_index)

        result = self._lib.NRFJPROG_unpower_ram_section_inst(self._handle, section_index)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def read_memory_descriptors(self, read_page_sizes=True):
        """
        Reads the list of memory descriptors available for the current coprocessor.

        @param bool read_page_sizes: If true, the page sizes of each memory is read from the family DLL and included in the result (MemoryDescription.page_repetitions)
        @return: list of Parameters.MemoryDescription: Memory descriptors read.
        """
        # Start by obtaining the number of memories available
        num_available = ctypes.c_uint32()
        result = self._lib.NRFJPROG_read_memory_descriptors_inst(
            self._handle, None, ctypes.c_uint32(0), ctypes.byref(num_available)
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result)

        # Skip read if nothing is available
        if num_available.value == 0:
            return list()

        # Perform the read
        memory_description_arr = (MemoryDescriptionStruct * num_available.value)()
        result = self._lib.NRFJPROG_read_memory_descriptors_inst(
            self._handle,
            ctypes.byref(memory_description_arr),
            num_available,
            ctypes.byref(num_available),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result)

        result = [
            MemoryDescription(mem_description) for mem_description in memory_description_arr[0 : num_available.value]
        ]
        if read_page_sizes:
            for memory_description in result:
                memory_description.page_repetitions = self.read_page_sizes(memory_description)
        return result

    def read_page_sizes(self, memory_description):
        """
        Reads the page sizes a memory referenced by a memory description.
        Note that memory references become invalid when changing target family, device, or coprocessor.

        @param  MemoryDescription memory_description: A memory description obtained using function 'read_memory_descriptors'

        @return: list of Parameters.PageRepetitions: Page repetitions read.
        """
        if not is_right_class(memory_description, MemoryDescription):
            raise ValueError("Parameter memory_description must be of type MemoryDescription.")

        # Start by obtaining the number of memories available
        num_available = ctypes.c_uint32()
        result = self._lib.NRFJPROG_read_page_sizes_inst(
            self._handle,
            ctypes.byref(memory_description._cstruct),
            None,
            ctypes.c_uint32(0),
            ctypes.byref(num_available),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result)

        # Skip read if nothing is available
        if num_available.value == 0:
            return list()

        # Perform the read
        page_repetitions_arr = (PageRepetitionsStruct * num_available.value)()
        result = self._lib.NRFJPROG_read_page_sizes_inst(
            self._handle,
            ctypes.byref(memory_description._cstruct),
            ctypes.byref(page_repetitions_arr),
            num_available,
            ctypes.byref(num_available),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result)

        return [PageRepetitions(page_rep) for page_rep in page_repetitions_arr[0 : num_available.value]]

    def read_cpu_register(self, register_name):
        """
        Reads a CPU register.

        @param  uint, str, or CPURegister(IntEnum) register_name: CPU register to read.
        @return int: Value read.
        """
        if not is_u32(register_name):
            register_enum = decode_enum(register_name, CpuRegister)
            if register_enum is None:
                raise ValueError("Parameter register_name must be of type uint, str, or CpuRegister enumeration.")
            register_name = register_enum.value

        register_name = ctypes.c_int(register_name)
        value = ctypes.c_uint32()

        result = self._lib.NRFJPROG_read_cpu_register_inst(self._handle, register_name, ctypes.byref(value))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return value.value

    def write_cpu_register(self, register_name, value):
        """
        Writes a CPU register.

        @param uint, str, or CPURegister(IntEnum) register_name: CPU register to write.
        @param int value: Value to write.
        """
        if not is_u32(value):
            raise ValueError("The value parameter must be an unsigned 32-bit value.")

        if not is_u32(register_name):
            register_enum = decode_enum(register_name, CpuRegister)
            if register_enum is None:
                raise ValueError("Parameter register_name must be of type uint, str, or CpuRegister enumeration.")
            register_name = register_enum.value

        register_name = ctypes.c_int(register_name)
        value = ctypes.c_uint32(value)

        result = self._lib.NRFJPROG_write_cpu_register_inst(self._handle, register_name, value)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def read_device_version(self):
        """
        Reads the version of the device connected to the emulator.

        @return str: Version of the target device.
        """
        version = ctypes.c_int()

        result = self._lib.NRFJPROG_read_device_version_inst(self._handle, ctypes.byref(version))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return DeviceVersion(version.value).name

    def read_device_info(self):

        version = ctypes.c_int()
        name = ctypes.c_int()
        memory = ctypes.c_int()
        revision = ctypes.c_int()
        result = self._lib.NRFJPROG_read_device_info_inst(
            self._handle,
            ctypes.byref(version),
            ctypes.byref(name),
            ctypes.byref(memory),
            ctypes.byref(revision),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return (
            DeviceVersion(version.value),
            DeviceName(name.value),
            DeviceMemory(memory.value),
            DeviceRevision(revision.value),
        )

    def read_cpu_architecture(self):
        """
        Gets the expected CPU architecture for the selected coprocessor.

        @return arch: Architecture of the selected coprocessor
        """

        arch = ctypes.c_int()
        result = self._lib.NRFJPROG_read_cpu_architecture_inst(self._handle, ctypes.byref(arch))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return Architecture(arch.value)

    def read_device_family(self):
        """
        Reads the family of the device connected to the emulator. Only if API class has been instantiated with AUTO family.

        @return str: Family of the target device.
        """
        family = ctypes.c_int()

        result = self._lib.NRFJPROG_read_device_family_inst(self._handle, ctypes.byref(family))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return DeviceFamily(family.value).name

    def read_debug_port_register(self, addr):
        """
        Reads a debug port register.

        @param int addr: Address to read.
        @return int: Value read.
        """
        if not is_u8(addr):
            raise ValueError("The addr parameter must be an unsigned 8-bit value.")

        addr = ctypes.c_uint8(addr)
        data = ctypes.c_uint32()

        result = self._lib.NRFJPROG_read_debug_port_register_inst(self._handle, addr, ctypes.byref(data))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return data.value

    def write_debug_port_register(self, addr, data):
        """
        Writes a debug port register.

        @param int addr: Address to write.
        @param int data: Value to write.
        """
        if not is_u8(addr):
            raise ValueError("The addr parameter must be an unsigned 8-bit value.")

        if not is_u32(data):
            raise ValueError("The data parameter must be an unsigned 32-bit value.")

        addr = ctypes.c_uint8(addr)
        data = ctypes.c_uint32(data)

        result = self._lib.NRFJPROG_write_debug_port_register_inst(self._handle, addr, data)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def read_access_port_register(self, ap_index, addr):
        """
        Reads a debugger access port register.

        @param int ap_index: Index of the access port to read.
        @param int addr: Address to read.
        @return int: Value read.
        """
        if not is_u8(ap_index):
            raise ValueError("The ap_index parameter must be an unsigned 8-bit value.")

        if not is_u8(addr):
            raise ValueError("The addr parameter must be an unsigned 8-bit value.")

        ap_index = ctypes.c_uint8(ap_index)
        addr = ctypes.c_uint8(addr)
        data = ctypes.c_uint32()

        result = self._lib.NRFJPROG_read_access_port_register_inst(self._handle, ap_index, addr, ctypes.byref(data))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return data.value

    def write_access_port_register(self, ap_index, addr, data):
        """
        Writes a debugger access port register.

        @param int ap_index: Index of the access port to write.
        @param int addr: Address to write.
        @param int data: Value to write.
        """
        if not is_u8(ap_index):
            raise ValueError("The ap_index parameter must be an unsigned 8-bit value.")

        if not is_u8(addr):
            raise ValueError("The addr parameter must be an unsigned 8-bit value.")

        if not is_u32(data):
            raise ValueError("The data parameter must be an unsigned 32-bit value.")

        ap_index = ctypes.c_uint8(ap_index)
        addr = ctypes.c_uint8(addr)
        data = ctypes.c_uint32(data)

        result = self._lib.NRFJPROG_write_access_port_register_inst(self._handle, ap_index, addr, data)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def is_rtt_started(self):
        """
        Checks if the RTT is started.

        @return bool: True if started.
        """
        started = ctypes.c_bool()

        result = self._lib.NRFJPROG_is_rtt_started_inst(self._handle, ctypes.byref(started))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return started.value

    def rtt_set_control_block_address(self, addr):
        """
        Indicates to the dll the location of the RTT control block in the device memory.

        @param int addr: Address of the RTT Control Block in memory.
        """
        if not is_u32(addr):
            raise ValueError("The address parameter must be an unsigned 32-bit value.")

        addr = ctypes.c_uint32(addr)

        result = self._lib.NRFJPROG_rtt_set_control_block_address_inst(self._handle, addr)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def rtt_start(self):
        """
        Starts RTT.

        """
        result = self._lib.NRFJPROG_rtt_start_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    async def rtt_asyncio_create_connection(
        self, protocol_factory, channel_index=0, max_read_data_len=64 * 1024, encoding="utf-8"
    ):
        """
        Create a streaming transport connection to a given RTT channel

        .. seealso:: create_connection() documentation in asyncio library

        :param channel_index: RTT channel index, used for both up and down channel
        :param max_read_data_len: Maximum size of data packet read buffer
        :param encoding: Encoding to be used when writing strings
        """
        loop = asyncio.get_event_loop()
        protocol = protocol_factory()
        transport = RTTAsyncIO.RTTTransport(protocol, loop, self, channel_index, encoding)

        # rtt_async_callback_start is blocking
        await loop.run_in_executor(
            None,
            self.rtt_async_callback_start,
            channel_index,
            max_read_data_len,
            transport.rtt_asyncio_read_callback,
            transport.rtt_asyncio_write_callback,
        )
        protocol.connection_made(transport)

        return transport, protocol

    async def rtt_asyncio_open_connection(self, channel_index=0, max_read_data_len=64 * 1024, encoding="utf-8"):
        """
        A wrapper for rtt_asyncio_create_connection() returning a (reader, writer) pair.

        .. seealso:: open_connection() documentation in asyncio library

        :param channel_index: RTT channel index, used for both up and down channel
        :param max_read_data_len: Maximum size of data packet read buffer
        :param encoding: Encoding to be used when writing strings
        """
        loop = asyncio.get_event_loop()
        reader = asyncio.StreamReader(loop=loop)
        protocol = asyncio.StreamReaderProtocol(reader, loop=loop)
        transport, _ = await self.rtt_asyncio_create_connection(
            lambda: protocol, channel_index, max_read_data_len, encoding
        )
        writer = asyncio.StreamWriter(transport, protocol, reader, loop)
        return reader, writer

    def rtt_async_callback_start(
        self,
        channel_index,
        max_read_data_len,
        read_callback_func,
        write_callback_func=None,
        callback_param=None,
    ):
        """
        Starts a separate thread feeding rtt data asynchronously to the callback function.
        Each channel gets a separate thread and can receive data asynchronously.
        Function can only be called once per channel.
        rtt_stop() may not immediately stop all threads. To stop rtt reception, call close().

        @param int channel_index: RTT channel to read and write to.
        @param int max_read_data_len: Maximum size of data packet read buffer.
        @param func read_callback: Callback function matching the rtt_read_callback type in CommonDefintions.
        @param func write_callback: Callback function matching the rtt_write_callback type in CommonDefintions.
        @param int callback_param: Value to be passed back to callback functions. Can be used to identify callbacks from different API instances or channels.


        read_callback signature:
        Called when data has been received on a channel configured with rtt_async_start.
        @param int up_channel_index: RTT channel that received data.
        @param bytes data: Recieved data.
        @param int callback_param: parameter callback_param from rtt_async_start.

        write_callback signature:
        Called when data has been written to a channel configured with rtt_async_start.
        @param int down_channel_index: RTT channel that data was sent to.
        @param int data_len: Number of bytes that were sent.
        @param NrfjprogdllErr result: Operation result.
        @param int callback_param: parameter callback_param from rtt_async_start.
        """

        if not is_u32(channel_index):
            raise ValueError("The channel_index parameter must be an unsigned 32-bit value.")
        if not is_u32(max_read_data_len):
            raise ValueError("The max_read_data_len parameter must be an unsigned 32-bit value.")
        if not is_pointer(callback_param):
            raise ValueError("The callback_param parameter must be an integer value or None.")

        read_callback = ctypes.c_void_p(None)
        write_callback = ctypes.c_void_p(None)

        if read_callback_func is not None:
            read_callback = lambda up_channel_index, data_len, data, param: read_callback_func(
                up_channel_index, data[:data_len], param
            )
            read_callback = ctypes.CFUNCTYPE(
                None, ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(ctypes.c_char), ctypes.c_void_p
            )(read_callback)

        if write_callback_func is not None:
            write_callback = lambda down_channel_index, data_len, result, param: write_callback_func(
                down_channel_index, data_len, NrfjprogdllErr(result), param
            )
            write_callback = ctypes.CFUNCTYPE(None, ctypes.c_uint32, ctypes.c_uint32, ctypes.c_int, ctypes.c_void_p)(
                write_callback
            )

        callback_param = ctypes.c_void_p(callback_param)

        self._rtt_callbacks[channel_index] = (read_callback, write_callback)

        channel_index = ctypes.c_uint32(channel_index)
        max_read_data_len = ctypes.c_uint32(max_read_data_len)

        result = self._lib.NRFJPROG_rtt_async_start_inst(
            self._handle,
            channel_index,
            max_read_data_len,
            read_callback,
            write_callback,
            callback_param,
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def rtt_async_flush(self):
        """
        Wait until all pending RT writes have been performed.
        """

        result = self._lib.NRFJPROG_rtt_async_flush_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def rtt_is_control_block_found(self):
        """
        Checks if RTT control block has been found.

        @return boolean: True if found.
        """
        is_control_block_found = ctypes.c_bool()

        result = self._lib.NRFJPROG_rtt_is_control_block_found_inst(self._handle, ctypes.byref(is_control_block_found))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return is_control_block_found.value

    def rtt_get_control_block_info(self):
        """
        Checks if RTT control block has been found, and at what address.

        @return (boolean, uint32): A tuple containing a boolean indicating if the control block was found, and the address of the control block.
        """
        is_found = ctypes.c_bool()
        address = ctypes.c_uint32()

        result = self._lib.NRFJPROG_rtt_get_control_block_info_inst(
            self._handle, ctypes.byref(is_found), ctypes.byref(address)
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return is_found.value, address.value

    def rtt_stop(self):
        """
        Stops RTT.

        """
        result = self._lib.NRFJPROG_rtt_stop_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        self._rtt_callbacks.clear()

    def rtt_async_callback_stop(self, channel_index):
        """
        Stops asynchronous RTT on specified channel.

        @param int channel_index: RTT channel to stop.
        """
        if not is_u32(channel_index):
            raise ValueError("The channel_index parameter must be an unsigned 32-bit value.")

        result = self._lib.NRFJPROG_rtt_async_stop_inst(self._handle, ctypes.c_uint32(channel_index))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        self._rtt_callbacks.pop(channel_index, None)

    def rtt_read(self, channel_index, length, encoding="utf-8"):
        """
        Reads from an RTT channel.

        @param int channel_index: RTT channel to read.
        @param int length: Number of bytes to read. Note that depending on the encoding parameter, the number of bytes read and the numbers of characters read might differ.
        @param (optional) str or None encoding: Encoding for the data read in order to build a readable string. Default value 'utf-8'. Note that since Python2 native string is coded in ASCII, only ASCII characters will be properly represented.
        @return str or bytearray: Data read. Return type depends on encoding optional parameter. If an encoding is given, the return type will be Python version's native string type. If None is given, a bytearray will be returned.
        """
        if not is_u32(channel_index):
            raise ValueError("The channel_index parameter must be an unsigned 32-bit value.")

        if not is_u32(length):
            raise ValueError("The length parameter must be an unsigned 32-bit value.")

        if encoding is not None and not is_valid_encoding(encoding):
            raise ValueError("The encoding parameter must be either None or a standard encoding in python.")

        channel_index = ctypes.c_uint32(channel_index)
        length = ctypes.c_uint32(length)
        data = (ctypes.c_uint8 * length.value)()
        data_read = ctypes.c_uint32()

        result = self._lib.NRFJPROG_rtt_read_inst(
            self._handle,
            channel_index,
            ctypes.byref(data),
            length,
            ctypes.byref(data_read),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        if encoding is None:
            return bytearray(data[0 : data_read.value])
        else:
            if sys.version_info[0] == 2:
                return bytearray(data[0 : data_read.value]).decode(encoding).encode("utf-8")
            else:
                return bytearray(data[0 : data_read.value]).decode(encoding)

    def rtt_write(self, channel_index, msg, encoding="utf-8"):
        """
        Writes to an RTT channel.

        @param int channel_index: RTT channel to write.
        @param sequence msg: Data to write. Any type that implements the sequence API (i.e. string, list, bytearray...) is valid as input.
        @param (optional) str or None encoding: Encoding of the msg to write. Default value 'utf-8'.
        @return int: Number of bytes written.  Note that if non-'latin-1' characters are used, the number of bytes written depends on the encoding parameter given.
        """
        if not is_u32(channel_index):
            raise ValueError("The channel_index parameter must be an unsigned 32-bit value.")

        if encoding is not None and not is_valid_encoding(encoding):
            raise ValueError("The encoding parameter must be either None or a standard encoding in python.")

        if isinstance(msg, str):
            msg = bytearray(msg, encoding)
        if not isinstance(msg, bytearray):
            msg = bytearray(msg)

        if not is_valid_buf(msg):
            raise ValueError("The msg parameter must be a sequence type with at least one item.")

        channel_index = ctypes.c_uint32(channel_index)
        length = ctypes.c_uint32(len(msg))
        data = (ctypes.c_uint8 * length.value)(*msg)
        data_written = ctypes.c_uint32()

        result = self._lib.NRFJPROG_rtt_write_inst(
            self._handle,
            channel_index,
            ctypes.byref(data),
            length,
            ctypes.byref(data_written),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return data_written.value

    def rtt_async_write(self, channel_index, msg, encoding="utf-8"):
        """
        Writes to an async RTT channel.

        If registered, a callback will be returned on write completion with the result, number of bytes written, and channel index as parameters.
        See rtt_async_start for more details. Note that if non-'latin-1' characters are used, the number of bytes written depends on the encoding parameter given.

        @param int channel_index: RTT channel to write.
        @param sequence msg: Data to write. Any type that implements the sequence API (i.e. string, list, bytearray...) is valid as input.
        @param (optional) str or None encoding: Encoding of the msg to write. Default value 'utf-8'.
        """
        if not is_u32(channel_index):
            raise ValueError("The channel_index parameter must be an unsigned 32-bit value.")

        if encoding is not None and not is_valid_encoding(encoding):
            raise ValueError("The encoding parameter must be either None or a standard encoding in python.")

        if isinstance(msg, str):
            msg = bytearray(msg, encoding)
        if not isinstance(msg, bytearray):
            msg = bytearray(msg)

        if not is_valid_buf(msg):
            raise ValueError("The msg parameter must be a sequence type with at least one item.")

        channel_index = ctypes.c_uint32(channel_index)
        length = ctypes.c_uint32(len(msg))
        data = (ctypes.c_uint8 * length.value)(*msg)

        result = self._lib.NRFJPROG_rtt_async_write_inst(self._handle, channel_index, ctypes.byref(data), length)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def rtt_read_channel_count(self):
        """
        Gets the number of RTT channels.

        @return (int, int): Tuple containing the number of down RTT channels and the number of up RTT channels.
        """
        down_channel_number = ctypes.c_uint32()
        up_channel_number = ctypes.c_uint32()

        result = self._lib.NRFJPROG_rtt_read_channel_count_inst(
            self._handle,
            ctypes.byref(down_channel_number),
            ctypes.byref(up_channel_number),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return down_channel_number.value, up_channel_number.value

    def rtt_read_channel_info(self, channel_index, direction):
        """
        Reads the info from one RTT channel.

        @param int channel_index: RTT channel to request info.
        @param int, str, or RTTChannelDirection(IntEnum) direction: Direction of the channel to request info.
        @return (str, int): Tuple containing the channel name and the size of channel buffer.
        """
        if not is_u32(channel_index):
            raise ValueError("The channel_index parameter must be an unsigned 32-bit value.")

        if not is_enum(direction, RTTChannelDirection):
            raise ValueError("Parameter direction must be of type int, str or RTTChannelDirection enumeration.")

        direction = decode_enum(direction, RTTChannelDirection)
        if direction is None:
            raise ValueError("Parameter direction must be of type int, str or RTTChannelDirection enumeration.")

        channel_index = ctypes.c_uint32(channel_index)
        direction = ctypes.c_int(direction.value)
        name_len = ctypes.c_uint32(32)
        name = (ctypes.c_uint8 * 32)()
        size = ctypes.c_uint32()

        result = self._lib.NRFJPROG_rtt_read_channel_info_inst(
            self._handle,
            channel_index,
            direction,
            ctypes.byref(name),
            name_len,
            ctypes.byref(size),
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return "".join(chr(i) for i in name if i != 0), size.value

    def is_qspi_init(self):
        """
        Checks if the QSPI peripheral is initialized.

        @return bool: True if initialized.
        """
        initialized = ctypes.c_bool()

        result = self._lib.NRFJPROG_is_qspi_init_inst(self._handle, ctypes.byref(initialized))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return initialized.value

    def qspi_init(self, retain_ram=False, init_params=None):
        """
        Configures and initializes the QSPI peripheral.

        @param (optional) bool retain_ram: Retain contents of device RAM used in the QSPI operations. RAM contents will be restored in qspi_uninit() operation.
        @param (optional) QSPIInitParams init_params: Configuration for the QSPI operations.
        """

        class _CtypesQSPIInitParams(ctypes.Structure):
            _fields_ = [
                ("read_mode", ctypes.c_int),
                ("write_mode", ctypes.c_int),
                ("address_mode", ctypes.c_int),
                ("frequency", ctypes.c_int),
                ("spi_mode", ctypes.c_int),
                ("sck_delay", ctypes.c_uint32),
                ("custom_instruction_io2_level", ctypes.c_int),
                ("custom_instruction_io3_level", ctypes.c_int),
                ("CSN_pin", ctypes.c_uint32),
                ("CSN_port", ctypes.c_uint32),
                ("SCK_pin", ctypes.c_uint32),
                ("SCK_port", ctypes.c_uint32),
                ("DIO0_pin", ctypes.c_uint32),
                ("DIO0_port", ctypes.c_uint32),
                ("DIO1_pin", ctypes.c_uint32),
                ("DIO1_port", ctypes.c_uint32),
                ("DIO2_pin", ctypes.c_uint32),
                ("DIO2_port", ctypes.c_uint32),
                ("DIO3_pin", ctypes.c_uint32),
                ("DIO3_port", ctypes.c_uint32),
                ("WIP_index", ctypes.c_uint32),
                ("pp_size", ctypes.c_int),
            ]

        if not is_bool(retain_ram):
            raise ValueError("The retain_ram parameter must be a boolean value.")

        if not is_right_class(init_params, QSPIInitParams) and init_params is not None:
            raise ValueError("The init_params parameter must be an instance of class QSPIInitParams.")

        if init_params is None:
            init_params = QSPIInitParams()

        retain_ram = ctypes.c_bool(retain_ram)
        qspi_init_params = _CtypesQSPIInitParams(
            init_params.read_mode,
            init_params.write_mode,
            init_params.address_mode,
            init_params.frequency,
            init_params.spi_mode,
            init_params.sck_delay,
            init_params.custom_instruction_io2_level,
            init_params.custom_instruction_io3_level,
            init_params.CSN_pin,
            init_params.CSN_port,
            init_params.SCK_pin,
            init_params.SCK_port,
            init_params.DIO0_pin,
            init_params.DIO0_port,
            init_params.DIO1_pin,
            init_params.DIO1_port,
            init_params.DIO2_pin,
            init_params.DIO2_port,
            init_params.DIO3_pin,
            init_params.DIO3_port,
            init_params.WIP_index,
            init_params.pp_size,
        )

        result = self._lib.NRFJPROG_qspi_init_inst(self._handle, retain_ram, ctypes.byref(qspi_init_params))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_init_ini(self, ini_path=QSPIIniFile):
        """
        Configures and initializes the QSPI peripheral using the specified ini config file. If no ini_path is specified, the default
        settings file QspiDefault.ini will be used.

        @param Path ini_path: Path to ini file containing qspi setup.
        """
        ini_path = str(ini_path).encode("utf-8")
        result = self._lib.NRFJPROG_qspi_init_ini_inst(self._handle, ini_path)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_start(self):
        """
        Initializes the QSPI peripheral.
        QSPI peripheral must be configured before calling this function, see functions 'qspi_configure' and 'qspi_configure_ini'.
        The QSPi peripheral can be configured and initialized in the same operation using functions 'qspi_init' or 'qspi_init_ini'.
        """
        result = self._lib.NRFJPROG_qspi_start_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_configure(self, retain_ram=False, init_params=None):
        """
        Configures the QSPI peripheral.

        @param (optional) bool retain_ram: Retain contents of device RAM used in the QSPI operations. RAM contents will be restored in qspi_uninit() operation.
        @param (optional) QSPIInitParams init_params: Configuration for the QSPI operations.
        """

        class _CtypesQSPIInitParams(ctypes.Structure):
            _fields_ = [
                ("read_mode", ctypes.c_int),
                ("write_mode", ctypes.c_int),
                ("address_mode", ctypes.c_int),
                ("frequency", ctypes.c_int),
                ("spi_mode", ctypes.c_int),
                ("sck_delay", ctypes.c_uint32),
                ("custom_instruction_io2_level", ctypes.c_int),
                ("custom_instruction_io3_level", ctypes.c_int),
                ("CSN_pin", ctypes.c_uint32),
                ("CSN_port", ctypes.c_uint32),
                ("SCK_pin", ctypes.c_uint32),
                ("SCK_port", ctypes.c_uint32),
                ("DIO0_pin", ctypes.c_uint32),
                ("DIO0_port", ctypes.c_uint32),
                ("DIO1_pin", ctypes.c_uint32),
                ("DIO1_port", ctypes.c_uint32),
                ("DIO2_pin", ctypes.c_uint32),
                ("DIO2_port", ctypes.c_uint32),
                ("DIO3_pin", ctypes.c_uint32),
                ("DIO3_port", ctypes.c_uint32),
                ("WIP_index", ctypes.c_uint32),
                ("pp_size", ctypes.c_int),
            ]

        if not is_bool(retain_ram):
            raise ValueError("The retain_ram parameter must be a boolean value.")

        if not is_right_class(init_params, QSPIInitParams) and init_params is not None:
            raise ValueError("The init_params parameter must be an instance of class QSPIInitParams.")

        if init_params is None:
            init_params = QSPIInitParams()

        retain_ram = ctypes.c_bool(retain_ram)
        qspi_init_params = _CtypesQSPIInitParams(
            init_params.read_mode,
            init_params.write_mode,
            init_params.address_mode,
            init_params.frequency,
            init_params.spi_mode,
            init_params.sck_delay,
            init_params.custom_instruction_io2_level,
            init_params.custom_instruction_io3_level,
            init_params.CSN_pin,
            init_params.CSN_port,
            init_params.SCK_pin,
            init_params.SCK_port,
            init_params.DIO0_pin,
            init_params.DIO0_port,
            init_params.DIO1_pin,
            init_params.DIO1_port,
            init_params.DIO2_pin,
            init_params.DIO2_port,
            init_params.DIO3_pin,
            init_params.DIO3_port,
            init_params.WIP_index,
            init_params.pp_size,
        )

        result = self._lib.NRFJPROG_qspi_configure_inst(self._handle, retain_ram, ctypes.byref(qspi_init_params))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_configure_ini(self, ini_path=QSPIIniFile):
        """
        Configures the QSPI peripheral using the specified ini config file. If no ini_path is specified, the default
        settings file QspiDefault.ini will be used.

        @param Path ini_path: Path to ini file containing qspi setup.
        """
        ini_path = str(ini_path).encode("utf-8")
        result = self._lib.NRFJPROG_qspi_configure_ini_inst(self._handle, ini_path)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_uninit(self):
        """
        Uninitializes the QSPI peripheral.

        """
        result = self._lib.NRFJPROG_qspi_uninit_inst(self._handle)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_set_rx_delay(self, rx_delay):
        """
        Set QSPI RX delay.

        @param int rx_delay: Rx delay to set. See the product specification of your device for possible values.

        """
        if not is_u8(rx_delay):
            raise ValueError("The rx_delay parameter must be an unsigned 8-bit value.")

        rx_delay = ctypes.c_uint8(rx_delay)

        result = self._lib.NRFJPROG_qspi_set_rx_delay_inst(self._handle, rx_delay)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_set_size(self, size):
        """
        Set QSPI size. This size is used by 'read_to_file' when reading qspi memory.

        @param int size: QSPI size in bytes
        """
        if not is_u32(size):
            raise ValueError("The size parameter must be an unsigned 32-bit value")

        size = ctypes.c_uint32(size)
        result = self._lib.NRFJPROG_qspi_set_size_inst(self._handle, size)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_get_size(self):
        """
        Get previously configured QSPI size.
        """
        size = ctypes.c_uint32(0)
        result = self._lib.NRFJPROG_qspi_get_size_inst(self._handle, ctypes.byref(size))
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return size.value

    def qspi_read(self, addr, length):
        """
        Reads from the external QSPI-connected memory.

        @param int addr: Address to read from.
        @param int length: Number of bytes to read.
        @return bytearray: Data read.
        """
        if not is_u32(addr):
            raise ValueError("The addr parameter must be an unsigned 32-bit value.")

        if not is_u32(length):
            raise ValueError("The length parameter must be an unsigned 32-bit value.")

        addr = ctypes.c_uint32(addr)
        length = ctypes.c_uint32(length)
        data = (ctypes.c_uint8 * length.value)()

        result = self._lib.NRFJPROG_qspi_read_inst(self._handle, addr, ctypes.byref(data), length)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        return bytearray(data)

    def qspi_write(self, addr, data):
        """
        Writes to the external QSPI-connected memory.

        @param int addr: Address to write to.
        @param sequence data: Data to write. Any type that implements the sequence API (i.e. string, list, bytearray...) is valid as input.
        """
        if not is_u32(addr):
            raise ValueError("The addr parameter must be an unsigned 32-bit value.")

        if not is_valid_buf(data):
            raise ValueError("The data parameter must be a sequence type with at least one item.")

        addr = ctypes.c_uint32(addr)
        data_len = ctypes.c_uint32(len(data))
        data = (ctypes.c_uint8 * data_len.value)(*data)

        result = self._lib.NRFJPROG_qspi_write_inst(self._handle, addr, ctypes.byref(data), data_len)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_erase(self, addr, length):
        """
        Erases the external QSPI-connected memory.

        @param int addr: Address to erase.
        @param int, str, or QSPIEraseLen(IntEnum) length: Erase length.
        """
        if not is_u32(addr):
            raise ValueError("The addr parameter must be an unsigned 32-bit value.")

        if not is_enum(length, QSPIEraseLen):
            raise ValueError("Parameter length must be of type int, str or QSPIEraseLen enumeration.")

        length = decode_enum(length, QSPIEraseLen)
        if length is None:
            raise ValueError("Parameter length must be of type int, str or QSPIEraseLen enumeration.")

        addr = ctypes.c_uint32(addr)
        length = ctypes.c_int(length)

        result = self._lib.NRFJPROG_qspi_erase_inst(self._handle, addr, length)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def qspi_custom(self, code, length, data_in=None, output=False):
        """
        Sends a custom instruction to the external QSPI-connected memory.

        @param int code: Code of the custom instruction.
        @param int length: Lenght of the custom instruction.
        @param (optional) sequence data_in: Data to send in the custom instruction. Any type that implements the sequence API (i.e. string, list, bytearray...) is valid as input.
        @param (optional) bool output: Ouput from the custom instruction is desired or not.
        @return None or bytearray: Custom instruction received data. Return type depends on output optional parameter.
        """
        if not is_u8(code):
            raise ValueError("The code parameter must be an unsigned 8-bit value.")

        if not is_u32(length):
            raise ValueError("The length parameter must be an unsigned 32-bit value.")

        if not is_valid_buf(data_in) and data_in is not None:
            raise ValueError("The data_in parameter must be a sequence type with at least one item.")

        if not is_bool(output):
            raise ValueError("The output parameter must be a boolean value.")

        code = ctypes.c_uint8(code)
        length = ctypes.c_uint32(length)
        data_in = (ctypes.c_uint8 * (length.value - 1))(*data_in) if data_in is not None else None
        data_out = (ctypes.c_uint8 * (length.value - 1))() if output else None

        result = self._lib.NRFJPROG_qspi_custom_inst(
            self._handle,
            code,
            length,
            ctypes.byref(data_in) if data_in is not None else None,
            ctypes.byref(data_out) if data_out is not None else None,
        )
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

        if output:
            return bytearray(data_out)


    def program_file(self, file_path):
        """
        Programs provided file to connected device.
        The device memory is not checked before or after writing. To ensure flash is empty before writing use
        function erase_file(). To verify after writing, use function verify_file().
        Supported file types are .hex, .elf, .bin, and .zip.
        QSPI peripheral must be configured before attempting operations that include external memory.

        This function can be used to upgrade modem firmware on nRF91 devices by passing pa

        @param Path file_path : Path to file to program.
        """
        file_path = str(file_path).encode("utf-8")
        result = self._lib.NRFJPROG_program_file_inst(self._handle, file_path)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def read_to_file(self, file_path, read_options=None):
        """
        Reads the memory content of the connected device and stores it in the provided file_path.
        Supported file types are .hex, .elf, and .bin.
        QSPI peripheral must be configured before attempting operations that include external memory.

        @param Path file_path: Path to store output.
        @param Parameters.ReadOptions read_options: Types of memories to read.
        """
        file_path = str(file_path).encode("utf-8")
        if read_options is None:
            read_options = Parameters.ReadOptions(readcode=True)
        if not isinstance(read_options, ReadOptions):
            raise TypeError("The program_options parameter must be an instance of class ReadOptions.")

        result = self._lib.NRFJPROG_read_to_file_inst(self._handle, file_path, read_options)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def verify_file(self, file_path, verify_action=VerifyAction.VERIFY_READ):
        """
        Verifies that the data in the provided file matches the memory contents of the connected device.
        Supported file types are .hex, .elf, .bin, and .zip.
        QSPI peripheral must be configured before attempting operations that include external memory.

        @param Path file_path: Path to file to verify.
        @param Parameters.VerifyAction verify_action: Type of verify operation.
        """
        if not isinstance(verify_action, VerifyAction):
            raise TypeError("Parameter verify_action must be of type int, str or VerifyAction enumeration.")

        verify_action = ctypes.c_int(decode_enum(verify_action, VerifyAction))

        file_path = str(file_path).encode("utf-8")

        result = self._lib.NRFJPROG_verify_file_inst(self._handle, file_path, verify_action)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())

    def erase_file(
        self,
        file_path,
        chip_erase_mode=EraseAction.ERASE_ALL,
        qspi_erase_mode=EraseAction.ERASE_NONE,
    ):
        """
        Erases sectors of non-volatile memory in which the provided file has data.
        Supported file types are .hex, .elf, .bin, and .zip.
        QSPI peripheral must be configured before attempting operations that include external memory.

        @param Path file_path: Path to file to erase.
        @param Parameters.EraseAction chip_erase_mode: Erase method for internal memories.
        @param Parameters.EraseAction qspi_erase_mode: Erase method for external memory.
        """
        if not isinstance(chip_erase_mode, EraseAction):
            raise TypeError("Parameter erase_action must be of type int, str or EraseAction enumeration.")
        if not isinstance(qspi_erase_mode, EraseAction):
            raise TypeError("Parameter erase_action must be of type int, str or EraseAction enumeration.")

        file_path = str(file_path).encode("utf-8")
        chip_erase_mode = ctypes.c_int(decode_enum(chip_erase_mode, EraseAction))
        qspi_erase_mode = ctypes.c_int(decode_enum(qspi_erase_mode, EraseAction))

        result = self._lib.NRFJPROG_erase_file_inst(self._handle, file_path, chip_erase_mode, qspi_erase_mode)
        if result != NrfjprogdllErr.SUCCESS:
            raise APIError(result, error_data=self.get_errors())


    """
    Internal helper functions.

    """

    @staticmethod
    def __log(msg, log_str, log_file):
        try:
            print(
                "{} {} {}".format(
                    datetime.datetime.now().replace(microsecond=0).isoformat(" "),
                    log_str,
                    decode_string(msg).strip(),
                ),
                file=log_file,
            )
        except (ValueError, OSError):
            pass

    def _set_logger_id(self, id):
        if self._logger is not None:
            self._logger.set_id(id)


    def __enter__(self):
        """
        Called automatically when the 'with' construct is used.

        """
        self.open()
        return self

    def __exit__(self, type, value, traceback):
        """
        Called automatically when the 'with' construct is used.

        """
        self.close()
