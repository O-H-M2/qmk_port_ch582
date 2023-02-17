/*
 * ISP download command tool for WCH MCU series.
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
 * Cross-compile with cross-gcc -I /path/to/cross-kernel/include
 *
 */

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <iostream>

#include "LinuxTypes.H"
#include "Utils.H"
#include "WCH55XISPDLL.H"

#define VERSION "V3.40"

#define DEV_MAX_NUMBER 16
#define BIN_MAX_BUFLEN 1024 * 1024

#pragma pack(1)

/* command parameter structure */
typedef struct _CMDPARA {
    char PortName[16];        /* usb or uart device name */
    int PortBaud;             /* uart baudrate */
    bool bPrtVer;             /* print BOOT version */
    bool bFlashOpt;           /* whether to program or verify */
    char CfgFile[MAX_PATH];   /* configuration file path */
    char OptType;             /* operation type, 0: download, 1: verfiy */
    char FlashFile[MAX_PATH]; /* download file path */
} CMDPARA, *pCmdPara;

/* devices array */
Isp55xDevInforS AfxDnDev[DEV_MAX_NUMBER] = {0};
/* download option, must be set before operation */
CH55xIspOptionS IspOption = {0};
/* device index */
unsigned int uDevIndex = 0;
/* command parameter */
CMDPARA cmdPara = {0};

/*
 * GetDevIndex - get device index number
 *
 * The function return 0 if success, or others if fail.
 */
int GetDevIndex()
{
    int iRet = 4, iDevCnt = 0, i = 0;
    unsigned char BtChipSeries;
    unsigned char BtChipType;
    BOOL IsPreBTV230;
    char szStr[20] = "";

    if (strlen(cmdPara.PortName) < 3) {
        iRet = 5;
        goto _END;
    }
    sprintf(szStr, "%s", cmdPara.PortName);
    /* enumerate the devices automatically */
    iDevCnt = WCH55x_EnumDevices(AfxDnDev, DEV_MAX_NUMBER, &BtChipSeries, &BtChipType, &IsPreBTV230);
    if (iDevCnt == 0) {
        iRet = 6;
        goto _END;
    }

    for (i = 0; i < iDevCnt; i++) {
        if (strcmp(AfxDnDev[i].PortName, szStr) == 0) {
            uDevIndex = i;
            iRet = 0;
            goto _END;
        }
    }

_END:
    return iRet;
}

/**
 * DoOption - perform mcu download or verify operation
 * @bFlashOpt: true on download operation, false on verify only
 *
 * The function return 0 if success, or others if fail.
 */
