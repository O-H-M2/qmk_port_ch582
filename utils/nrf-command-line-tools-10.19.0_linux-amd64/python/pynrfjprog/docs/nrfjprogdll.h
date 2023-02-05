/*
 * Copyright (c) 2010 - 2018, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NRFJPROGDLL_FUNC_H
#define NRFJPROGDLL_FUNC_H

#include "DllCommonDefinitions.h"
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief   Returns the nrfjprog DLL version.
 *
 * @param   major                               Pointer for storing dll major version.
 * @param   minor                               Pointer for storing dll minor version.
 * @param   micro                               Pointer for storing dll micro version.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   The major parameter is NULL.
 *                                              The minor parameter is NULL.
 *                                              The micro parameter is NULL.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_version(uint32_t * major, uint32_t * minor, uint32_t * micro);

/**
 * @brief   Returns the JLinkARM DLL version.
 *
 * @details Returns the JLinkARM DLL version that has been opened in with the NRFJPROG_open_dll() function.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   major                               Pointer for storing of dll major version.
 * @param   minor                               Pointer for storing of dll minor version.
 * @param   revision                            Pointer for storing of dll revision.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The major parameter is NULL.
 *                                              The minor parameter is NULL.
 *                                              The revision parameter is NULL.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_dll_version_inst(nrfjprog_inst_t instance,
                                                         uint32_t * major,
                                                         uint32_t * minor,
                                                         char * revision);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_dll_version(uint32_t * major, uint32_t * minor, char * revision);

/**
 * @brief   Returns the path to the JLinkARM shared library.
 *
 * @details Returns the path to the JLinkARM DLL that has been opened with the NRFJPROG_open_dll() function.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   buffer                              Buffer to store the path.
 * @param   buffer_size                         Size of buffer
 * @param   bytes_copied                        Pointer to variable that will hold the amount of bytes copied to buffer.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The major parameter is NULL.
 *                                              The minor parameter is NULL.
 *                                              The revision parameter is NULL.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_get_jlink_path_inst(nrfjprog_inst_t instance,
                                                            char * buffer,
                                                            const size_t buffer_size,
                                                            size_t * bytes_copied);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_get_jlink_path(char * buffer, const size_t buffer_size, size_t * bytes_copied);

/**
 * @brief   Attempts to find and return the path to the newest JLinkARM shared library installation.
 *
 * @details Attempts to find the newest JLinkARM shared library installation. The path returned will be the same path found when auto-detecting
 *          JLinkARM DLL in function NRFJPROG_open_dll(). Note that NRFJPROG_find_jlink_path() always returns a system
 *          search result, and not which JLinkARM library is in use. See function NRFJPROG_get_jlink_path() for the
 *          latter.
 *
 *          Upon success, the path to the JLinkARM shared library is stored in 'buffer' as a 0-terminated UTF-8 string,
 *          and bytes_copied is updated accordingly. If buffer and buffer_size was passed as NULL and zero, bytes_copied
 *          is set to the total size needed to store the returned JLinkARM path. This can be used to find the buffer
 *          size needed. The size of 'bytes_copied' includes 0-terminator.
 *
 *          The search strategy chosen is dependent on platform. On Windows, Windows Registry is searched.
 *          On OSX the default install location "/Applications/SEGGER/JLink/" is scanned for shared library files.
 *          Similarly, on Linux, the default install location "/opt/SEGGER/JLink/" is checked.
 *          On unix-like systems the function might also return a library name compatible with dlopen if no library file
 *          is found in the default search path.
 *
 * @param   buffer                              Buffer to store the path. Can be NULL if buffer_size is zero.
 * @param   buffer_size                         Length of buffer.
 * @param   bytes_copied                        Pointer to variable that will hold the amount of bytes copied.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   buffer is NULL while buffer_size is non-zero.
 *                                              bytes_copied is NULL.
 * @retval  JLINKARM_DLL_NOT_FOUND              No search results yielded.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_find_jlink_path(char * buffer, uint32_t buffer_size, uint32_t * bytes_copied);

/**
 * @brief   Checks if the library is ready to execute commands.
 *
 * @details For NRFJPROG_is_dll_open_inst():
 *          Checks if the instance variable provided points to a valid and open nrfjprogdll instance.
 *
 *          For NRFJPROG_is_dll_open():
 *          NRFJPROG_open_dll() function has been called since the last call to NRFJPROG_close_dll().
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   opened                              Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called. or instance has been closed.
 * @retval  INVALID_PARAMETER                   The opened parameter is NULL.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_dll_open_inst(nrfjprog_inst_t instance, bool * opened);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_dll_open(bool * opened);

/**
 * @brief   Opens the JLinkARM DLL and sets the log callback. Prepares the dll for work with the specified family.
 *
 * @details This function opens the JLinkARM DLL using the received path. The path should include the name of the DLL
 *          itself (i.e. "JLinkARM.dll"). Only JLinkARM DLLs whose versions are greater than a minimum version will be
 *          accepted. The minimum version for the JLinkARM DLL is defined in MIN_JLINK_MAJOR_VERSION and
 *          MIN_JLINK_MINOR_VERSION defines. The log callback may be NULL. In that case no logging mechanism is
 *          provided. The msg_callback typedef is defined in DllCommonDefinitions.h. To close the dll, see
 *          NRFJPROG_close() function.
 *
 * @pre     The dll must not be open. To close the dll, see NRFJPROG_close_dll()
 *          function.
 *
 * @post    The JLINKARM DLL pointers will be loaded and some memory reserved. To
 *          unload the pointers and free the memory, see NRFJPROG_close_dll() function.
 * @post    The instance variable pointed to by instance_ptr will refer to the
 *          opened nrfjprog session.
 *
 * @param   instance_ptr                        Pointer to variable that will store the working instance of nrfjprog.
 * @param   jlink_path                          Path to the JLinkARM DLL. Does not support unicode paths. If NULL or
 *                                              nullptr, nrfjprog will attempt to find the newest installed J-Link Dll.
 * @param   log_cb                              Callback for reporting informational and error messages. May be NULL.
 * @param   log_param                           User-selectable value that will be passed back when calling log_cb in the _ex version of this function.
 *                                              log_param will not be dereferenced. May be NULL.
 * @param   family                              Defines the device family the next commands are going to be called to.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance_ptr is a NULL pointer.
 * @retval  INVALID_OPERATION                   The NRFJPROG_open_dll() function has already been called.
 * @retval  INVALID_PARAMETER                   The provided device family is not supported by this DLL.
 * @retval  JLINKARM_DLL_TOO_OLD                The version of JLinkARM is lower than the minimum version required.
 * @retval  JLINKARM_DLL_NOT_FOUND              The jlink_path did not yield a usable DLL, or the automatic search
 *                                              failed.
 * @retval  JLINKARM_DLL_COULD_NOT_BE_OPENED    An error occurred while opening the JLinkARM DLL.
 *                                              A required function could not be loaded from the DLL.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_open_dll_inst(nrfjprog_inst_t * instance_ptr,
                                                      const char * jlink_path,
                                                      msg_callback_ex * log_cb,
                                                      void * log_param,
                                                      device_family_t family);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_open_dll(const char * jlink_path, msg_callback * cb, device_family_t family);

/**
 * @brief   Closes and frees the JLinkARM DLL.
 *
 * @details Closes and frees the JLinkARM DLL. This function needs to be called before exiting if NRFJPROG_open_dll()
 *          has been called. After the execution of this function, the device CPU will not change its state from running
 *          or halted. Before closing the J-Link session, this function ensures all RTT messages are written to target
 *          as if NRFJPROG_rtt_async_flush() was called.
 *
 * @post    The JLINKARM DLL function pointers will be unloaded and the reserved
 *          memory freed. To open the dll, see NRFJPROG_open_dll() function.
 * @post    The device will not be in debug interface mode. To enter debug
 *          interface mode, see NRFJPROG_connect_to_device() function.
 * @post    The PC will be disconnected from an emulator. To connect to an
 *          emulator, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @post    The emulator will be disconnected from the device. To connect to the
 *          device, see NRFJPROG_connect_to_device() function.
 * @post    The trace and debug resources will be switched off. To switch on the
 *          debug resources, see NRFJPROG_connect_to_device() function.
 * @post    The instance pointer will be cleared, and the memory associated with
 *          the instace will be freed.
 *
 * @param   instance_ptr                        Pointer to instance variable that will be closed.
 */
NRFJPROG_API void NRFJPROG_close_dll_inst(nrfjprog_inst_t * instance_ptr);
NRFJPROG_API void NRFJPROG_close_dll(void);

/**
 * @brief   Configure dll behavior
 *
 * @details This function loads configuration data from the referenced .toml file or a toml-formatted string buffer.
 *          See example file config.toml for confguration description and default values.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   config_file_cstr                    UTF-8-encoded path to configuration file
 *                                              or a c-string buffer containing toml-
 *                                              formatted contents.
 * @param   type                                Type of data passed to config_file_cstr.
 *                                                  If FILE_INPUT_PATH, config_file_cstr is interpreted as a UTF-8
 *                                                  encoded file path that points to the configuration file to read.
 *
 *                                                  If FILE_INPUT_BUFFER, config_file_cstr is interpreted as a UTF-8
 *                                                  encoded buffer containing configuration data in toml format.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll()
 *                                              function has not been called.
 * @retval  INTERNAL_ERROR                      An internal error has occured.
 * @retval  INVALID_PARAMETER                   The config_file_cstr parameter is NULL.
 **/
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_configure_inst(nrfjprog_inst_t instance,
                                                       const char * config_file_cstr,
                                                       file_input_type_t type);

/**
 * @brief   Enumerates all serial ports connected to a given Segger debug probe
 *
 * @details This function finds all serial ports hosted by a given debug probe.
 *          The number of serial ports found is written into the num_com_ports parameter. It also copies
 *          up to com_ports_len com_port_info_t objects into the com_ports array parameter.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   serial_number                       Serial number of the debug probe to find the serial port of.
 * @param   com_ports                           Array in which to store the enumerated serial ports.
 * @param   com_ports_len                       Number of com_port_info_t values that can be stored in the
 *                                              com_ports array.
 * @param   num_com_ports                       The number of serial ports that were discovered.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll()
 *                                              function has not been called.
 * @retval  INTERNAL_ERROR                      An internal error has occured.
 * @retval  INVALID_PARAMETER                   The com_ports parameter is NULL.
 *                                              The com_ports_len parameter is 0.
 *                                              The num_available parameter is NULL.
 **/
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enum_emu_com_inst(nrfjprog_inst_t instance,
                                                          const uint32_t serial_number,
                                                          com_port_info_t com_ports[],
                                                          const uint32_t com_ports_len,
                                                          uint32_t * num_com_ports);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enum_emu_com(const uint32_t serial_number,
                                                     com_port_info_t com_ports[],
                                                     const uint32_t com_ports_len,
                                                     uint32_t * num_com_ports);

