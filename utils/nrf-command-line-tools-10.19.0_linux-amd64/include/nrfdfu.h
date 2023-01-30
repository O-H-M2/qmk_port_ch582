#ifndef NRFDFU_H
#define NRFDFU_H

#include "DllCommonDefinitions.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum
{
    IPCEVENT_NONE,
    IPCEVENT_FAULT,
    IPCEVENT_COMMAND,
    IPCEVENT_DATA
} nrfdfu_ipc_event_t;

typedef struct
{
    uint32_t id[10];
} ipc_dfu_id_t;

/* Old definition kept for backwards compatibility. */
typedef nrfjprog_inst_t connection_handle_t;

/**
 * @brief   Deprecated, use nrfjprog.dll
 *
 * @retval  NOT_IMPLEMENTED_ERROR
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_connect_to_ipc_target(nrfjprog_inst_t * handle,
                                                            const char * jlink_path_cstr,
                                                            device_family_t family,
                                                            coprocessor_t coprocessor,
                                                            uint32_t snr,
                                                            uint32_t swd_frequency,
                                                            msg_callback_ex * callback,
                                                            void * param,
                                                            void * prog_cb);

/**
 * @brief   Initialize MCUBoot DFU connection.
 *
 * @details Initializes connection and starts the DFU session with the device.
 *
 * @post    The initial settings for the DFU will be activated.
 *
 * @param   handle                              Pointer to connection handle to initialize
 * @param   serial_port                         Serial port to connect to.
 * @param   baud_rate                           Baud rate of serial connection.
 * @param   response_timeout                    Time in ms before serial port should time out while waiting for a response from target.
 * @param   callback                            Pointer to logger callback function to use, or NULL.
 * @param   param                               Parameter to pass to callback function.
 * @param   prog_cb                             Deprecated, progress info is passed back on the info log level.
 *
 * @retval  SUCCESS
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 * @retval  INVALID_PARAMETER                   The handle parameter is NULL.
 * @retval                                      The serial_port parameter is already initialized.
 * @retval  FILE_OPERATION_FAILED               Failed to make temporary files.
 * @retval  TIME_OUT                            The operation timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_connect_to_mcuboot_target(nrfjprog_inst_t * handle,
                                                                const char * serial_port,
                                                                const uint32_t baud_rate,
                                                                const uint32_t response_timeout,
                                                                msg_callback_ex * callback,
                                                                void * param,
                                                                void * prog_cb);

/**
 * @brief   Initialize Modem UART DFU connection.
 *
 * @details Initializes connection and starts the DFU session with the device.
 *
 * @post    The initial settings for the DFU will be activated.
 *
 * @param   handle                              Pointer to connection handle to initialize
 * @param   serial_port                         Serial port to connect to.
 * @param   baud_rate                           Baud rate of serial connection.
 * @param   response_timeout                    Time in ms before serial port should time out while waiting for a response from target.
 * @param   callback                            Pointer to logger callback function to use, or NULL.
 * @param   param                               Parameter to pass to callback function.
 * @param   prog_cb                             Deprecated, progress info is passed back on the info log level.
 *
 * @retval  SUCCESS
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 * @retval  INVALID_PARAMETER                   The handle parameter is NULL.
 * @retval                                      The serial_port parameter is already initialized.
 * @retval  SERIAL_PORT_RESOURCE_ERROR          Unable to open serial port.
 * @retval  TIME_OUT                            The operation timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_connect_to_modemdfu_target(nrfjprog_inst_t * handle,
                                                                 const char * serial_port,
                                                                 const uint32_t baud_rate,
                                                                 const uint32_t response_timeout,
                                                                 msg_callback_ex * callback,
                                                                 void * param,
                                                                 void * prog_cb);

/**
 * @brief   Closes the connection to the underlying DLLs.
 *
 * @details Closes the connection to the underlying DLLs.
 *
 * @pre     handle must point to a handle that was previously initialized with any function NRFDFU_connect_to_*_target().
 *
 * @param   handle                              An initialized connection handle
 *
 * @post    The connection will be closed, and the handle invalidated.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   The handle parameter is NULL.
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_close_connection(nrfjprog_inst_t * handle);

/**
 * @brief   Programs the provided zip file
 *
 * @details Initializes connection and starts the DFU session with the device.
 *
 * @pre     handle must be initialized with any function NRFDFU_connect_to_*_target() before this function is run.
 *
 * @param   handle                              An initialized connection handle
 * @param   package_path                        Path to zip file containing a DFU package compatible with the current DFU connection.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   The handle parameter is NULL.
 * @retval                                      The package_path parameter is NULL.
 * @retval                                      The package_path points to a non existing file.
 * @retval                                      The package_path points to a file without reading rights.
 * @retval  FILE_OPERATION_FAILED               Failed to make temporary files.
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_program_package(nrfjprog_inst_t handle, const char * package_path);

/**
 * @brief   Programs using the list of files provided
 *
 * @details Programs the files provided as if they were part of a zip file passed to program_package
 *
 * @pre     handle must be initialized with the function NRFDFU_connect_to_*_target() before this function is run.
 *
 * @param   handle                              An initialized connection handle
 * @param   paths                               List of zero-terminated C strings containing file paths
 * @param   num_files                           The number of file paths in paths.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   The handle parameter is NULL.
 * @retval                                      The paths points to a non existing file.
 * @retval                                      The paths points to a file without reading rights.
 * @retval  FILE_OPERATION_FAILED               Failed to make temporary files.
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_program_files(nrfjprog_inst_t handle, const char ** paths, uint32_t num_files);

/**
 * @brief   Verifies target firmware.
 *
 * @details Verifies the firmware on the target and checks if the firmware digest is correct.
 *
 * @pre     handle must be initialized with any function NRFDFU_connect_to_*_target() before this function is run.
 *
 * @param   handle                              An initialized connection handle
 * @param   package_path                        Path to package of files to be verified.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   The handle parameter is NULL.
 * @retval                                      The package_path parameter is NULL.
 * @retval                                      The package_path points to a non existing file.
 * @retval                                      The package_path points to a file without reading rights.
 * @retval  FILE_OPERATION_FAILED               Failed to make temporary files.
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_verify_package(nrfjprog_inst_t handle, const char * package_path);

/**
 * @brief   Updates, and verifies target.
 *
 * @details Updates the target firmware with the hex file provided. Then verifies the update and checks if the firmware digest is correct.
 *
 * @pre     handle must be initialized with any function NRFDFU_connect_to_*_target() before this function is run.
 *
 * @param   handle                              An initialized connection handle
 * @param   paths                               List of zero-terminated C strings containing file paths
 * @param   num_files                           The number of file paths in paths.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   The handle parameter is NULL.
 * @retval                                      The paths points to a non existing file.
 * @retval                                      The paths points to a file without reading rights.
 * @retval  FILE_OPERATION_FAILED               Failed to make temporary files.
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_verify_files(nrfjprog_inst_t handle, const char ** paths, uint32_t num_files);

/**
 * @brief   Read part of the target firmware to a buffer.
 * @details Reads length bytes from address into buffer.
 *
 * @pre     handle must be initialized with the function NRFDFU_connect_to_ipc_target() before this function is run.
 *
 * @param   handle                              An initialized connection handle
 * @param   address                             The start address of the read.
 * @param   buffer                              A path to where the output is written.
 * @param   length                              length of reading opperation.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   The handle parameter is NULL.
 * @retval                                      The address parameter is not page aligned.
 * @retval                                      The length parameter is not larger than 0.
 * @retval                                      The length parameter is not a multiple of 4.
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_read(nrfjprog_inst_t handle, uint32_t address, uint8_t * buffer, uint32_t length);

/**
 * @brief   Deprecated
 *
 * @retval  INVALID_OPERATION
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_IPC_get_event_status(nrfjprog_inst_t handle,
                                                           nrfdfu_ipc_event_t event,
                                                           bool * status);

/**
 * @brief   Deprecated
 *
 * @retval  INVALID_OPERATION
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_IPC_acknowledge_event(nrfjprog_inst_t handle, nrfdfu_ipc_event_t event);

/**
 * @brief   Deprecated
 *
 * @retval  INVALID_OPERATION
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_IPC_read_id(nrfjprog_inst_t handle, ipc_dfu_id_t * id);

/**
 * @brief   Deprecated
 *
 * @retval  INVALID_OPERATION
 */
NRFJPROG_API nrfjprogdll_err_t NRFDFU_IPC_read_digest(nrfjprog_inst_t handle, void * digest);

#if defined(__cplusplus)
}
#endif

#endif /* NRFDFU_H */
