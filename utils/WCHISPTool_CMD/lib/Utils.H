/*
 * Header file for Helper function.
 *
 * Copyright (C) 2022 Nanjing Qinheng Microelectronics Co., Ltd.
 * Web: http://wch.cn
 * Author: WCH@TECH39 <tech@wch.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __UTILS_H
#define __UTILS_H

#include "LinuxTypes.H"
#include "WCH55XISPDLL.H"

// Convert 2 characters to 1 byte hexadecimal value
BOOL Char2ToHex(PCHAR pInChar,     // 2 character buffers to be converted
                PUCHAR pOutChar);  // converted hexadecimal value

// Convert string to integer
UINT mTStrToHEX(PCHAR str);  // string to be converted

// Convert HEX format file to BIN format
BOOL HexToBin(ULONG StartAddr,   // file start address
              PVOID Hexbuf,      // HEX file buffer
              ULONG iHexBufLen,  // length of HEX file buffer
              PVOID Binbuf,      // length of BIN fle buffer
              PULONG iBinLen);   // length of BIN file buffer

// Read all data to buffer from download file, it should be converted to BIN file if it is HEX file. The downloaded data
// is aligned with 4K length, and 0xff is filled for the part less then 4096.
BOOL ReadDataFromDnFileB(PUCHAR FileDataBuf, ULONG FileDataLen,
                         UCHAR FileDataType,  // 0:HEX, 1:BIN
                         ULONG StartAddr,    // start address of file
                         PUCHAR DnDataBuf,   // download file content buffer
                         ULONG *DnDataLen);  // download file content length

// Load chip configuration
BOOL LoadChipCfg();

// Get file length
UINT Get_file_size(LPCSTR path);

// Set log switch
VOID SetDebug(INT level);

// Delay function, in milliseconds
VOID Sleep(INT msecs);

// Resovle the IspOption download option from the configuration file
INT SetIspCfg(PCH55xIspOptionS pIspOption, CHAR *cfgfilename, CHAR *PortName, INT PortBaud);

// Read the specified key value from the configuration file
INT GetProfileString(PCHAR profile, PCHAR AppName, PCHAR KeyName, PCHAR KeyVal);

#endif