/**
 * @brief   Enumerates the serial numbers of connected USB J-Link emulators.
 *
 * @details This function asks the JLinkARM DLL how many USB J-Link emulators are connected to
 *          the PC, and writes that value into the num_available parameter. It also copies
 *          up to serial_numbers_len serial numbers into the serial_numbers array parameter.
 *          The function can be called with serial_numbers set to NULL and serial_numbers_len
 *          set to zero to obtain the number of connected emulators.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   serial_numbers                      Array in which to store the enumerated serial numbers.
 * @param   serial_numbers_len                  Number of uint32_t values that can be stored in the
 *                                              serial_numbers array (may be zero).
 * @param   num_available                       The number of serial numbers that were enumerated.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  INVALID_PARAMETER                   The serial_numbers parameter is NULL but serial_numbers_len is > 0.
 *                                              The num_available parameter is NULL.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enum_emu_snr_inst(nrfjprog_inst_t instance,
                                                          uint32_t serial_numbers[],
                                                          uint32_t serial_numbers_len,
                                                          uint32_t * num_available);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enum_emu_snr(uint32_t serial_numbers[],
                                                     uint32_t serial_numbers_len,
                                                     uint32_t * num_available);

/**
 * @brief   Enumerates connection information of discoverable J-Link emulators.
 *
 * @details This function uses the J-Link DLL to get a list of discovered emulators. Information about each probe is copied to the array 'emu_infos',
 *          up to a max count of 'emu_infos_len'. 'num_available' is updated with the total number of emulators
 *          discovered by the J-Link DLL. If 'emu_infos' is NULL and 'emu_infos_len' is zero, only 'num_available' will
 *          be updated. Which interfaces are checked is controlled by the parameters 'list_usb_emus' and 'list_ip_emus'.
 *          If only USB devices are needed, the function NRFJPROG_enum_emu_snr() can be used instead.
 *
 *          Note that not all network connected debuggers are discoverable. Generally, debug probes with built in
 *          Ethernet/Wi-Fi support are automatically discoverable, but sessions created through the JLink Remote Server
 *          application are not.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   emu_infos                           Array in which to store the enumerated emulator connection information. Can be NULL.
 * @param   emu_infos_len                       Number of emu_con_info_t structs that can be stored in the emu_infos array.
 * @param   num_available                       The total number of emulators discovered.
 * @param   list_usb_emus                       List emulators discovered on the USB interface.
 * @param   list_ip_emus                        List emulators discovered on the IP/network interface.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  INVALID_PARAMETER                   The emu_infos parameter is NULL but emu_infos_len is > 0.
 *                                              The num_available parameter is NULL.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enum_emu_con_info_inst(nrfjprog_inst_t instance,
                                                               emu_con_info_t emu_infos[],
                                                               uint32_t emu_infos_len,
                                                               uint32_t * num_available,
                                                               bool list_usb_emus,
                                                               bool list_ip_emus);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enum_emu_con_info(emu_con_info_t emu_infos[],
                                                          uint32_t emu_infos_len,
                                                          uint32_t * num_available,
                                                          bool list_usb_emus,
                                                          bool list_ip_emus);

/**
 * @brief   Checks if the emulator has an established connection with Segger emulator/debugger.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_pc_connected_to_emu              Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The is_connected_to_emu pointer is NULL.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_connected_to_emu_inst(nrfjprog_inst_t instance,
                                                                 bool * is_pc_connected_to_emu);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_connected_to_emu(bool * is_pc_connected_to_emu);

/**
 * @brief   Connects to a given emulator/debugger.
 *
 * @details This function connects to serial_number emulator and sets the SWD communication speed at clock_speed_in_khz.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must not be established. To disconnect from an emulator, see NRFJPROG_disconnect_from_emu() function.
 * @pre     The emulator must be physically connected to a powered board.
 *
 * @post    The PC will be connected to an emulator. To disconnect to the emulator, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   serial_number                       Serial number of the emulator to connect to.
 * @param   clock_speed_in_khz                  Speed for the SWD communication. It must be between JLINKARM_SWD_MIN_SPEED_KHZ
 *                                              and JLINKARM_SWD_MAX_SPEED_KHZ defined in DllCommonDefinitions.h. If the
 *                                              emulator does not support the input clock_speed, the emulators maximum
 *                                              supported speed will be used.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has already been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  INVALID_PARAMETER                   The clock_speed_in_khz parameter is not within limits.
 * @retval  EMULATOR_NOT_CONNECTED              The serial_number emulator is not connected to the PC.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_connect_to_emu_with_snr_inst(nrfjprog_inst_t instance,
                                                                     uint32_t serial_number,
                                                                     uint32_t clock_speed_in_khz);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_connect_to_emu_with_snr(uint32_t serial_number, uint32_t clock_speed_in_khz);

/**
 * @brief   Connects to an emulator/debugger.
 *
 * @details This function connects to an available emulator and sets the SWD communication speed at clock_speed_in_khz.
 *          If more than one emulator is available, a pop-up window will appear to make a selection.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must not be established. To disconnect from an emulator, see NRFJPROG_disconnect_from_emu() function.
 * @pre     The emulator must be physically connected to a powered board.
 *
 * @post    The PC will be connected to an emulator. To disconnect to the emulator, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   clock_speed_in_khz                  Speed for the SWD communication. It must be between JLINKARM_SWD_MIN_SPEED_KHZ
 *                                              and JLINKARM_SWD_MAX_SPEED_KHZ defined in DllCommonDefinitions.h. If the
 *                                              emulator does not support the input clock_speed, the emulators maximum
 *                                              supported speed will be used.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has already been called.
 * @retval  NO_EMULATOR_CONNECTED               There is no emulator connected to the PC.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  INVALID_PARAMETER                   The clock_speed_in_khz parameter is not within limits.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The target debug port IDR does not match the expected value of an SoC in the family configured in open_dll.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_connect_to_emu_without_snr_inst(nrfjprog_inst_t instance,
                                                                        uint32_t clock_speed_in_khz);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_connect_to_emu_without_snr(uint32_t clock_speed_in_khz);

/**
 * @brief   Connects to a given emulator/debugger
 *
 * @details Connect to an emulator over IP. The target emulator can be specified either by hostname and port, OR by serial number.
 *          When a hostname is provided, the serial number argument is ignored.
 *
 *          The hostname c-string can be an IPv4 address or hostname. IPv6 is currently not supported. The hostname can
 *          also be used to connect to a JLink emulator that is connected in tunnel mode. Example of hostname strings:
 *          IPv4: "192.168.0.1" Hostname: "myserver.com", "localhost" Tunnel:
 *          "tunnel:123456789:test:jlink-europe.segger.com"
 *
 *          In the tunnel example above a devkit with serial number '123456789' is connected to the official
 *          jlink-europe.segger.com tunnel server using the J-Link remote Server application with password "test". Read
 *          more about JLink Remote Server here https://wiki.segger.com/J-Link_Remote_Server .
 *
 *          If a serial number is passed while hostname is NULL, the serial number will be looked for in a list of JLink
 *          emulators that has been discovered on the network. Note that not all network sessions are discoverable.
 *          Generally, debug probes with built in Ethernet/Wi-Fi support are automatically discoverable, but sessions
 *          created through the JLink Remote Server application are not. The list of discovered network debuggers can be
 *          retrieved using function NRFJPROG_enum_emu_con_info().
 *
 *          Not specifying hostname nor serial number will result in a pop-up window appearing for the user to make a
 *          selection. This is done by passing NULL for 'ip' and NRFJPROG_INVALID_EMU_SNR for 'serial_number'.
 *          NRFJPROG_INVALID_EMU_SNR is defined in DllCommonDefinitions.h.
 *
 *          Tip: The function NRFJPROG_read_connected_emu_snr() can be used to check the serial number of an emulator
 *          after connecting.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must not be established. To disconnect from an emulator, see NRFJPROG_disconnect_from_emu() function.
 * @pre     The emulator must be physically connected to a powered board.
 *
 * @post    The PC will be connected to an emulator. To disconnect to the emulator, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   hostname                            The hostname/IP address to connect to (C-string). Can be NULL.
 * @param   port                                Specifies which port to use when connecting. Only applies when the 'hostname' parameter is set. Pass '0' for default port (19020).
 * @param   serial_number                       Serial number of the emulator to connect to. Pass NRFJPROG_INVALID_EMU_SNR for no serial number.
 * @param   clock_speed_in_khz                  Speed for the SWD communication. It must be between JLINKARM_SWD_MIN_SPEED_KHZ
 *                                              and JLINKARM_SWD_MAX_SPEED_KHZ defined in DllCommonDefinitions.h. If the
 *                                              emulator does not support the input clock_speed, the emulators maximum
 *                                              supported speed will be used.
 *
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has already been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  INVALID_PARAMETER                   The clock_speed_in_khz parameter is not within limits.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_connect_to_emu_with_ip_inst(nrfjprog_inst_t instance,
                                                                    const char * hostname,
                                                                    uint16_t port,
                                                                    uint32_t serial_number,
                                                                    uint32_t clock_speed_in_khz);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_connect_to_emu_with_ip(const char * hostname,
                                                               uint16_t port,
                                                               uint32_t serial_number,
                                                               uint32_t clock_speed_in_khz);

/**
 * @brief   Reads the serial number of the emulator connected to.
 *
 * @details Reads the serial number of the emulator connected to.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   serial_number                       Pointer to variable that will contain the snr.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The serial_number pointer is NULL.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_connected_emu_snr_inst(nrfjprog_inst_t instance, uint32_t * serial_number);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_connected_emu_snr(uint32_t * serial_number);

/**
 * @brief   Attempts to reset the connected J-Link OB.
 *
 * @details Resets and reconnects to the J-Link OB.
 *          This operation is only available in debug probes of type J-Link OB-SAM3U128-V2-NordicSemi.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection,
 *          see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The PC will still be connected to the emulator.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has not been called.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected debug probe does not support the ResetJLink command.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error, check log for details.
 * @retval  EMULATOR_NOT_CONNECTED              The emulator did not successfully reenumerate within 10s after the reset.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_reset_connected_emu_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_reset_connected_emu(void);

/**
 * @brief   Replaces the firmware on the connected J-Link debug probe.
 *
 * @details Replaces the firmware on the selected debug probe.
 *          The debug probe firmware is replaced with the fw version that shipped with the J-Link DLL selected in
 *          open_dll() even if a newer version is already present.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection,
 *          see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The debug probe will have been reset.
 * @post    The PC will still be connected to the emulator.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error, check log for details.
 * @retval  EMULATOR_NOT_CONNECTED              The emulator did not successfully reenumerate within 10s after the reset.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_replace_connected_emu_fw_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_replace_connected_emu_fw(void);

/**
 * @brief   Reads the firmware identification string of the emulator connected to.
 *
 * @details This function reads the firmware identification string of the emulator connected to into the
 *          given buffer. The function will read a maximum of buffer_size-1 characters into the buffer, and 0-terminate
 *          it. Any excess characters are not read.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   buffer                              Pointer to buffer to contain the firmware string.
 * @param   buffer_size                         Size of the buffer. The user is responsible for ensuring a big enough buffer. A 255 byte buffer is suggested.
 *                                              Maximum buffer_size value is INT_MAX (2147483647).
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   NRFJPROG_open_dll() function has not been called.
 *                                              NRFJPROG_connect_to_emu_with_snr() or
 *                                              NRFJPROG_connect_to_emu_without_snr() has not been called.
 * @retval  INVALID_PARAMETER                   The character buffer pointer is a NULL-pointer.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_connected_emu_fwstr_inst(nrfjprog_inst_t instance,
                                                                      char * buffer,
                                                                      uint32_t buffer_size);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_connected_emu_fwstr(char * buffer, uint32_t buffer_size);

/**
 * @brief   Disconnects from an emulator.
 *
 * @details This function disconnects from a connected emulator. This also disconnects from a connected device if connected. Will
 *          not fail if we have never connected to an emulator. After the execution of this function, the device CPU
 *          will not change its state from running or halted.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @post    The device will not be in debug interface mode. To enter debug interface mode, see NRFJPROG_connect_to_device() function.
 * @post    The PC will be disconnected from an emulator. To connect to an emulator, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @post    The emulator will be disconnected from the device. To connect to the device, see NRFJPROG_connect_to_device() function.
 * @post    The trace and debug resources will be switched off. To switch on the debug resources, see NRFJPROG_connect_to_device() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_disconnect_from_emu_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_disconnect_from_emu(void);

/**
 * @brief   Selects a new family
 *
 * @details This function switches to the dll api that belongs to the family passed in. J-Link state is preserved.
 *          Its primary use is for switching from UNKNOWN family to a known family, but the function also supports other
 *          switches. No family-specific cleanup is performed before switching family. There's a risk that the device
 *          will be left in an unknown state when other transitions are done than from UNKNOWN_FAMILY.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @post    The api will call the functions belonging to the selected family dll.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   family                              New family to interact with
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_select_family_inst(nrfjprog_inst_t instance, device_family_t family);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_select_family(device_family_t family);

/**
 * @brief   Checks if the coprocessor is enabled.
 *
 * @details Checks if nrfjprog has enabled the coprocessor using a device-dependent force-on routine.
 *          Even if this method returns false, the coprocessor may be available due to the hw implementation.
 *          For coprocessors that cannot be disabled, this method always returns true.
 *          For devices with a single processor, this method always returns true for CP_APPLICATION.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_coprocessor_enabled              Pointer of the location to store the result.
 * @param   coprocessor                         The targeted coprocessor.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The is_coprocessor_enabled is NULL.
 *                                              The coprocessor is not a valid coprocessor.
 * @retval  UNAVAILABLE_BECAUSE_PROTECTION      The Application coprocessor is readback protected, making the POWER.LTEMODEM registers unavailable.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_coprocessor_enabled_inst(nrfjprog_inst_t instance,
                                                                    coprocessor_t coprocessor,
                                                                    bool * is_coprocessor_enabled);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_coprocessor_enabled(coprocessor_t coprocessor,
                                                               bool * is_coprocessor_enabled);

/**
 * @brief   Enables the coprocessor.
 *
 * @details Enables the coprocessor by performing a device-dependent force-on routine.
 *          Enabling coprocessors that cannot be disabled returns SUCCESS without performing any actions.
 *          For devices with a single processor, this method always succeeds, but does nothing.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   coprocessor                         The targeted coprocessor.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The coprocessor is not a valid coprocessor.
 * @retval  UNAVAILABLE_BECAUSE_PROTECTION      The Application coprocessor is readback protected, making the POWER.LTEMODEM registers unavailable.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enable_coprocessor_inst(nrfjprog_inst_t instance, coprocessor_t coprocessor);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enable_coprocessor(coprocessor_t coprocessor);

/**
 * @brief   Disables the coprocessor.
 *
 * @details Disables the coprocessor by relaxing the device-dependent force-on routine used in NRFJPROG_enable_coprocessor().
 *          For devices with a single processor, this method always succeeds, but does nothing.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   coprocessor                         The targeted coprocessor.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The coprocessor is not a valid coprocessor.
 * @retval  UNAVAILABLE_BECAUSE_PROTECTION      The Application coprocessor is readback protected, making the POWER.LTEMODEM registers unavailable.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_disable_coprocessor_inst(nrfjprog_inst_t instance, coprocessor_t coprocessor);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_disable_coprocessor(coprocessor_t coprocessor);

/**
 * @brief   Selects which coprocessor to connect to.
 *
 * @details Selects which coprocessor for the NRFJPROG_connect_to_device() function.
 *          If application coprocessor is readback protected, the function still attempts to select the target
 *          coprocessor. This may fail in a later function call if the coprocessor is disabled.
 *          For devices with a single processor, this method always succeeds for CP_APPLICATION, but does nothing.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     The emulator must not be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   coprocessor                         The targeted coprocessor.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_connect_to_device() function has already been
 *                                              called.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  INVALID_PARAMETER                   The coprocessor is not one belonging to a coprocessor of the device.
 *                                              The coprocessor is not a valid coprocessor.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_select_coprocessor_inst(nrfjprog_inst_t instance, coprocessor_t coprocessor);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_select_coprocessor(coprocessor_t coprocessor);

/**
 * @brief   Selects a new family
 *
 * @details For further details, see the device family header file.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_select_family_inst(nrfjprog_inst_t instance, device_family_t family);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_select_family(device_family_t family);

/**
 * @brief   Recovers the device.
 *
 * @details This operation attempts to recover the device and leave it as it was when it left Nordic factory. It will attempt to
 *          erase all user available flash and disable any protection. It will then connect to the device and halt the CPU before issuing a system reset.
 *
 *          On some devices the APPROTECT configuration in UICR is optionally configured to open. To skip this and leave the UICR registers as if
 *          they were erased, set see the target.approtect.write_uicr_approtect configuration property using NRFJPROG_configure().
 *
 *          On devices that support erase protection if no readback protection is enabled and erase protection is enabled, this function will disable
 *          erase protection.
 *
 *          Note that this operation may take up to 30 s if the device was readback protected.
 *
 *          For nRF51 series devices this method connects to the device and manually erases code, UICR, and RAM.
 *          For nRF52 series devices this method executes a CTRL-AP ERASEALL to clear AP protection.
 *          For nRF53 series devices this method executes a CTRL-AP ERASEALL to clear AP protection.
 *          For nRF91 series devices this method executes a CTRL-AP ERASEALL to clear AP protection.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    All device RAM will be powered. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 * @post    The device code and UICR flash will be erased.
 * @post    If the device was readback protected, the device RAM will be erased.
 * @post    If the device was readback protected, the device will no longer be readback protected.
 * @post    The reset reason register will be cleared.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  RECOVER_FAILED                      Recover failed for any reason, including that the wrong family was used in the NRFJPROG_open_dll() function.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_recover_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_recover(void);

/**
 * @brief   Checks if the connected emulator has an established connection with an nRF device.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_emu_connected_to_device          Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The is_emu_connected_to_device pointer is NULL.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_connected_to_device_inst(nrfjprog_inst_t instance,
                                                                    bool * is_emu_connected_to_device);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_connected_to_device(bool * is_emu_connected_to_device);

/**
 * @brief   Connects to the nRF device.
 *
 * @details This function connects the emulator to the nRF device, powering the debug resources and entering debug interface mode.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 * @pre     The emulator must not be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The trace and debug resources will be switched on. To switch off the debug resources, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_connect_to_device() function has already been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  LOW_VOLTAGE                         Low voltage was detected at the target device.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_connect_to_device_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_connect_to_device(void);

/**
 * @brief   Disconnects from the nRF device.
 *
 * @details Disconnects the emulator from the nRF device, by executing the exit sequence. If the emulator was not connected to the device, no operation is performed.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will not be in debug interface mode. To enter debug interface mode, see NRFJPROG_connect_to_device() function.
 * @post    The emulator will be disconnected from the device. To connect to the device, see NRFJPROG_connect_to_device() function.
 * @post    The trace and debug resources will be switched off. To switch on the debug resources, see NRFJPROG_connect_to_device() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  EMULATOR_NOT_CONNECTED              The emulator with which a connection was established is no longer connected to the PC.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_disconnect_from_device_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_disconnect_from_device(void);

/**
 * @brief   Protects the device against read or debug.
 *
 * @details Protects the device against read or debug by enabling access port protection or region protection following the procedure described in the devices product specification.
 *          The function will reset with a debug reset after execution. Setting protection level NONE will do nothing and return SUCCESS.
 *          nRF51 devices support the protection levels "NONE", "REGION0", and "BOTH".
 *          nRF52 devices support the protection levels "NONE" and "ALL".
 *          nRF53 devices support the protection levels "NONE" "SECURE", and "ALL".
 *          nRF91 devices support the protection levels "NONE" "SECURE", and "ALL".
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     In devices with access port protection, access port protection must be disabled to use this method. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_hard_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    In devices with access port protection, the device debug access port will be disconnected or disabled. To reconnect or re-enablethe access port, see function NRFJPROG_recover().
 * @post    In devices with access port protection, the device CPU will be running. It is not possible to halt the device.
 * @post    In nRF51 devices, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    In nRF51 devices, the device CPU will be  will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   desired_protection                  Desired protection level of readback protection.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The desired_protection is not supported by the family.
 *                                              The desired_protection cannot be encoded in
 *                                              readback_protection_status_t.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_readback_protect_inst(nrfjprog_inst_t instance,
                                                              readback_protection_status_t desired_protection);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_readback_protect(readback_protection_status_t desired_protection);

/**
 * @brief   Returns the status of the readback protection.
 *
 * @details Returns the status of the readback protection.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    In nRF51 devices, the emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    In nRF51 devices, the device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   status                              Pointer for storing of readback status.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The status pointer is NULL.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_readback_status_inst(nrfjprog_inst_t instance,
                                                             readback_protection_status_t * status);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_readback_status(readback_protection_status_t * status);

/**
 * @brief   Returns the status of the erase protection.
 *
 * @details Returns the status of the erase protection if supported.
 *          If status is true, erase protection is enabled, and it may not be possible to erase the device.
 *          If the device does not support erase protection this method
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a
 *          connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface
 *          mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   status                              Pointer for storing of eraseprotect status.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or
 *                                              NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  INVALID_PARAMETER                   The status pointer is NULL.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_eraseprotect_enabled_inst(nrfjprog_inst_t instance, bool * status);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_eraseprotect_enabled(bool * status);

/**
 * @brief   Enable erase protection
 *
 * @details Protects the device against reuse by enabling ERASEPROTECT and resetting. The function will reset
 *          with a debug reset after execution. To disable erase protection, see the product specification of your
 *          device. Note that disabling erase protection may require fw that is able to disable the protection to
 *          already be programmed. NRFJPROG_recover() may be able to disable erase protection if APPROTECT and
 *          SECUREAPPROTECT is not enabled.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a
 *          connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port
 *          protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface
 *          mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The device CPU will be running.
 * @post    ERASEPROTECT will be enabled, and functions NRFJPROG_erase_all(), and
 *          NRFJPROG_page_erase() are not available.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or
 *                                              NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  NVMC_ERROR                          Flash operation failed.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enable_eraseprotect_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_enable_eraseprotect();

/**
 * @brief   Returns the region 0 size and source of protection if any for nRF51 devices.
 *
 * @details Returns the region 0 size and source of protection if any.
 *          For devices other than nRF51, this method sets size and source to 0, and returns SUCCESS.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a
 *          connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface
 *          mode, see NRFJPROG_pin_reset() function.
 * @post    The emulator will be connected to the device. To disconnect from the
 *          device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device()
 *          functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see
 *          NRFJPROG_pin_reset(), NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   size                                Pointer for storing of region 0 protection size.
 * @param   source                              Pointer for storing of region 0 protection source.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or
 *                                              NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The size pointer is NULL.
 *                                              The source pointer is NULL.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_region_0_size_and_source_inst(nrfjprog_inst_t instance,
                                                                           uint32_t * size,
                                                                           region_0_source_t * source);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_region_0_size_and_source(uint32_t * size, region_0_source_t * source);

/**
 * @brief   Executes a soft reset using the CTRL-AP.
 *
 * @details Executes a soft reset by the use of the CTRL-AP RESET register.
 *          nRF51 devices do not have a ctrl-ap, so this method is not available and will return
 *          INVALID_DEVICE_FOR_OPERATION.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The emulator will be disconnected from the device. To connect to the device, see NRFJPROG_connect_to_device() function.
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The device CPU will be running. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_debug_reset_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_debug_reset(void);

/**
 * @brief   Executes a system reset request.
 *
 * @details Executes a cortex-M standard sys reset request by the use of SCB.AIRCR register.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_hard_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_hard_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_sys_reset_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_sys_reset(void);

/**
 * @brief   Executes a pin reset.
 *
 * @details Executes a pin reset by using the method for triggering a pin reset specified in the product specification.
 *          For devices with a physical reset pin a pin reset is triggered by lowering the nReset SWD pin to GND for 20 ms.
 *
 *          For nRF51, This method executes a pin reset by manipulating SWDIO and SWCLK pins like described in the product specification.
 *          For nRF52, in order for the function execution to have the desired effect, the pin reset must be enabled in UICR.PSELRESET[] registers.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @during  The emulator mode is changed to JTAG. If the execution fails, the emulator might be left in JTAG mode. If the execution fails, execute NRFJPROG_close_dll() and start anew.
 *
 * @post    The device will not be in debug interface mode. To enter debug interface mode, see NRFJPROG_connect_to_device() function.
 * @post    The emulator will be disconnected from the device. To connect to the device, see NRFJPROG_connect_to_device() function.
 * @post    The trace and debug resources will be switched off. To switch on the debug resources, see NRFJPROG_connect_to_device() function.
 * @post    The device CPU will be running. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_pin_reset_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_pin_reset(void);

/**
 * @brief   Executes a hard reset.
 *
 * @details Executes a hard reset that resets the debug system.
 *          On devices without a dedicated hard reset mechanism a hard reset is triggered by turning on the watchdog
 *          timer with a 1 ms timeout and disconnecting from the device before the 1 ms has passed. Devices with
 *          hardened AP protect will be protected after this operation.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will not be in debug interface mode. To enter debug interface mode, see NRFJPROG_connect_to_device() function.
 * @post    The emulator will be disconnected from the device. To connect to the device, see NRFJPROG_connect_to_device() function.
 * @post    The trace and debug resources will be switched off. To switch on the debug resources, see NRFJPROG_connect_to_device() function.
 * @post    The device CPU will be running. To halt the device CPU, see NRFJPROG_halt() function.
 * @post    In devices with hardened AP protect the temporary unlock caused by a CTRL-AP ERASEALL will be released, and the device may be protected.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_hard_reset_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_hard_reset(void);

/**
 * @brief   Disables run-time code memory protection mechanisms.
 *
 * @details Disables BPROT by writing into register BPROT.DISABLEINDEBUG, or ACL, SPU, and MPU protection by issuing a system reset with vector catch to prevent the protection from being enabled after reset.
 *          The method then halts the target device and performs a system reset to clear any block protection settings.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @during  A system reset will be executed.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    If UICR->APPROTECT was written, readback protection may be enabled. If so, the function will fail with NOT_AVAILABLE_BECAUSE_PROTECTION.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection, or the operation triggered readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_disable_bprot_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_disable_bprot(void);

/**
 * @brief   Detects if  run-time code memory protection mechanisms are enabled.
 *
 * @details Checks target device BPROT, ACL, SPU and MPU configuration, and returns bprot_enabled = true if the address range address_start to address_start + length overlaps a region with write restrictions.
 *          If the range address_start -> address_start + length falls outside code flash, protection is assumed
 *          disabled for the part that falls outside flash memory.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_hard_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_hard_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   bprot_enabled                       Pointer to a boolean value to store the query result in.
 * @param   address_start                       Query address range start.
 * @param   length                              Query address range length.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The bprot_enabled parameter is a NULL pointer.
 *                                              The length parameter is 0.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_bprot_enabled_inst(nrfjprog_inst_t instance,
                                                              bool * bprot_enabled,
                                                              uint32_t address_start,
                                                              uint32_t length);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_bprot_enabled(bool * bprot_enabled, uint32_t address_start, uint32_t length);

/**
 * @brief   Erases all code and UICR flash.
 *
 * @details Erases all code and UICR flash regions using the device NVM controller. Note that erase_all() will not disable BPROT if enabled while in debug
 *          interface mode. See NRFJPROG_disable_bprot() funtion to disable BPROT while in debug interface mode.
 *
 *          For nRF51:  If the device is programmed at the Nordic factory with a SoftDevice, only Region 1 and UICR will
 *                          be erased.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  NOT_AVAILABLE_BECAUSE_MPU_CONFIG    The operation is not available due to the MPU configuration. The
 *                                              operation is not available due to the presence of Pre-Programmed Factory
 *                                              Code (PPFC).
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_erase_all_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_erase_all(void);

/**
 * @brief   Erases a page of code flash.
 *
 * @details Erases a page of code flash containing the addr. Note that NRFJPROG_erase_page() will not disable BPROT if enabled while in debug
 *          interface mode. See NRFJPROG_disable_bprot() funtion to disable BPROT while in debug interface mode.
 *
 *          For nRF51:  Only available if the page addr is not configured in region 0, see
 *                          NRFJPROG_read_region_0_size_and_source() function.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address of the code flash page to erase.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_BPROT         The page is not erasable because it is configured as write protected a run-tim code protection module. Call NRFJPROG_disable_bprot() to allow the page to be erased.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback or region protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_erase_page_inst(nrfjprog_inst_t instance, uint32_t addr);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_erase_page(uint32_t addr);

/**
 * @brief   Erases UICR info page.
 *
 * @details Erases UICR info page.
 *
 *          For nRF51:  Only available if the device is not programmed at Nordic factory with a SoftDevice.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_erase_uicr_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_erase_uicr(void);

/**
 * @brief   Writes one uint32_t data into the given address.
 *
 * @details Writes one uint32_t data to the given addr without verifying that the address is accessible or even exists.
 *          If nvmc_control is true, it will control the NVMC in order to write into flash. Writes need to be 32-bit
 *          aligned. Note that if the target address is in unpowered RAM, the operation will fail.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to write to.
 * @param   data                                Value to write.
 * @param   nvmc_control                        If the target address needs NVMC control.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The addr parameter is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 *                                              The address to write is in unpowered RAM.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write_u32_inst(nrfjprog_inst_t instance,
                                                       uint32_t addr,
                                                       uint32_t data,
                                                       bool nvmc_control);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write_u32(uint32_t addr, uint32_t data, bool nvmc_control);

/**
 * @brief   Reads one uint32_t from the given address.
 *
 * @details Reads one uint32_t data from the given addr without verifying that the address is accessible or even exists.
 *          Reads need to be 32-bit aligned. Note that if the target address is in unpowered RAM, the operation will
 *          fail.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to read from.
 * @param   data                                Pointer of the location to store the value.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The addr parameter is not 32-bit aligned.
 *                                              The data parameter is null.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 *                                              The address to read is in unpowered RAM.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_u32_inst(nrfjprog_inst_t instance, uint32_t addr, uint32_t * data);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_u32(uint32_t addr, uint32_t * data);

/**
 * @brief   Writes data from the array into the device starting at the given address.
 *
 * @details Writes data_len bytes from the data array into the device starting at the given addr without verifying that the address is accessible
 *          or even exists. If nvmc_control is true, it will control the NVMC in order to write into flash. Writing addr
 *          does not need to be 32-bit aligned. Writing length data_len does not need to be a multiple of four, but
 *          please note that the number of writes to a flash word between erases is limited (see nWRITE parameter in the
 *          Product Specification for your device) so data_len less than four bytes should be avoided if possible. Note
 *          that if the target address is in unpowered RAM, the operation will fail.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Start address of the region to write to.
 * @param   data                                Pointer to an array with the data to write.
 * @param   data_len                            Length of the data array.
 * @param   nvmc_control                        If the target address needs NVMC control.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The data_len parameter is 0.
 *                                              The data parameter is NULL.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 *                                              The address to write is in unpowered RAM.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 * @retval  NVMC_ERROR                          Flash operation failed.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write_inst(nrfjprog_inst_t instance,
                                                   uint32_t addr,
                                                   const uint8_t * data,
                                                   uint32_t data_len,
                                                   bool nvmc_control);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write(uint32_t addr,
                                              const uint8_t * data,
                                              uint32_t data_len,
                                              bool nvmc_control);

/**
 * @brief   Reads data_len bytes from the device starting at the given address.
 *
 * @details Reads data_len bytes from the device starting at the given addr without verifying that the addresses are accessible or even
 *          exist. Note that if the target address is in unpowered RAM, the operation will fail.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to read from.
 * @param   data                                Pointer of the location to store the value.
 * @param   data_len                            Number of bytes to read.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_len parameter is 0.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_inst(nrfjprog_inst_t instance,
                                                  uint32_t addr,
                                                  uint8_t * data,
                                                  uint32_t data_len);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read(uint32_t addr, uint8_t * data, uint32_t data_len);


/**
 * @brief   Checks if the device CPU is halted.
 *
 * @details Checks if the device CPU is halted.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_device_halted                    Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  INVALID_PARAMETER                   The is_device_halted pointer is NULL.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_halted_inst(nrfjprog_inst_t instance, bool * is_device_halted);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_halted(bool * is_device_halted);

/**
 * @brief   Halts the device CPU.
 *
 * @details Halts the device CPU.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_halt_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_halt(void);

/**
 * @brief   Starts the device CPU with the given pc and sp.
 *
 * @details Starts the device CPU with the given pc and sp.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @during  The device CPU is halted. If the function execution fails, the device CPU might be left halted.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be running. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   pc                                  Program Counter to start running from.
 * @param   sp                                  Stack Pointer to use when running.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_run_inst(nrfjprog_inst_t instance, uint32_t pc, uint32_t sp);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_run(uint32_t pc, uint32_t sp);

/**
 * @brief   Starts the device CPU.
 *
 * @details Starts the device CPU.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be running. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_go_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_go(void);

/**
 * @brief   Runs the device CPU for one instruction.
 *
 * @details Runs the device CPU for one instruction and halts.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 * @pre     The device CPU must be halted. To halt the device CPU, see NRFJPROG_halt() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The device is not halted.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_step_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_step(void);

/**
 * @brief   Reads the number of RAM sections in the device.
 *
 * @details Reads the number of RAM sections in device and returns the result in input pointer.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ram_sections_count                  Pointer of the location to store the number of RAM section in the device.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The ram_sections_count pointer is NULL.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_ram_sections_count_inst(nrfjprog_inst_t instance,
                                                                     uint32_t * ram_sections_count);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_ram_sections_count(uint32_t * ram_sections_count);

/**
 * @brief   Reads the size in bytes of the RAM sections in device.
 *
 * @details Reads the size of the RAM sections in device and returns the result in the input array in bytes. If input array size is larger
 *          than the number of RAM sections, the rest of the array will be left untouched.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ram_sections_size                   Array to store the results.
 * @param   ram_sections_size_len               Size of ram_sections_size array.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The ram_sections_size pointer is NULL.
 *                                              The ram_sections_size_len is less than the number of RAM sections in
 *                                              device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_ram_sections_size_inst(nrfjprog_inst_t instance,
                                                                    uint32_t * ram_sections_size,
                                                                    uint32_t ram_sections_size_len);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_ram_sections_size(uint32_t * ram_sections_size,
                                                               uint32_t ram_sections_size_len);

/**
 * @brief   Reads the RAM sections power status.
 *
 * @details Reads the RAM sections power status and returns the result, on or off for each section, in ram_sections_power_status array.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ram_sections_power_status           Array to store the results.
 * @param   ram_sections_power_status_len       Size of ram_sections_power_status array.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The ram_sections_power_status pointer is NULL.
 *                                              The ram_sections_power_status_len is less than the number of RAM
 *                                              sections in device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t
NRFJPROG_read_ram_sections_power_status_inst(nrfjprog_inst_t instance,
                                             ram_section_power_status_t * ram_sections_power_status,
                                             uint32_t ram_sections_power_status_len);
NRFJPROG_API nrfjprogdll_err_t
NRFJPROG_read_ram_sections_power_status(ram_section_power_status_t * ram_sections_power_status,
                                        uint32_t ram_sections_power_status_len);

/**
 * @brief   Powers up all RAM sections of the device.
 *
 * @details Powers up all the RAM of the device in ON state. Will not affect the RAM retention.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_power_ram_all_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_power_ram_all(void);

/**
 * @brief   Powers down a RAM section of the device.
 *
 * @details Powers down a RAM section of the device in ON state. Will not affect the RAM retention.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   section_index                       Section of RAM to power down.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The section section_index does not exist in the device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_unpower_ram_section_inst(nrfjprog_inst_t instance, uint32_t section_index);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_unpower_ram_section(uint32_t section_index);

/**
 * @brief   Read memory descriptors.
 *
 * @details The 'memories' array is populated with descriptions of each of the memories that are available for the currently selected coprocessor, and the number of memories written is stored in the 'num_memories_available' parameter.
 *          If the function is called with a NULL array and zero length, the total number of available memories is
 *          stored in 'num_memories_available'. See header DllCommonDefinitions.h for type definitions. If the
 *          function returns an UNKNOWN_DEVICE error, the device version was not recognized and default values based
 *          on family have been generated.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   memories                            Array for storing the memory descriptors. Can be NULL of 'memories_len' is zero.
 * @param   memories_len                        Length of the 'memories' array, and the max number of memory descriptors that will be written.
 * @param   num_memories_available              Pointer for storing the number of memory descriptions available. Can be NULL.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   'memories' parameter is NULL while 'memories_len' is non-zero.
 *                                              'num_memories_available' is NULL.
 * @retval  INVALID_OPERATION                   The NRFJPROG_open_dll() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  TIME_OUT                            Timed out waiting for the device debug systems to power up.
 * @retval  UNKNOWN_DEVICE                      Device version was unrecognized.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    Memory descriptions unavailable due to device readback protection.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_memory_descriptors_inst(nrfjprog_inst_t instance,
                                                                     memory_description_t memories[],
                                                                     uint32_t memories_len,
                                                                     uint32_t * num_memories_available);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_memory_descriptors(memory_description_t memories[],
                                                                uint32_t memories_len,
                                                                uint32_t * num_memories_available);

/**
 * @brief   Read the page sizes of a memory.
 *
 * @details Read the page sizes of the memory referenced by the provided memory_description struct, and store the number of page repetitions written in 'num_page_reps_available'.
 *          If the function is called with a NULL array and zero length, the total number of available page
 *          repetitions is stored in 'num_page_reps_available'. See header DllCommonDefinitions.h for type
 *          definitions.
 *
 *          Each element of the 'page_reps' array contains a page size, and the number of times the page is
 *          repeated. For instance, if a memory has homogenous page size, one element will be written. The address
 *          of a page can be found by adding the memory start address to the sum of pages before it. Pages are
 *          ordered the way they appear in the 'page_reps' array, where the first page repetitions element describes
 *          the first pages of a memory.
 *
 *          Note that the memories returned by NRFJPROG_read_memory_descriptors() changes based on selected
 *          coprocessor and read device version, potentially rendering the memory reference 'memory_description'
 *          invalid.
 *
 * @pre     A memory device struct has been obtained with the NRFJPROG_read_memory_descriptors() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   memory_description                  Pointer to a memory description struct that has been obtained with the NRFJPROG_read_memory_descriptors() function.
 * @param   page_reps                           Array for storing page repetitions. Can be NULL if 'page_reps_len' is zero.
 * @param   page_reps_len                       Length of the 'page_reps' array, and the max number of page repetitions that will be written.
 * @param   num_page_reps_available             Pointer for storing the number of page repetitions available.
 *
 * @retval  SUCCESS
 * @retval  INVALID_PARAMETER                   'memory_description' is NULL.
 *                                              'num_page_reps_available' is NULL.
 *                                              'page_reps' parameter is NULL while 'page_reps_len' is non-zero.
 *                                              The memory referenced by 'memory_description' was not found.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_page_sizes_inst(nrfjprog_inst_t instance,
                                                             memory_description_t * memory_description,
                                                             page_repetitions_t page_reps[],
                                                             uint32_t page_reps_len,
                                                             uint32_t * num_page_reps_available);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_page_sizes(memory_description_t * memory_description,
                                                        page_repetitions_t page_reps[],
                                                        uint32_t page_reps_len,
                                                        uint32_t * num_page_reps_available);

/**
 * @brief   Reads a CPU register.
 *
 * @details Reads a CPU register. Valid registers are R0-R15, XPSR, MSP and PSP.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   register_name                       Register name to read. See cpu_registers_t definition for valid values.
 * @param   register_value                      Pointer of the location to store the read register.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The register_data parameter is null.
                                                The register_name parameter is not a valid register.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_cpu_register_inst(nrfjprog_inst_t instance,
                                                               int register_name,
                                                               uint32_t * register_value);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_cpu_register(int register_name, uint32_t * register_value);

/**
 * @brief   Reads the device architecture of the target coprocessor.
 *
 * @details Reads the device architecture of the target coprocessor, and writes the value to the arch parameter.
 *          See the cpu_architecture_t enum in DllCommonDefinitions.h header for possible arch values.
 *          To select a target coprocessor see function NRFJPROG_select_coprocessor.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_cpu_architecture_inst(nrfjprog_inst_t instance, cpu_architecture_t * arch);

/**
 * @brief   Writes a CPU register.
 *
 * @details Writes a CPU register. Valid registers are R0-R15, XPSR, MSP and PSP.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   register_name                       CPU register to write. See cpu_registers_t definition for valid values.
 * @param   register_value                      Value to write into the CPU register.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The register_name parameter is not a valid register.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write_cpu_register_inst(nrfjprog_inst_t instance,
                                                                int register_name,
                                                                uint32_t register_value);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write_cpu_register(int register_name, uint32_t register_value);


/**
 * @brief   Reads the version of the device connected to the emulator.
 *
 * @details Reads the version of the device connected to the emulator. If the device family does not match the configured family the davice version parameter will be set to UNKNOWN and WRONG_FAMILY_FOR_DEVICE error returned.
 *          If the connected device is a newer revision of a known device, but this revision is not known at compile
 *          time of this dll, the appropriate device _FUTURE will be returned instead. See DllCommonDefinitions.h header
 *          file for the possible values of device_version_t.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   version                             Pointer of the location to store the device type.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The version parameter is null.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_device_version_inst(nrfjprog_inst_t instance, device_version_t * version);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_device_version(device_version_t * version);

/**
 * @brief   Reads the version, name, memory and revision descriptors of the device connected to the emulator.
 *
 * @details Reads the version, name, memory and revision descriptors of the device connected to the emulator. If the device is not an nRF50 device version, name, memory and revision parameters will be set to UNKNOWN, UNKNOWN_NAME, UNKNOWN_MEM and UNKNOWN_REV and WRONG_FAMILY_FOR_DEVICE error is returned.
 *
 *          If the connected device is a newer revision of a known device, but this revision is not known at compile
 *          time of this dll, the appropriate device FUTURE enum will be returned instead. See DllCommonDefinitions.h
 *          header file for the possible values of the device info enums. If a NULL-pointer is passed as an argument,
 *          that parameter is not updated.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     Access port protection must be disabled. To disable access port protection, see NRFJPROG_recover() function.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   version                             Pointer of the location to store the device version.
 * @param   name                                Pointer of the location to store the device name.
 * @param   memory                              Pointer of the location to store the device memory.
 * @param   revision                            Pointer of the location to store the device revision.
 *
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_device_info_inst(nrfjprog_inst_t instance,
                                                              device_version_t * version,
                                                              device_name_t * name,
                                                              device_memory_t * memory,
                                                              device_revision_t * revision);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_device_info(device_version_t * version,
                                                         device_name_t * name,
                                                         device_memory_t * memory,
                                                         device_revision_t * revision);

/**
 * @brief   Reads the family of the device connected to the emulator.
 *
 * @details Reads the family of the device connected to the emulator by the use of NRFJPROG_read_access_port_register() function.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see the appropriate family header file for functions that can take the device out of debug interface mode.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   family                              Pointer of the location to store the device family.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The family parameter is null.
 * @retval  UNKNOWN_DEVICE                      Family identification failed.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_device_family_inst(nrfjprog_inst_t instance, device_family_t * family);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_device_family(device_family_t * family);

/**
 * @brief   Reads a debug port register.
 *
 * @details Reads into data pointer a debug port register.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   reg_addr                            Register address to read, either in debug port or access port.
 * @param   data                                Pointer of the location to store the value read.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The register address is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_debug_port_register_inst(nrfjprog_inst_t instance,
                                                                      uint8_t reg_addr,
                                                                      uint32_t * data);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_debug_port_register(uint8_t reg_addr, uint32_t * data);

/**
 * @brief   Writes a debug port register.
 *
 * @details Writes data parameter into a debug port register.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   reg_addr                            Register address to write, either in debug port or access port.
 * @param   data                                Data to write into the register.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The register address is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write_debug_port_register_inst(nrfjprog_inst_t instance,
                                                                       uint8_t reg_addr,
                                                                       uint32_t data);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write_debug_port_register(uint8_t reg_addr, uint32_t data);

/**
 * @brief   Reads a debugger access port register.
 *
 * @details Reads into data pointer a debugger access port register.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ap_index                            Access port index for read if ap_access.
 * @param   reg_addr                            Register address to read, either in debug port or access port.
 * @param   data                                Pointer of the location to store the value read.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The register address is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_access_port_register_inst(nrfjprog_inst_t instance,
                                                                       uint8_t ap_index,
                                                                       uint8_t reg_addr,
                                                                       uint32_t * data);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_access_port_register(uint8_t ap_index, uint8_t reg_addr, uint32_t * data);

/**
 * @brief   Writes a debugger access port register.
 *
 * @details Writes data parameter into a debugger access port register.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   ap_index                            Access port index for write if ap_access.
 * @param   reg_addr                            Register address to write, either in debug port or access port.
 * @param   data                                Data to write into the register.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The register address is not 32-bit aligned.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write_access_port_register_inst(nrfjprog_inst_t instance,
                                                                        uint8_t ap_index,
                                                                        uint8_t reg_addr,
                                                                        uint32_t data);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_write_access_port_register(uint8_t ap_index, uint8_t reg_addr, uint32_t data);

/**
 * @brief   Checks if the RTT is started.
 *
 * @details Checks if the NRFJPROG_rtt_start() function has been called since the last call to NRFJPROG_rtt_stop() or since the loading of this dll.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   started                             Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The started parameter is null.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_rtt_started_inst(nrfjprog_inst_t instance, bool * started);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_rtt_started(bool * started);

/**
 * @brief   Indicates to the dll the location of the RTT control block in the device memory.
 *
 * @details Indicates to the dll the location of the RTT control block in the device memory, to accelerate the discovery of the RTT control block.
 *          If the RTT control block is not located at the address given, NRFJPROG_rtt_start() will never locate the RTT
 *          control block. If address is NRFJPROG_INVALID_ADDRESS any previously set address is cleared, and the dll
 *          reverts to auto-detection.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   address                             RTT control block address in device memory.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_rtt_start() function has been called.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_set_control_block_address_inst(nrfjprog_inst_t instance, uint32_t address);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_set_control_block_address(uint32_t address);

/**
 * @brief   Starts RTT.
 *
 * @details Starts RTT, initiating the search for the RTT control block and preparing the DLL for RTT operations. If NRFJPROG_rtt_set_control_block_address() has been called,
 *          the address provided will be used to locate the control block. Since NRFJPROG_rtt_start() function is not
 *          blocking, to find out if NRFJPROG_rtt_start() has located the RTT control block and is ready to receive
 *          commands, see NRFJPROG_rtt_is_control_block_found() function. After the execution of NRFJPROG_rtt_start(),
 *          please ensure that NRFJPROG_rtt_stop() function will be executed.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     RTT must not be started. To stop RTT, see NRFJPROG_rtt_stop() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     The RTT control block must be present in RAM. Normally it is the firmware execution that writes the control block into RAM. To run the firmware, see NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_rtt_start() function has already been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected does not match the configured family.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_start_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_start(void);

/**
 * @brief   Setup RTT channel for asynchronous operation.
 *
 * @details Sets RTT channel up to be used asynchronously.
 *          After the function is called successfully, NRFJPROG_rtt_async_write() can be used to write data
 *          asynchronously, when a write is done, write_callback is called with the amount of bytes written, and a
 *          result code. For up-channels, this method starts a separate thread feeding rtt data asynchronously to the
 *          callback function. Up to max_read_data_len bytes of data will be read at a time. Each channel gets a
 *          separate thread and can receive data asynchronously.
 *          This method can only be called once per channel.
 *
 *          The target device may not neccessarily provide both read and write (up / down) capabilities on all channels.
 *          If only one of the up / down RTT capabilities are enabled, the available capability is set up and the
 *          function successfully returns.
 *          If neither up nor down RTT capabilities are available the channel is not open, and the method fails with
 *          INVALID_PARAMETER.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 * @pre     RTT must be started. To start RTT, see NRFJPROG_rtt_start() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   channel_index                       RTT channel index to set up for asynchronous operation.
 * @param   max_read_data_len                   Maximum number of bytes to read per callback.
 * @param   read_callback                       This method will be called on data reception.
 * @param   write_callback                      This method will be called on successful data transmission.
 * @param   callback_param                      Reference data to be passed back together with callbacks.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 *                                              RTT control block has not been found yet. See
 *                                              NRFJPROG_rtt_is_control_block_found().
 * @retval  INVALID_PARAMETER                   The callback parameter is null.
 *                                              There is no channel in the device with the given channel_index index.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_async_start_inst(nrfjprog_inst_t instance,
                                                             uint32_t channel_index,
                                                             uint32_t max_read_data_len,
                                                             rtt_read_callback read_callback,
                                                             rtt_write_callback write_callback,
                                                             void * callback_param);

/**
 * @brief   Flush asynchronous RTT write buffer.
 *
 * @details Waits until all messages in RTT write queue have been written to target.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 * @pre     RTT must be started. To start RTT, see NRFJPROG_rtt_start() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_async_flush_inst(nrfjprog_inst_t instance);

/**
 * @brief   Checks if RTT control block has been found.
 *
 * @details Checks if RTT control block has been found. Can be used to make sure RTT is ready to be used before calling NRFJPROG_rtt_read() and NRFJPROG_rtt_write() functions.
 *
 *          If RTT is not started, the method returns successfully with is_control_block_found = 0.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_control_block_found              Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The is_found parameter is null.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_is_control_block_found_inst(nrfjprog_inst_t instance,
                                                                        bool * is_control_block_found);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_is_control_block_found(bool * is_control_block_found);

/**
 * @brief   Checks if RTT control block has been found, and what address it was found at.
 *
 * @details Checks if RTT control block has been found, and what address it was found at.
 *          Can be used to make sure RTT is ready to be used before calling NRFJPROG_rtt_read() and NRFJPROG_rtt_write()
 *          functions. Can also be used to improve startup speed of subsequent rtt sessions by caching the rtt control
 *          block address and setting it with NRFJPROG_rtt_set_control_block_address.
 *
 *          If RTT is not started, the method returns successfully with is_found = 0.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   is_found                            Pointer of the location of a variable that will indicate if the control block has been detected yet.
 * @param   address                             Pointer of the location of a variable that will indicate the address of the control block if found. If is_found is false,
 *                                              the content of this variable is undefined.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The is_found parameter is null.
 *                                              The address parameter is null.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_get_control_block_info_inst(nrfjprog_inst_t instance,
                                                                        bool * is_found,
                                                                        uint32_t * address);

/**
 * @brief   Stops RTT.
 *
 * @details Stops RTT and clears the RTT Control Buffer.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     RTT must be started. To start RTT, see NRFJPROG_rtt_start() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @post    The RTT control block in RAM will have been erased. Therefore, before another NRFJPROG_rtt_start() can be issued, the control block must be rewritten to RAM. See NRFJPROG_rtt_start() function for details.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_stop_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_stop(void);

/**
 * @brief   Stop asynchronous handling of RTT channel.
 *
 * @details Cancels asynchronous handling previously started by NRFJPROG_rtt_async_start_inst().
 *          After the function is called.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 * @pre     RTT must be started. To start RTT, see NRFJPROG_rtt_start() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   channel_index                       RTT channel index to stop asynchronous operation for
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 *                                              RTT control block has not been found yet. See
 *                                              NRFJPROG_rtt_is_control_block_found().
 * @retval  INVALID_PARAMETER                   The callback parameter is null.
 *                                              There is no channel in the device with the given channel_index index.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_async_stop_inst(nrfjprog_inst_t instance, uint32_t channel_index);

/**
 * @brief   Reads from an RTT channel.
 *
 * @details Reads up to data_len characters from the up_channel_index RTT channel.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     RTT must be started. To start RTT, see NRFJPROG_rtt_start() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   up_channel_index                    RTT channel index to read from.
 * @param   data                                Pointer of the location to store the value.
 * @param   data_len                            Number of bytes to read.
 * @param   data_read                           Pointer to the location to store the actual number of read characters.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_read parameter is null.
 *                                              There is no channel in the device with the given up_channel_index index.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_read_inst(nrfjprog_inst_t instance,
                                                      uint32_t up_channel_index,
                                                      char * data,
                                                      uint32_t data_len,
                                                      uint32_t * data_read);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_read(uint32_t up_channel_index,
                                                 char * data,
                                                 uint32_t data_len,
                                                 uint32_t * data_read);

/**
 * @brief   Writes to an RTT channel.
 *
 * @details Writes the given data_len length data string into the down_channel_index RTT channel.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     RTT must be started. To start RTT, see NRFJPROG_rtt_start() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   down_channel_index                  RTT channel index to write to.
 * @param   data                                Pointer to a string with the string to write.
 * @param   data_len                            Length of the string.
 * @param   data_written                        Pointer to the location to store the actual number of written characters.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_read parameter is null.
 *                                              There is no channel in the device with the given channel index.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_write_inst(nrfjprog_inst_t instance,
                                                       uint32_t down_channel_index,
                                                       const char * data,
                                                       uint32_t data_len,
                                                       uint32_t * data_written);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_write(uint32_t down_channel_index,
                                                  const char * data,
                                                  uint32_t data_len,
                                                  uint32_t * data_written);

/**
 * @brief   Sends data to the specified RTT channel asynchronously.
 *
 * @details The message is passed to a separate thread that performs the write. An internal buffer allows multiple messages to be queued up without waiting.
 *          When the message is written, the write_callback parameter from NRFJPROG_rtt_async_start will be called.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 * @pre     RTT must be started. To start RTT, see NRFJPROG_rtt_start() function.
 * @pre     The RTT channel must be enabled for asynchronous operation, see NRFJPROG_rtt_async_start() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   down_channel_index                  RTT channel index to write to.
 * @param   data                                Pointer to a buffer with the data to write.
 * @param   data_len                            Length of the buffer.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 *                                              The NRFJPROG_rtt_async_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_read parameter is 0.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_async_write_inst(nrfjprog_inst_t instance,
                                                             uint32_t down_channel_index,
                                                             const char * data,
                                                             uint32_t data_len);

/**
 * @brief   Gets the number of RTT channels.
 *
 * @details Gets the number of down and up channels in the device.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     RTT must be started. To start RTT, see NRFJPROG_rtt_start() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   down_channel_number                 Pointer to store the number of down channels.
 * @param   up_channel_number                   Pointer to store the number of up channels.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The down_channel_number parameter is null.
 *                                              The up_channel_number parameter is null.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_read_channel_count_inst(nrfjprog_inst_t instance,
                                                                    uint32_t * down_channel_number,
                                                                    uint32_t * up_channel_number);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_read_channel_count(uint32_t * down_channel_number,
                                                               uint32_t * up_channel_number);

/**
 * @brief   Reads the info from one RTT channel.
 *
 * @details Reads the info from one RTT channel of index channel_index of direction dir. Reads the rtt channel size into channel_size and name into channel_name pointers.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     RTT must be started. To start RTT, see NRFJPROG_rtt_start() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_rtt_start() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   channel_index                       Channel index to read the info from.
 * @param   dir                                 Channel direction of channel to read the info from.
 * @param   channel_name                        Pointer to store the channel name, must be able to store at least 32 characters.
 * @param   channel_name_len                    Size of channel_name storage size.
 * @param   channel_size                        Pointer to store the channel size.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_rtt_start() function has not been called.
 * @retval  INVALID_PARAMETER                   The channel_name parameter is null.
 *                                              The channel_size parameter is null.
 *                                              The channel_size_len parameter is less than 32.
 *                                              The dir parameter is neither UP_DIRECTION or DOWN_DIRECTION defined in
 *                                                  rtt_direction_t enum in DllCommonDefinitions.h The channel with
 *                                                  index channel_index does not exist in the device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_read_channel_info_inst(nrfjprog_inst_t instance,
                                                                   uint32_t channel_index,
                                                                   rtt_direction_t dir,
                                                                   char * channel_name,
                                                                   uint32_t channel_name_len,
                                                                   uint32_t * channel_size);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_rtt_read_channel_info(uint32_t channel_index,
                                                              rtt_direction_t dir,
                                                              char * channel_name,
                                                              uint32_t channel_name_len,
                                                              uint32_t * channel_size);

/**
 * @brief   Checks if the QSPI peripheral is initialized.
 *
 * @details Checks if the NRFJPROG_qspi_init() function has been called since the last call to NRFJPROG_qspi_uninit() or since the loading of this dll.
 *          The physical state of the QSPI peripheral is also read from the device to check if it has been
 *          reset/uninitialized by an external source. For example, resetting the device will also reset the QSPI
 *          peripheral, and any configuration applied in NRFJPROG_qspi_init() will be lost.
 *
 *          If the device does not have a QSPI peripheral, false is returned.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   initialized                         Pointer of the location to store the result.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The initialized parameter is null.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF53 device.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_qspi_init_inst(nrfjprog_inst_t instance, bool * initialized);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_qspi_init(bool * initialized);

/**
 * @brief   Configures and initializes the QSPI peripheral.
 *
 * @details Initializes the QSPI peripheral, configuring the QSPI peripheral according to init_params. If retain_ram is true, the device RAM used for QSPI operations will be read and stored.
 *          The stored contents will be restored after the execution of NRFJPROG_qspi_uninit() function.
 *          NRFJPROG_qspi_init() function does not initialize the external flash memory device in any way. If the
 *          external flash device requires special configuration for the QSPI configuration given by init_params, it is
 *          up to the user to do so with the help of NRFJPROG_qspi_custom() function. Note that for certain external
 *          flash memory devices some operation modes are not available, so use the available options with care.
 *
 *          NRFJPROG_qspi_init() function currently supports a
 *          limited range of configurations. The only supported memory device at the moment is the MX25R6435F. It will
 *          be extended in the future on request.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    All device RAM will be powered. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   retain_ram                          Save the RAM contents of the region used as buffer for the QSPI operations.
 * @param   init_params                         Pointer to the initialization paramters for QSPI.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              The NRFJPROG_qspi_init() function has already been called.
 * @retval  INVALID_PARAMETER                   The qspi_ini_path parameter is null.
 *                                              The supplied .ini file has syntax errors.
 *                                              One of the QSPI parameters failed verification.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_init_inst(nrfjprog_inst_t instance,
                                                       bool retain_ram,
                                                       const qspi_init_params_t * init_params);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_init(bool retain_ram, const qspi_init_params_t * init_params);

/**
 * @brief   Configures and initializes the QSPI peripheral.
 *
 * @details Initializes the QSPI peripheral, configuring the QSPI peripheral with parameters from the provided ini file. See file QspiDefault.ini for an example.
 *          If retain_ram is true, the device RAM used for QSPI operations will be read and stored.
 *          The stored contents will be restored after the execution of NRFJPROG_qspi_uninit() function.
 *          NRFJPROG_qspi_init() function does not initialize the external flash memory device in any way. If the
 *          external flash device requires special configuration for the QSPI configuration given by init_params, it is
 *          up to the user to do so with the help of NRFJPROG_qspi_custom() function. Note that for certain external
 *          flash memory devices some operation modes are not available, so use the available options with care.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    All device RAM will be powered. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   qspi_ini_path                       Path to the .ini containing initialization parameters for the QSPI memory interface.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   Could not find file referenced by qspi_ini_path.
 *                                              Syntax error encountered while parsing QSPI ini file.
 * @retval  OUT_OF_MEMORY                       Ran out of memory while parsing QSPI ini file.
 * @retval  FILE_OPERATION_FAILED               Failed to read ini file.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_init_ini_inst(nrfjprog_inst_t instance, const char * qspi_ini_path);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_init_ini(const char * qspi_ini_path);

/**
 * @brief   Initializes the QSPI peripheral.
 *
 * @details Initializes the QSPI peripheral, using the settings from the already configured QSPI peripheral.
 *          If retain_ram has been set, the device RAM used for QSPI operations will be read and stored. The stored
 *          contents will be restored after the execution of NRFJPROG_qspi_uninit() function. NRFJPROG_qspi_start()
 *          function does not initialize the external flash memory device in any way. If the external flash device
 *          requires special configuration for the QSPI configuration given by init_params, it is up to the user to do
 *          so with the help of NRFJPROG_qspi_custom() function. Note that for certain external flash memory devices
 *          some operation modes are not available, so use the available options with care.
 *
 *          The QSPI peripheral configuration will remain after uninitializing QSPI, allowing this function to be called
 *          directly after.
 *
 * @pre     The QSPI module must be configured. See functions NRFJPROG_qspi_configure() and NRFJPROG_qspi_configure_ini().
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    All device RAM will be powered. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_OPERATION                   QSPI has not been configured.
 *                                              QSPI is already initialized.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_start_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_start();

/**
 * @brief   Configures the QSPI driver.
 *
 * @details Configure the QSPI peripheral according to init_params, without initializing the QSPI peripheral on the device. See file QspiDefault.ini for an example.
 *          After configuration NRFJPROG_qspi_start() can be called to initialize the QSPI peripheral. See function
 *          NRFJPROG_qspi_init() for QSPI configuration and initialization in one step.
 *
 *          Note that configuring using qspi_init_params_t does not set qspi memory size which is needed for
 *          NRFJPROG_program_file(), NRFJPROG_read_to_file(), NRFJPROG_verify_file() and NRFJPROG_erase_file(). Use
 *          NRFJPROG_qspi_set_size() to configure memory size if needed.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   retain_ram                          Save the RAM contents of the region used as buffer for the QSPI operations.
 * @param   init_params                         Pointer to the initialization paramters for QSPI.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              QSPI is initialized.
 * @retval  INVALID_PARAMETER                   The init_params parameter is null.
 *                                              The init_params->read_mode parameter cannot be encoded in a
 *                                              qspi_read_mode_t. The init_params->write_mode parameter cannot be
 *                                              encoded in a qspi_write_mode_t. The init_params->frequency parameter
 *                                              cannot be encoded in a qspi_frequency_t.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_configure_inst(nrfjprog_inst_t instance,
                                                            bool retain_ram,
                                                            const qspi_init_params_t * init_params);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_configure(bool retain_ram, const qspi_init_params_t * init_params);

/**
 * @brief   Configures the QSPI peripheral.
 *
 * @details Configure the QSPI peripheral with parameters from the provided ini file. See file QspiDefault.ini for an example.
 *          After configuration NRFJPROG_qspi_start() can be called to initialize the QSPI peripheral. See function
 *          NRFJPROG_qspi_init_ini() for QSPI configuration and initialization in one step.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must not be initialized. To uninitialize the QSPI, see NRFJPROG_qspi_uninit() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   qspi_ini_path                       Path to the .ini containing initialization parameters for the QSPI memory interface.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              QSPI is initialized.
 * @retval  INVALID_PARAMETER                   The qspi_ini_path parameter is null.
 *                                              The supplied .ini file has syntax errors.
 *                                              One of the QSPI parameters failed verification.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_configure_ini_inst(nrfjprog_inst_t instance, const char * qspi_ini_path);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_configure_ini(const char * qspi_ini_path);

/**
 * @brief   Uninitializes the QSPI peripheral.
 *
 * @details Uninitializes the QSPI peripheral. If NRFJPROG_qspi_init() function was called with the retain_ram parameter set as true, the RAM contents at the time of NRFJPROG_qspi_init() call will
 *          be restored.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode, see NRFJPROG_pin_reset(), NRFJPROG_disconnect_from_emu() and NRFJPROG_close_dll() functions.
 * @post    The emulator will be connected to the device. To disconnect from the device, see NRFJPROG_disconnect_from_emu(), NRFJPROG_close_dll() and NRFJPROG_disconnect_from_device() functions.
 * @post    If NRFJPROG_qspi_init() function was called with retain_ram parameter set as true, the contents of the RAM used for QSPI operations will be restored.
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 * @retval  WRONG_FAMILY_FOR_DEVICE             The device connected is not an NRF52.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available due to readback protection.
 * @retval  CANNOT_CONNECT                      It is impossible to connect to any nRF device.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_uninit_inst(nrfjprog_inst_t instance);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_uninit(void);

/**
 * @brief   Set QSPI RX delay
 *
 * @details Configures the dll to set the QSPI peripheral rx_delay mechanism to account for CLK round trip issues. If this function is not called, a default value will be used.
 *
 *          This method is ignored for nRF53 devices since rx delay timing is handled in hardware.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must not be initialized.
 *
 * @post    The DLL will be prepared to use rx_delay instead of the built in default value.
 * @post    The RAM memory contents might be altered. To restore the contents of the RAM memory, see NRFJPROG_qspi_uninit() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   rx_delay                            Rx delay to set. See the product specification of your device for possible values.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_qspi_init() function has been called.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_set_rx_delay_inst(nrfjprog_inst_t instance, uint8_t rx_delay);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_set_rx_delay(uint8_t rx_delay);

/**
 * @brief   Set QSPI memory size.
 *
 * @details Set QSPI memory size. This size is used when reading the QSPI memory in NRFJPROG_program_file(), NRFJPROG_read_to_file(), NRFJPROG_verify_file(), and NRFJPROG_erase_file() functions.
 *          Size can also be set by initializing QSPI using an ini file. See functions NRFJPROG_qspi_init_ini() and
 *          NRFJPROG_qspi_configure_ini().
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   qspi_size                           QSPI memory size in bytes.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_set_size_inst(nrfjprog_inst_t instance, uint32_t qspi_size);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_set_size(uint32_t qspi_size);

/**
 * @brief   Get QSPI memory size.
 *
 * @details Get QSPI memory size set by NRFJPROG_qspi_set_size() or NRFJPROG_qspi_init_ini()
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   qspi_size                           Pointer to variable where QSPI memory size in bytes should be written.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The qspi_size parameter is null.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support this operation.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_get_size_inst(nrfjprog_inst_t instance, uint32_t * qspi_size);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_get_size(uint32_t * qspi_size);

/**
 * @brief   Reads from the external QSPI-connected memory.
 *
 * @details Reads data_len characters from addr address of the external QSPI-connected memory. Actual read command address and length sent to the device may be adjusted since the
 *          QSPI peripheral only accepts 4-byte aligned addresses and multiple of 4 lengths. Any extra bytes read from
 *          the external QSPI-connected memory will be discarded.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must be initialized. To initialize the QSPI, see NRFJPROG_qspi_init() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_qspi_init() functions.
 *
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    The RAM memory contents might be altered. To restore the contents of the RAM memory, see NRFJPROG_qspi_uninit() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to read from.
 * @param   data                                Pointer of the location to store the read data.
 * @param   data_len                            Number of bytes to read.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_qspi_init() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_len parameter is equal to 0.
 *                                              The addr parameter is greater than 0x00FFFFFFul when QSPI is configured
 *                                              to work with 24-bit addresses.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_read_inst(nrfjprog_inst_t instance,
                                                       uint32_t addr,
                                                       uint8_t * data,
                                                       uint32_t data_len);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_read(uint32_t addr, uint8_t * data, uint32_t data_len);

/**
 * @brief   Writes to the external QSPI-connected memory.
 *
 * @details Writes data_len characters to addr address of the external QSPI-connected memory. Actual write command address and length sent to the device may be adjusted since
 *          the QSPI peripheral only accepts 4-byte aligned addresses and multiple of 4 lengths. Any extra bytes written
 *          to the external QSPI-connected memory will be 0xFF to avoid altering the contents of the flash. Note that
 *          NRFJPROG_qspi_write() function does not erase the addresses to be written, so if target addresses are not
 *          erased, the flash contents after NRFJPROG_qspi_write() operation will therefore be undefined.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must be initialized. To initialize the QSPI, see NRFJPROG_qspi_init() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_qspi_init() functions.
 *
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 * @post    The RAM memory contents might be altered. To restore the contents of the RAM memory, see NRFJPROG_qspi_uninit() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to write to.
 * @param   data                                Pointer of the location of data to write.
 * @param   data_len                            Number of bytes to write.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_qspi_init() function has not been called.
 * @retval  INVALID_PARAMETER                   The data parameter is null.
 *                                              The data_len parameter is equal to 0.
 *                                              The addr parameter is greater than 0x00FFFFFFul when QSPI is configured
 *                                              to work with 24-bit addresses.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_write_inst(nrfjprog_inst_t instance,
                                                        uint32_t addr,
                                                        const uint8_t * data,
                                                        uint32_t data_len);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_write(uint32_t addr, const uint8_t * data, uint32_t data_len);

/**
 * @brief   Erases the external QSPI-connected memory.
 *
 * @details Erases length bytes from addr address of the external QSPI-connected memory. The given addr must be aligned to the desired erase length. Please note that some erase operations
 *          take a long time with certain external flash devices, so this operation might take minutes, specially with a
 *          full erase. Note that for certain external flash memory devices some erase lengths are not supported, please
 *          consult your datasheet and use the available options with care.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must be initialized. To initialize the QSPI, see NRFJPROG_qspi_init() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_qspi_init() functions.
 *
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   addr                                Address to erase from.
 * @param   length                              Desired erase length.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_qspi_init() function has not been called.
 * @retval  INVALID_PARAMETER                   The length parameter cannot be encoded in a qspi_erase_len_t.
 *                                              The addr parameter is greater than 0x00FFFFFFul when QSPI is configured
 *                                              to work with 24-bit addresses. The addr parameter is not aligned to
 *                                              erase length.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_erase_inst(nrfjprog_inst_t instance,
                                                        uint32_t addr,
                                                        qspi_erase_len_t length);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_erase(uint32_t addr, qspi_erase_len_t length);

/**
 * @brief   Sends a custom instruction to the external QSPI-connected memory.
 *
 * @details Sends the custom instruction with instruction_code code and instruction_length length to the external QSPI-connected memory.
 *          If data_in parameter is NULL, 0x00 will be sent as data in the custom instruction. The data_out parameter
 *          can be NULL. If more than 8 bytes of data is sent, it will start a long frame qspi operation if available in
 *          the device.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     The QSPI must be initialized. To initialize the QSPI, see NRFJPROG_qspi_init() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     A connection to the device must be established. To establish a connection, see NRFJPROG_connect_to_device() and NRFJPROG_qspi_init() functions.
 *
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_pin_reset(), NRFJPROG_debug_reset(),  NRFJPROG_go() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   instruction_code                    Instruction code of the custom instruction.
 * @param   instruction_length                  Length of the custom instruction.
 * @param   data_in                             Pointer to data to send in the custom instruction. Can be NULL if no data is desired to be sent, where 0x00 will be used if needed.
 * @param   data_out                            Pointer to data to write the data received during the custom instruction. Can be NULL if no data is desired.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_OPERATION                   The NRFJPROG_connect_to_emu_with_snr() or NRFJPROG_connect_to_emu_without_snr() function has not been called.
 *                                              There is no connection between the emulator and the device.
 *                                              The NRFJPROG_qspi_init() function has not been called.
 * @retval  OUT_OF_MEMORY                       Memory could not be allocated for the operation.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The instruction_length parameter is larger than 9 for a device that does not support long frame operations.
 * @retval  INVALID_PARAMETER                   The instruction_length parameter is equal to 0.
 *                                              The instruction_length parameter is greater than 9 and extended custom
 *                                                  instructions are not enabled.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_custom_inst(nrfjprog_inst_t instance,
                                                         uint8_t instruction_code,
                                                         uint32_t instruction_length,
                                                         const uint8_t * data_in,
                                                         uint8_t * data_out);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_qspi_custom(uint8_t instruction_code,
                                                    uint32_t instruction_length,
                                                    const uint8_t * data_in,
                                                    uint8_t * data_out);

/**
 *  @brief   Deprecated, this function only returns NOT_IMPLEMENTED_ERROR.
 **/
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_ram_powered_inst(nrfjprog_inst_t instance,
                                                            ram_section_power_status_t * ram_sections_power_status,
                                                            uint32_t ram_sections_power_status_array_size,
                                                            uint32_t * ram_sections_number,
                                                            uint32_t * ram_sections_size);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_is_ram_powered(ram_section_power_status_t * ram_sections_power_status,
                                                       uint32_t ram_sections_power_status_array_size,
                                                       uint32_t * ram_sections_number,
                                                       uint32_t * ram_sections_size);

