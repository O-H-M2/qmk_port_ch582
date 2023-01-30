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

#ifndef MERGEHEX_H
#define MERGEHEX_H

#define major_version (10) 
#define minor_version (19) 
#define micro_version (0) 

enum MergehexErrorCodesType
{

    Success = 0, // Requested operation (operations) were successfully completed.

    /* mergehex.exe or PC errors */
    MergehexError = 1,         // An error condition that should not occur has happened.
                               // It is most probably a bug in mergehex.exe.
    MemoryAllocationError = 3, // Memory allocation for mergehex failed.

    /* Command line input errors */
    InvalidArgumentError       = 11, // Invalid arguments passed to the application.
    InsufficientArgumentsError = 12, // Needed arguments not passed to the application.
    IncompatibleArgumentsError = 13, // Incompatible arguments passed to the application.
    DuplicatedArgumentsError   = 14, // The same argument has been provided twice.
    NoOperationError           = 15, // The arguments passed do not perform a valid operation.

    /* General errors */
    FileError              = 50, // A file operation failed. See output.
    FileNotFoundError      = 51, // Unable to find the given file.
    InvalidHexFileError    = 52, // File specified does not seem to be a valid file for the detected format.
    NoWritePermissionError = 56, // megehex application has no write permission in the given path.
    ConflictError          = 60, // There is a conflict in the files to merge.
};

#endif // MERGEHEX_H