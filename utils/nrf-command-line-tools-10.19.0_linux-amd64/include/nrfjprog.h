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
 
#ifndef NRFJPROG_H
#define NRFJPROG_H

#define major_version (10) 
#define minor_version (19) 
#define micro_version (0) 


 enum NrfjprogErrorCodesType {
    
    Success                                         = 0,                // Requested operation (operations) were successfully completed.
                                                                        
    /* nrfjprog.exe or PC errors */                                     
    NrfjprogError                                   = 1,                // An error condition that should not occur has happened. 
                                                                        // It is most probably a bug in nrfjprog.exe or nrfjprog.dll. 
    NrfjprogOutdatedError                           = 2,                // Nrfjprog version is too old for the device 
    MemoryAllocationError                           = 3,                // Memory allocation for nrfjprog failed failed.
                                                                        
    /* Command line input errors */                                     
    InvalidArgumentError                            = 11,               // Invalid arguments passed to the application.
    InsufficientArgumentsError                      = 12,               // Needed arguments not passed to the application.
    IncompatibleArgumentsError                      = 13,               // Incompatible arguments passed to the application.
    DuplicatedArgumentsError                        = 14,               // The same argument has been provided twice.
    NoOperationError                                = 15,               // The arguments passed do not perform a valid operation.
    UnavailableOperationBecauseProtectionError      = 16,               // The operation attempted can not be performed because either the main-ap or the ctrl-ap is not available.
    UnavailableOperationInFamilyError               = 17,               // The operation attempted can not be performed in the device because the feature is lacking in your device.
    WrongFamilyForDeviceError                       = 18,               // The --family option given with the command (or the default from nrfjprog.ini) does not match the device connected.
    UnavailableOperationBecauseMpuConfiguration     = 19,               // For nRF51, --eraseuicr is unavailable unless the device came with an ANT softdevice programmed at Nordic factory.
                                                                        
    /* nrfjprog.dll errors */                                           
    NrfjprogDllNotFoundError                        = 20,               // Unable to find nrfjprog.dll in the installation folder. Reinstall nrfjprog.
    NrfjprogDllLoadFailedError                      = 21,               // Failed to Load nrfjprog.dll
    NrfjprogDllFunctionLoadFailedError              = 22,               // Failed to Load the functions from nrfjprog.dll
    NrfjprogDllNotImplementedError                  = 23,               // Dll still does not implement this function for your device.
    NrfjprogWorkerExeNotFoundError                  = 29,               // The Dll could not find the required worker executable.
                                                                        
    /* nrfjprog.ini errors */                                           
    NrfjprogIniSyntaxError                          = 24,               // Syntax error in nrfjprog.ini file
    NrfjprogIniNotFoundError                        = 25,               // Unable to find nrfjprog.ini in the installation folder. Reinstall nrfjprog.
    NrfjprogIniCannotBeOpenedError                  = 26,               // Opening the nrfjprog.ini file for read failed.
    NrfjprogIniFamilyMissingError                   = 27,               // Family parameter cannot be parsed from ini file. Line might be deleted or invalid format.
    NrfjprogIniClockspeedMissingError               = 28,               // Clock speed parameter cannot be parsed from ini file. Line might be deleted or invalid format.
    NrfjprogIniPropertyMissingError                 = 39,               // A required property is missing from the selected configuration file.
                                                                        
    /* JLinkARM.dll errors */                                           
    JLinkARMDllNotFoundError                        = 30,               // Unable to find install path for JLink software
    JLinkARMDllInvalidError                         = 31,               // Dll found does not seem a valid dll.
    JLinkARMDllFailedToOpenError                    = 32,               // Dll could not be opened.
    JLinkARMDllError                                = 33,               // Dll reported error.
    JLinkARMDllTooOldError                          = 34,               // Dll is too old for functionality. Install a newer version of JLinkARM.dll       
    JLinkARMDllTimeOutError                         = 35,               // Host-to-J-Link communication timed out.
    UnavailableOperationBecauseCoprocessorDisabled  = 36,               // Operation is not able to be executed because of the selected coprocessor is disabled.
    UnavailableOperationBecauseTrustZone            = 37,               // The address area attempted to be accessed is unavailable because of TrustZone set up.
    UnavailableOperationBecauseBPROT                = 38,               // The address area attempted to be accessed is unavailable because of memory block protection (ie. MPU, BPROT, ACL, or SPU) setup.
                                                                        
    /* Emulator errors */                                               
    InvalidSerialNumberError                        = 40,               // Serial number provided is not among those connected.
    NoDebuggersError                                = 41,               // There are no debuggers connected to the PC.
    NotPossibleToConnectError                       = 42,               // Not possible to connect to the NRF device.
    LowVoltageError                                 = 43,               // Low voltage detected at target device.
                                                                        
    /* General errors */                                                
    FileNotFoundError                               = 51,               // Unable to find the given file.
    InvalidHexFileError                             = 52,               // File specified does not seem a valid hex file.
    FicrReadError                                   = 53,               // FICR read failed.
    WrongArgumentError                              = 54,               // One of the arguments is wrong. Path does not exist, memory access is not aligned...
    VerifyError                                     = 55,               // The write verify operation failed.
    NoWritePermissionError                          = 56,               // Unable to create file in the current working directory.
    NVMCOperationError                              = 57,               // The flash operation in the device failed.
    FlashNotErasedError                             = 58,               // A program operation failed because the area to write was not erased.
    RamIsOffError                                   = 59,               // The RAM area to read or write is unpowered.
    NoReadPermissionError                           = 60,               // Unable to open file for read.
    NoExternalMemoryConfiguredError                 = 61,               // A QSPI operation is attempted without an external memory configured.
    RecoverFailed                                   = 62,               // Recovery failed.
    InternalError                                   = 63,               // An unexpected internal error occured.
    TimeOutError                                    = 64,               // An operation timed out.
    ReadError                                       = 65,               // A read operation failed.
                                                                        
    /* QSPI ini parsing. */                                             
    NrfjprogQspiIniNotFoundError                    = 70,               // Deprecated. NrfjprogIniPropertyMissingError is used instead. Check log for details.
    NrfjprogQspiIniCannotBeOpenedError              = 71,               // Deprecated. NrfjprogIniPropertyMissingError is used instead. Check log for details.
    NrfjprogQspiSyntaxError                         = 72,               // Deprecated. NrfjprogIniPropertyMissingError is used instead. Check log for details.
    NrfjprogQspiIniParsingError                     = 73,               // Deprecated. NrfjprogIniPropertyMissingError is used instead. Check log for details.
    NrfjprogQspiIniCustomMissingError               = 74,               // Deprecated. NrfjprogIniPropertyMissingError is used instead. Check log for details.
                                                                            
    /* Warning. Will not be returned by nrfjprog but used to generate warnings. */
    FicrOperationWarning                            = 100,              // FICR operation. It is important to be certain of what you do.
    UnalignedPageEraseWarning                       = 101,              // Address provided with page erase is not aligned to first address of page.
    NoLogWarning                                    = 102,              // Deprecated. NoLogWritePermissionWarning and NoLogParentDirDoesNotExistWarning are used instead.
    UicrWriteOperationWithoutEraseWarning           = 103,              // A UICR write operation is requested but there has not been a UICR erase.
    VeryLongOperationWarning                        = 104,              // An operation that might take up to several minutes is been executed. Please wait.
    FlashNotErasedWarning                           = 105,              // The address being written to is not empty. Writing to this address may results in unexpected behavior.
    NoVerifyOperationWarning                        = 106,              // No verify operation selected with program. Result may not be correct.
    LogWritePermissionWarning                       = 110,              // No log is possible because the program could not open the log file for writing.
    IniAndConfigWarning                             = 111,              // --ini and --config were both provided
    QspiIniAndConfigWarning                         = 112,              // --qspiini and --config were both provided
};

#endif //NRFJPROG_H