/**
 * @brief   Programs the provided file to the connected device.
 *
 * @details Programs the provided file to the connected device. Supported file formats are ".hex", ".ihex", ".elf", ".bin" and ".zip".
 *
 *          A .zip file containing several images can be passed. Each file within the .zip that has a supported file
 *          extension will be programmed individually.
 *
 *
 *          A .zip file containing a modem firmware update can be passed to start a modem update process on lte devices.
 *
 *          Block protection (BPROT) will be disabled before writing to the
 *          device. No implicit erase or verify operation will be performed. See the NRFJPROG_erase_file() function for
 *          erasing the device before programming. For verifying device memories after programming, see the
 *          NRFJPROG_verify_file() function.
 *
 *          If the file contains data in external memory, the QSPI module must be configured before calling this
 *          function. See functions NRFJPROG_qspi_init_ini() and NRFJPROG_qspi_configure_ini(). Note that
 *          NRFJPROG_qspi_init() and NRFJPROG_qspi_configure() does not set the external memory size and must be used in
 *          combination with NRFJPROG_qspi_set_size().
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     If the file has data in external memory the QSPI module must be configured.
 * @post    If the file to program was a modem firmware package,  the selected coprocessor is CP_APPLICATION.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode see NRFJPROG_reset().
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_reset() and NRFJPROG_run() functions.
 * @post    All device RAM will be powered if the file contains data in any part of RAM. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   file_path                           Path to the file to be programmed.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The file path is NULL.
 *                                              The file cannot be opened for reading.
 * @retval  UNKNOWN_MEMORY_ERROR                The file has data at illegal or unknown memory addresses.
 * @retval  FILE_OPERATION_FAILED               Unable to open file.
 *                                              File is empty.
 * @retval  FILE_INVALID_ERROR                  File has overlapping segments of data.
 * @retval  FILE_PARSING_ERROR                  Failed to parse file contents.
 * @retval  FILE_UNKNOWN_FORMAT_ERROR           File has unsupported file ending.
 *                                              File contents did not match file ending format.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available because the device is readback protected.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support an attempted operation.
 * @retval  OUT_OF_MEMORY                       Could not allocate hex file buffer.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_program_file_inst(nrfjprog_inst_t instance, const char * file_path);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_program_file(const char * file_path);

/**
 * @brief   Reads the memory of the connected device into the provided file path.
 *
 * @details Reads the memory of the connected device into the provided file path. The areas of the
 *          device to read can be configured by the use of the read_options parameter. See the description of
 *          read_options_t for details. Supported file formats are ".hex", ".ihex", ".elf", ".bin".
 *
 *          If read_options includes external memory, the QSPI module must be configured before calling this function.
 *          See functions NRFJPROG_qspi_init_ini() and NRFJPROG_qspi_configure_ini(). Note that NRFJPROG_qspi_init() and
 *          NRFJPROG_qspi_configure() does not set the external memory size and must be used in combination with
 *          NRFJPROG_qspi_set_size().
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     If read_options includes external memory the QSPI module must be configured.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode see NRFJPROG_reset().
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_reset() and NRFJPROG_run() functions.
 * @post    All device RAM will be powered readram was specified. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   file_path                           Path to output file.
 * @param   read_options                        Pointer to struct containing read parameters.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  INVALID_PARAMETER                   The output file path is NULL.
 *                                              The file cannot be overwritten.
 *                                              The read_options pointer is NULL.
 *                                              The file cannot be opened for writing.
 * @retval  UNKNOWN_MEMORY_ERROR                The file has data at illegal or unknown memory addresses.
 * @retval  FILE_OPERATION_FAILED               Unable to open file.
 *                                              Unable to write to file.
 *                                              File is empty.
 * @retval  FILE_INVALID_ERROR                  File has overlapping segments of data.
 * @retval  FILE_PARSING_ERROR                  Failed to parse file contents.
 * @retval  FILE_UNKNOWN_FORMAT_ERROR           File has unsupported file ending.
 *                                              File contents did not match file ending format.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 *                                              The address to read is in unpowered RAM.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available because the device is readback protected.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support an attempted operation.
 * @retval  OUT_OF_MEMORY                       Could not write to the hex file.
 *                                              Could not allocate hex file buffer.
 *                                              Could not extend hex file buffer.
 *                                              Could not save the hex file.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_to_file_inst(nrfjprog_inst_t instance,
                                                          const char * file_path,
                                                          read_options_t read_options);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_read_to_file(const char * file_path, read_options_t read_options);

/**
 * @brief   Compares the contents of the provided file against the contents of the memory of the connected device.
 *
 * @details Compares the contents of the provided file against the contents of the memory of the connected device. The compare method can be configured with the verify_action parameter.
 *          If verify action is set to VERIFY_READ, the memory contents of the device is read using the debugger and
 *          directly compared to the contents of the provided file.
 *
 *          In the case of verify action VERIFY_HASH, a program
 *          is loaded into the device RAM that can calculate the hash of a memory area using on-board hardware
 *          acceleration. This method is usually faster than VERIFY_READ.
 *
 *          Supported file formats are ".hex", ".ihex", ".elf", ".bin" and ".zip".
 *
 *          A .zip file containing several images can be passed. Each file within the .zip that has a supported file
 *          extension will be verified individually.
 *
 *          On devices that have LTE support, a .zip file containing modem firmware can be passed to start a modem
 *          firmware verify process. Modem firmware verification only supports the VERIFY_HASH verify action.
 *
 *          If the file contains data in external memory, the QSPI module must be configured before calling this
 *          function. See functions NRFJPROG_qspi_init_ini() and NRFJPROG_qspi_configure_ini(). Note that
 *          NRFJPROG_qspi_init() and NRFJPROG_qspi_configure() does not set the external memory size and must be used in
 *          combination with NRFJPROG_qspi_set_size().
 *
 *          Devices with hardened AP protection by default ignore verification errors for AP-protect registers in UICR,
 *          since these register does not always erase to 0xFFFFFFFF. To control this behavior, configure property
 *          target.approtect.verify_uicr_approtect using NRFJPROG_configure().
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     If the file has data in external memory the QSPI module must be configured.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode see NRFJPROG_reset().
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_reset() and NRFJPROG_run() functions.
 * @post    All device RAM will be powered if the file contains data in any part of RAM. To unpower the device RAM, see NRFJPROG_unpower_ram_section() function.
 * @post    If the file to verify was a modem firmware package,  the selected coprocessor is CP_APPLICATION.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   file_path                           Path to output file.
 * @param   verify_action                       Type of verify to perform.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  VERIFY_ERROR                        Data in file does not match the contents of the connected device.
 * @retval  INVALID_OPERATION                   Requested verify action is not available.
 *                                              There is data to verify in external memory, but the QSPI module has not
 *                                              been configured.
 * @retval  INVALID_PARAMETER                   The output file path is NULL.
 *                                              The read_options pointer is NULL.
 * @retval  UNKNOWN_MEMORY_ERROR                The file has data at illegal or unknown memory addresses.
 * @retval  FILE_OPERATION_FAILED               Unable to open file.
 *                                              File is empty.
 * @retval  FILE_INVALID_ERROR                  File has overlapping segments of data.
 * @retval  FILE_PARSING_ERROR                  Failed to parse file contents.
 * @retval  FILE_UNKNOWN_FORMAT_ERROR           File has unsupported file ending.
 *                                              File contents did not match file ending format.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available because the device is readback protected.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support an attempted operation.
 * @retval  OUT_OF_MEMORY                       Could not allocate hex file buffer.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_verify_file_inst(nrfjprog_inst_t instance,
                                                         const char * file_path,
                                                         verify_action_t verify_action);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_verify_file(const char * file_path, verify_action_t verify_action);

/**
 * @brief   Erases the flash of the connected device based on the contents of the provided file.
 *
 * @details Erases the flash of the connected device. If erase mode is set to erase page, only the pages where the provided file contains data will be erased.
 *          The mode of operation can be configured by setting the erase method for the internal device memory and for
 *          the external memory. Block protection (BPROT) will be disabled if needed before erasing the device.
 *
 *          Supported file formats are ".hex", ".ihex", ".elf", ".bin" and ".zip".
 *
 *          A .zip file containing several images can be passed. Each file within the .zip that has a supported file
 *          extension will be passed to the erase function individually.
 *
 *          If the file contains data in external memory and qspi_erase_action is set, the QSPI module must be
 *          configured before calling this function. See functions NRFJPROG_qspi_init_ini() and
 *          NRFJPROG_qspi_configure_ini(). Note that NRFJPROG_qspi_init() and NRFJPROG_qspi_configure() does not set the
 *          external memory size and must be used in combination with NRFJPROG_qspi_set_size().
 *
 *          An "erase all" operation can also be triggered using the dedicated NRFJPROG_erase_all() function.
 *
 * @pre     The dll must be open. To open the dll, see NRFJPROG_open_dll() function.
 * @pre     A connection to the emulator must be established. To establish a connection, see NRFJPROG_connect_to_emu_with_snr() and NRFJPROG_connect_to_emu_without_snr() functions.
 * @pre     If the file has data in external memory and qspi_erase_mode is set, the QSPI module must be configured.
 *
 * @post    The device will be in debug interface mode. To exit debug interface mode see NRFJPROG_reset().
 * @post    The device CPU will be halted. To unhalt the device CPU, see NRFJPROG_reset() and NRFJPROG_run() functions.
 *
 * @param   instance                            A handle to an open nrfjprog instance.
 * @param   file_path                           Path to output file.
 * @param   chip_erase_mode                     Erase mode for internal flash memory
 * @param   qspi_erase_mode                     Erase mode for external memory.
 *
 * @retval  SUCCESS
 * @retval  INVALID_SESSION                     Instance is not a valid nrfjprog instance, or NRFJPROG_open_dll() function has not been called.
 * @retval  VERIFY_ERROR                        Data in file does not match the contents of the connected device.
 * @retval  INVALID_OPERATION                   Requested verify action is not available.
 *                                              There is data to verify in external memory, but the QSPI module has not
 *                                              been configured.
 * @retval  INVALID_PARAMETER                   The output file path is NULL.
 *                                              The read_options pointer is NULL.
 * @retval  UNKNOWN_MEMORY_ERROR                The file has data at illegal or unknown memory addresses.
 * @retval  FILE_OPERATION_FAILED               Unable to open file.
 *                                              File is empty.
 * @retval  FILE_INVALID_ERROR                  File has overlapping segments of data.
 * @retval  FILE_PARSING_ERROR                  Failed to parse file contents.
 * @retval  FILE_UNKNOWN_FORMAT_ERROR           File has unsupported file ending.
 *                                              File contents did not match file ending format.
 * @retval  JLINKARM_DLL_ERROR                  The JLinkARM DLL function returned an error.
 * @retval  JLINKARM_DLL_TIMEOUT_ERROR          Communication with the J-Link probe timed out.
 * @retval  NOT_AVAILABLE_BECAUSE_PROTECTION    The operation is not available because the device is readback protected.
 * @retval  INVALID_DEVICE_FOR_OPERATION        The connected device does not support an attempted operation.
 * @retval  OUT_OF_MEMORY                       Could not allocate hex file buffer.
 */
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_erase_file_inst(nrfjprog_inst_t instance,
                                                        const char * file_path,
                                                        erase_action_t chip_erase_mode,
                                                        erase_action_t qspi_erase_mode);
NRFJPROG_API nrfjprogdll_err_t NRFJPROG_erase_file(const char * file_path,
                                                   erase_action_t chip_erase_mode,
                                                   erase_action_t qspi_erase_mode);

#if defined(__cplusplus)
}
#endif

#endif /* NRFJPROGDLL_FUNC_H */
