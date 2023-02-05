import enum

try:
    from . import Parameters
except Exception:
    import Parameters


@enum.unique
class NrfjprogdllErr(enum.IntEnum):
    """
    Wraps nrfjprogdll_err_t values from DllCommonDefinitions.h

    """
    SUCCESS                                     =    0
    
    OUT_OF_MEMORY                               =   -1
    INVALID_OPERATION                           =   -2
    INVALID_PARAMETER                           =   -3
    INVALID_DEVICE_FOR_OPERATION                =   -4
    WRONG_FAMILY_FOR_DEVICE                     =   -5
    UNKNOWN_DEVICE                              =   -6
    INVALID_SESSION                             =   -7

    EMULATOR_NOT_CONNECTED                      =  -10
    CANNOT_CONNECT                              =  -11
    LOW_VOLTAGE                                 =  -12
    NO_EMULATOR_CONNECTED                       =  -13

    NVMC_ERROR                                  =  -20
    RECOVER_FAILED                              =  -21

    NOT_AVAILABLE_BECAUSE_PROTECTION            =  -90
    NOT_AVAILABLE_BECAUSE_MPU_CONFIG            =  -91
    NOT_AVAILABLE_BECAUSE_COPROCESSOR_DISABLED  =  -92
    NOT_AVAILABLE_BECAUSE_TRUST_ZONE            =  -93
    NOT_AVAILABLE_BECAUSE_BPROT                 =  -94

    JLINKARM_DLL_NOT_FOUND                      = -100
    JLINKARM_DLL_COULD_NOT_BE_OPENED            = -101
    JLINKARM_DLL_ERROR                          = -102
    JLINKARM_DLL_TOO_OLD                        = -103
    JLINKARM_DLL_READ_ERROR                     = -104
    JLINKARM_DLL_TIME_OUT_ERROR                 = -105

    SERIAL_PORT_NOT_FOUND                       = -110
    SERIAL_PORT_PERMISSION_ERROR                = -111
    SERIAL_PORT_WRITE_ERROR                     = -112
    SERIAL_PORT_READ_ERROR                      = -113
    SERIAL_PORT_RESOURCE_ERROR                  = -114
    SERIAL_PORT_NOT_OPEN_ERROR                  = -115

    NRFJPROG_SUB_DLL_NOT_FOUND                  = -150
    NRFJPROG_SUB_DLL_COULD_NOT_BE_OPENED        = -151
    NRFJPROG_SUB_DLL_COULD_NOT_LOAD_FUNCTIONS   = -152
    NRFJPROG_HOST_EXE_NOT_FOUND                 = -153

    VERIFY_ERROR                                = -160
    RAM_IS_OFF_ERROR                            = -161

    FILE_OPERATION_FAILED                       = -162
    FILE_PARSING_ERROR                          = -170
    FILE_UNKNOWN_FORMAT_ERROR                   = -171
    FILE_INVALID_ERROR                          = -172
    UNKNOWN_MEMORY_ERROR                        = -173
    CONFIG_TYPE_ERROR                           = -174
    CONFIG_SYNTAX_ERROR                         = -175
    CONFIG_PROPERTY_MISSING_ERROR               = -176

    PYTHON_ALREADY_INSTANTIATED_ERROR           = -200

    TIME_OUT                                    = -220
    DFU_ERROR                                   = -221

    INTERNAL_ERROR                              = -254
    NOT_IMPLEMENTED_ERROR                       = -255

class APIError(Exception):
    """
    pynrfjprog DLL exception class, inherits from the built-in Exception class.

    """

    def __init__(self, err_code=None, err_msg="", log=None, error_data=None):
        """
        Constructs a new object and sets the err_code.

        @param int err_code: The error code returned by the nrfjprog DLL.
        """

        self.err_code = err_code
        self.err_msg = err_msg
        self.error_enum = NrfjprogdllErr(err_code)
        self.err_str = 'An error was reported by NRFJPROG DLL: {} {}. {}'.format(self.err_code, self.error_enum .name, err_msg).rstrip()

        if error_data:
            error_data = " \n" + ("\n\textra: ".join(error_data))
            self.err_str += error_data

        Exception.__init__(self, self.err_str)

        if log is not None:
            log(self.err_str.encode('utf-8'))

    def __reduce__(self):
        return self.__class__, (self.err_code, self.err_msg, None, )