int DoOption(bool bFlashOpt)
{
    int iRet = 1;
    char szStr[200] = "";
    int hFile;
    unsigned long uFileLen = 0, uBinLen = 0, uWrRdLen = 0;
    char *pFileBuf = NULL, *pBinBuf = NULL;

    /* search isp device to operate */
    if (!WCH55x_GetIspDeviceInfor(uDevIndex, &AfxDnDev[uDevIndex])) {
        iRet = 7;
        goto _END;
    }

    /* print boot version */
    if (cmdPara.bPrtVer) {
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "BOOT VERSION:     v%X.%X%X\r\n", AfxDnDev[uDevIndex].IspVer[1], AfxDnDev[uDevIndex].IspVer[2],
                AfxDnDev[uDevIndex].IspVer[3]);
        std::cout << szStr << std::endl;
        iRet = 0;
    }

    if (bFlashOpt) {
        memset(szStr, 0, sizeof(szStr));
        sprintf(szStr, "{\"Device\":\"%s\",\"Status\":\"Ready\"}", cmdPara.PortName);
        std::cout << szStr << std::endl;

        /* read target file */
        if (strlen(cmdPara.FlashFile) < 1) {
            iRet = 8;
            goto _END;
        }

        hFile = open(cmdPara.FlashFile, O_RDONLY);
        if (hFile < 0) {
            iRet = 9;
            goto _END;
        }

        uFileLen = Get_file_size(cmdPara.FlashFile);
        if (uFileLen < 1) {
            iRet = 9;
            goto _END;
        }

        pFileBuf = (char *)malloc(uFileLen + 1);
        memset(pFileBuf, 0, uFileLen + 1);

        pBinBuf = (char *)malloc(BIN_MAX_BUFLEN);
        memset(pBinBuf, 0, BIN_MAX_BUFLEN);

        uWrRdLen = read(hFile, pFileBuf, uFileLen);
        if (uWrRdLen != uFileLen) {
            iRet = 10;
            goto _END;
        }

        close(hFile);

        /* deal with file and convert the file to bin format */
        if (strstr(cmdPara.FlashFile, ".hex") || strstr(cmdPara.FlashFile, ".HEX")) {
            if (!HexToBin(0, pFileBuf, uFileLen, pBinBuf, &uBinLen)) {
                iRet = 11;
                goto _END;
            }
        } else {
            memcpy(pBinBuf, pFileBuf, uFileLen);
            uBinLen = uFileLen;
        }

        /* the length is large enough for clearing dataflash */
        uWrRdLen = 800 * 1024;
        if (IspOption.IsClearDataFlash) {
            if (!WCH55x_WriteDataFlash(AfxDnDev[uDevIndex].Index, 0, &uWrRdLen, (unsigned char *)"CLEAR", true)) {
                iRet = 12;
                goto _END;
            }
        }

        if (cmdPara.OptType == 0) {
            memset(szStr, 0, sizeof(szStr));
            /* download file to flash */
            if (!WCH55x_FlashProgramB(AfxDnDev[uDevIndex].Index, (unsigned char *)pBinBuf, uBinLen, 1, NULL, 0, 0)) {
                iRet = 13;
                goto _END;
            } else {
                usleep(10 * 1000);
                memset(szStr, 0, sizeof(szStr));
                sprintf(szStr, "{\"Device\":\"%s\", \"Status\":\"Programming\", \"Progress\":100%%} ",
                        cmdPara.PortName);
                std::cout << szStr << std::endl;
                iRet = 0;
                goto _END;
            }
        } else {
            /* read flash and verify */
            if (!WCH55x_FlashVerifyB(AfxDnDev[uDevIndex].Index, (unsigned char *)pBinBuf, uBinLen, 1, NULL, 0, 0)) {
                iRet = 14;
                goto _END;
            } else {
                usleep(10 * 1000);
                memset(szStr, 0, sizeof(szStr));
                sprintf(szStr, "{\"Device\":\"%s\", \"Status\":\"Verifing\", \"Progress\":100%%} ", cmdPara.PortName);
                std::cout << szStr << std::endl;
                iRet = 0;
                goto _END;
            }
        }
    }

_END:
    if (hFile < 0) {
        close(hFile);
    }
    if (pFileBuf != NULL) {
        free(pFileBuf);
        pFileBuf = NULL;
    }
    if (pBinBuf != NULL) {
        free(pBinBuf);
        pBinBuf = NULL;
    }

    return iRet;
}

/**
 * option structure
 */
static const struct option lopts[] = {
    {"device", required_argument, 0, 'p'},
    {"speed", optional_argument, 0, 'b'},
    {"version", required_argument, 0, 'v'},
    {"configure", required_argument, 0, 'c'},
    {"operation", required_argument, 0, 'o'},
    {"flash", required_argument, 0, 'f'},
    {NULL, 0, 0, 0},
};

/**
 * print_usage - parameter usage helper
 *
 * The function has no return value.
 */
static void print_usage(const char *prog)
{
    printf("Usage: %s [-pbvecofh]\n", prog);
    puts(
        "  -p --device    device to operate\n"
        "  -b --speed     uart speed\n"
        "  -v --version   boot/tool version\n"
        "  -c --configure configure file path\n"
        "  -o --operation operation type\n"
        "  -f --flash	   flash file path\n");
    exit(1);
}

/**
 * parse_opts - parse command options
 * @argc: arguments counts
 * @argv: arguments string array
 *
 * The function has no return value.
 */
static void parse_opts(int argc, char *argv[])
{
    int c;

    while (1) {
        c = getopt_long(argc, argv, "p:b:v:c:o:f:h", lopts, NULL);
        if (c == -1) {
            break;
        }
        switch (c) {
        case 'p':
            if (optarg != NULL)
                strcpy(cmdPara.PortName, optarg);
            break;
        case 'b':
            if (optarg != NULL)
                cmdPara.PortBaud = atoi(optarg);
            break;
        case 'v':
            if (strcmp(optarg, "boot") == 0)
                cmdPara.bPrtVer = true;
            break;
        case 'c':
            if (optarg != NULL)
                strcpy(cmdPara.CfgFile, optarg);
            break;
        case 'o':
            cmdPara.bFlashOpt = true;
            if (strcmp(optarg, "verify") == 0)
                cmdPara.OptType = 1;
            else if (strcmp(optarg, "program") == 0)
                cmdPara.OptType = 0;
            break;
        case 'f':
            if (optarg)
                strcpy(cmdPara.FlashFile, optarg);
            break;
        case 'h':
        default:
            print_usage(argv[0]);
            break;
        }
    }
}

/*
 * structure for codes and messages.
 */
struct result {
    int i;
    char msg[20];
    char msg1[512];
};

/*
 * operation codes and messages.
 */
static const struct result rets[] = {
    {0, "{\"Device\":", "\", \"Status\":\"Finished\", \"Code\":0,\"Message\":\"Succeed\"}\r\n"},
    {2, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":2, \"Message\":\"Fail to get parameters from cfg file\"}\r\n"},
    {3, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":3, \"Message\":\"Fail to set isp parameters\"}\r\n"},
    {4, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":4, \"Message\":\"Fail to find the specified isp device\"}\r\n"},
    {5, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":5, \"Message\":\"The specified isp device name is invalid\"}\r\n"},
    {6, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":6, \"Message\":\"Fail to find any valid isp device\"}\r\n"},
    {7, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":7, \"Message\":\"Fail to get device info\"}\r\n"},
    {8, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":8, \"Message\":\"The path of the flash file is invalid\"}\r\n"},
    {9, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":9,\"Message\":\"The length of the flash file is invalid\"}\r\n"},
    {10, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":10, \"Message\":\"Fail to read the flash file\"}\r\n"},
    {11, "{\"Device\":", "\", \"Status\":\"Fail\",\"Code\":11, \"Message\":\"Fail to convert the flash file from HEX to BIN\"}\r\n"},
    {12, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":12, \"Message\":\"Fail to clear dataflsh\"}\r\n"},
    {13, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":12, \"Message\":\"Fail to download\"}\r\n"},
    {14, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":12, \"Message\":\"Fail to verify\"}\r\n"},
    {100, "{\"Device\":", "\", \"Status\":\"Fail\", \"Code\":100, \"Message\":\"Unknow error\"}\r\n"}};

/**
 * parse_result - output message
 * @code: code number
 *
 * The function has no return value.
 */
static void parse_result(int code)
{
    char TextBufferTmp[2000] = "";
    int index;

    if (code == 100)
        index = sizeof(rets) / sizeof(struct result) - 1;
    else if (code >= 2)
        index = code - 1;
    else
        index = code;

    sprintf(TextBufferTmp, "%s%s%s", rets[index].msg, cmdPara.PortName, rets[index].msg1);
    std::cout << TextBufferTmp << std::endl;
}

/**
 * main - main routine
 * @argc: arguments counts
 * @argv: arguments string array
 *
 * The function return 0 if success, or others if fail.
 */
int main(int argc, char *argv[])
{
    int i = 0, ret = 0, iRet = 0;
    char TextBufferTmp[2000] = "";
    bool bRemove = false;

    std::cout << "\r\n=====ISP_Command_Tool=====\r\n" << std::endl;
    std::cout << "TOOL VERSION:  " << VERSION << std::endl << std::endl;

    parse_opts(argc, argv);

    /* no valid operation, return */
    if (!cmdPara.bPrtVer && !cmdPara.bFlashOpt)
        return 0;

    sprintf(TextBufferTmp, "p:%s\r\nb:%d\r\nv:%d\r\nc:%s\r\no:%d\r\nf:%s\r\n", cmdPara.PortName, cmdPara.PortBaud,
            cmdPara.bPrtVer, cmdPara.CfgFile, cmdPara.OptType, cmdPara.FlashFile);
    std::cout << TextBufferTmp << std::endl;

    /* device name and configuration file path must be valid when downloading or verifying */
    if (cmdPara.bFlashOpt) {
        if ((strlen(cmdPara.PortName) == 0) || (strlen(cmdPara.CfgFile) == 0)) {
            std::cout << "No specified device and configuration file.\n" << std::endl;
        }
    }

    /* disable debug messages */
    SetDebug(0);

    /* load chip configuration */
    LoadChipCfg();

    if (cmdPara.bPrtVer || cmdPara.bFlashOpt) {
        iRet = SetIspCfg(&IspOption, cmdPara.CfgFile, cmdPara.PortName, cmdPara.PortBaud);
        if (iRet) {
            std::cout << "read configuration file or set isp option to device error.\n" << std::endl;
            goto exit;
        }
    }

retry:

    iRet = GetDevIndex();
    if (iRet)
        goto exit;
    if (cmdPara.OptType == 0 && ret == 0) {
        if (WCH55x_IspRemoveProtect(uDevIndex)) {
            Sleep(2000);
            if (bRemove == false) {
                bRemove = true;
                goto retry;
            }
        }
    }

    iRet = DoOption(cmdPara.bFlashOpt);

exit:
    parse_result(iRet);
    return iRet;
}
