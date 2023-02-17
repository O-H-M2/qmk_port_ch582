/*
 * ISP Development Library for WCH MCU.
 *
 * Copyright (C) 2022 Nanjing Qinheng Microelectronics Co., Ltd.
 * Web: http://wch.cn
 * Author: WCH@TECH37 <tech@wch.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#define LINUX_VER

#ifndef		_WCH55XISP_DLL_H
#define		_WCH55XISP_DLL_H

#ifdef  LINUX_VER
#include "LinuxTypes.H"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define		mOFFSET( s, m )			( (ULONG) & ( ( ( s * ) 0 ) -> m ) )

#ifndef		max
#define		max( a, b )				( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#endif

#ifndef		min
#define		min( a, b )				( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

#define MAX_ISPDEV_NUM 16

//MCU types
#define   MCU_CH55X   0
#define   MCU_CH56X   1
#define   MCU_CH54X   2
#define   MCU_CH57X   3
#define   MCU_CH32F1X 4
#define   MCU_CH32V1X 5
#define   MCU_CH58X   6
#define   MCU_CH32V3X 7
#define   MCU_CH32F2X 8
#define   MCU_CH32V2X 9

#ifndef LINUX_VER
typedef		VOID	( CALLBACK	* mPCH55X_USB_NOTIFY_ROUTINE ) (  //MCU event notification callback procedure
		    ULONG			iEventStatus,	//MCU events and current status (defined in the following line): 0=MCU unplug event, 3=MCU plug event
			ULONG           DevIndexArray,	//serial number of the MCU inserted or removed
			ULONG           DevCnt);		//total number of current MCU

typedef		VOID	( CALLBACK	* mOutputPrint ) (	//application layer information output function
			UCHAR InforType,WCHAR *DataStr);

typedef		VOID	( CALLBACK	* mAppendPrint ) (	//application layer information append output function
			ULONG LineNo,WCHAR *DataStr);
#endif

#define InforType_Succ 0
#define InforType_Err  1
#define InforType_Hit  2

#ifndef LINUX_VER
typedef		VOID	( CALLBACK	* mPCH55X_USB_NOTIFY_ROUTINE ) (  //MCU event notification callback procedure
		    ULONG			iEventStatus,  //MCU events and current status : 0=device unplug, 3=device plug 
			ULONG           DevIndexArray, //serial number of the MCU 
			ULONG           DevCnt);       //total number of connected MCU
#endif


#pragma pack(1)

//MCU performance parameters structure 
typedef struct _CHIP_INFOEX {
	TCHAR		ChipName[16];
	UINT		FlashMaxSize;
	UINT		EepromMaxSize;
	BOOL		IsNetworkDownloadAvailable;
	BOOL		IsUsbDownloadAvailable;
	BOOL		IsSerialDownloadAvailable;
	UCHAR   	DeviceType;
#ifndef LINUX_VER
	LIST_ENTRY	listEntry;
#endif
	UCHAR		ChipId;
	USHORT		EepromStartAddr;
	TCHAR       Introduction[256];
	TCHAR       IntroductionEn[256];
}CHIP_INFOEX, *PCHIP_INFOEX;

//MCU information parameters structure 
typedef struct _ISP55XDEVICEINFOR
{
	ULONG       Index;			//serial number of the MCU after enumeration
	UCHAR       McuType;		//MCU type
	BOOL        IsSupportUID;	//MCU support getting UID
	UCHAR       IspVer[4];		//ISP version
	UCHAR       IspMcuUID[8];	//MCU Unique ID
	CHAR        PortName[32];	//serial MCU name, only the value is available when the serial port is downloaded
	BOOL        DevIsOnline;	//MCU is connected to the serial port, only useful when downloading from the serial port
	BOOL        IsCoolBoot;		//it is powering up for BOOT,or the configuration bits cannot be modified
	UCHAR       MacAddr[6];
	BOOL        IsPreBTV230;
}Isp55xDevInforS, *PIsp55xDevInforS;


//MCU ISP parameters  structure
typedef struct _CH55xISPSETTING
{
	ULONG       IspInterface;	//download interface type 0:USB, 1:serial port 2:network port
	UCHAR       IspMcuType;		//MCU type, the last two digits of the chip type, such as CH563, then write 0x63
	UCHAR       IspMcuSeries;	//MCU type 0x00:CH55X;0x01:CH56X;0x02:CH54X;0x03:CH57X;0x04:CH32F10X;0x05:CH32V10X;0x06:CH58X;0x07:CH32V30X;0x8:CH32F20X;0x09:CH32V20X;0x0a:CH32F208;0x0b:CH32V00X

	BOOL   IsEnableLongRest;	//enables additional delayed reset during power-on reset
	BOOL   IsXtOscStrong;		//enable crystal oscillator to enhance external drive capability, CH554 does not support
	BOOL   IsEnableResetPin;	//specify the manual reset input pin, CH554 use RST pin, other types specify P5.7
	BOOL   IsEnableP0PullUp;	//enable internal pull-up resistor on port P0 during system reset,CH554 does not support
	BOOL   IsMcuResetAfterIsp;	//reset the target program to run directly after the download is completed 
	BOOL   IsEnableCfgBuf64K;
	BOOL   EnableIce;			//multiplexed as serial port one-touch download function at CH55X,CH32FX
	BOOL   EnableBootLoader;
	BOOL   IsSetNetDevMac;
	UCHAR  NetDevMacAddr[6];	//the first byte is multiplexed as record the number of write-protected blocks at CH58X 
	BOOL   IsCodeProtect;		//enable code protection
	BOOL   IsFirstRunIAP;		//run IAP after power up
	BOOL   IsEnableIAP;			//enable IAP
	ULONG  IAPStartAddr;		//IAP Start Address
	CHAR   UserFileName[MAX_PATH];	//user filename
	CHAR   IapFileName[MAX_PATH];	//IAP filename
	CHAR   DataFileName[MAX_PATH];	//dataflash filename

	BOOL   IsNoKeyDnAtSer;		//use the serial port ammonium-free key download mode
	UCHAR  BootPinNum;			//MCU BOOT pin
	BOOL   IsClearDataFlash;	//clear dataflash

	BOOL   IsEnableUsbPnpNotify;	//enable USB plug notification
	BOOL   IsEraseAllCFlash;
	BOOL   IsEraseAllDFlash;
#ifndef LINUX_VER
	mPCH55X_USB_NOTIFY_ROUTINE   UsbPnpNotifyRoutine;     //Plug notification callback function
#endif
	ULONG  LocalIP;              //local ISP IP address
	ULONG  GatewayIP;
	ULONG  MaskIP;
	BOOL   IsSetDevIP;
	ULONG  dwDevIP;

#ifndef LINUX_VER
	mOutputPrint AppOutput;       //application information print function
	mAppendPrint AppAppendOutput; //application information append print function
#endif

	BOOL    UILangIsCH;           //the language of UI
	UCHAR   LV_RST_VOL;           //threshold voltage
	BOOL    IsSimulat;            //enable two-wire emulation debug interface 
	BOOL    IsBootLoader;         //enable bootloader mode
	BOOL    IsPorCtr;
	BOOL    IsUsbdPu;
	BOOL	IsUsbdMode;
	BOOL	IsStandbyRst;
	BOOL	IsStopRst;
	BOOL	IsIwdgSw;
	UCHAR   WPR_DATA[8];

	BOOL    IsEnLockup;
	BOOL    IsEnOutReset;
	BOOL    IsEnDebug;
	UCHAR   UserMem;              //multiplexed as LDO output voltage parameters at CH543
	UCHAR   Baud[4];
}CH55xIspOptionS,*PCH55xIspOptionS;


#pragma pack()

#define		CH375_DEVICE_ARRIVAL		3		//MCU plugging event, already plugginged
#define		CH375_DEVICE_REMOVE			0		//MCU unplugging event, already unplugged

/*
USB download can realize automatic download after plugging in and multiple devices at once
USB download steps:
1.Set ISP download settings, set download interface, chip type, must be set before all operations
  WCH55x_SetIspOption,set ISP download settings
  needs to be downloaded automatically when plugging in
  need to be set in advance:CH55xIspOptionS->IsEnableUsbPnpNotify
2.Search for ISP download devices via USB, return the number of devices enumerated, and save the returned IspDevInfor device array information
  WCH55x_EnumDevices
3.Write the target file data to mcu flash, and specify the device serial number.
  WCH55x_FlashProgramB


Serial port download, can achieve automatic download after inserting a certain serial port
Serial port download steps
1.WCH55x_SetIspOption
  Set ISP download settings, set download interface, chip type, must be set before all operations

2.WCH55x_EnumDevices
  IspOption->IspInterface = 1:when downloading from serial port, this function will enumerate all the serial ports on the computer
  return the number of enumerated serial ports, and save the returned IspDevInfor device array information.

3.WCH55x_GetIspDeviceInfor
  Before downloading, you must call WCH55x_GetIspDeviceInfor function to get the information of the specified device, whether it is online or not, chip UID, etc.
  because the serial port does not support plug-and-play, it can only be obtained manually.
  IspDevInfor->DevIsOnline=TRUE indicates that a device has been detected connected to the interface

4.Write the target file data to mcu flash, and specify the device serial number.
  WCH55x_FlashProgramB
*/

//Set ISP parameters 
BOOL WCH55x_SetIspOption(CH55xIspOptionS *IspOption);	//MCU ISP parameter structure

//Get ISP parameters 
BOOL WCH55x_GetIspOption(CH55xIspOptionS *IspOption);	//MCU ISP parameter structure

//Write data at MCU EEPROM  
BOOL WCH55x_WriteDataFlash(ULONG            iIndex,				//MCU serial number
							 ULONG            StartAddr,		//start address
							 ULONG            *oWriteLen,		//write Length
							 PUCHAR           DataBuf,			//data buffer
                             BOOL             bIsErase);

//Read data at MCU EEPROM   
BOOL WCH55x_ReadDataFlash(ULONG            iIndex,				//MCU serial number
							ULONG            StartAddr,			//start Address
							ULONG            *oReadLen,			//write Length
							PUCHAR           DataBuf);			//data buffer

//Read configuration word
BOOL WCH55xIsp_ReadConfig(ULONG DevI,
						 PUCHAR IapCfgVal,		//4 bytes
						 PUCHAR IspCfgVal,		//4 bytes
						 PUCHAR CFlashCfgVal,	//4 bytes
						 PULONG BootVer,
						 PUCHAR UUID);

//Stop waiting for MCU to connect 
VOID WCH55x_StopOp();

//Close the specified MCU
VOID WCH55x_CloseDevice(ULONG DevI);

//Search MCU and return the enumerated serial number
//When the USB download mode, this function will perform a search for the ISP download MCU via USB, and return the number of MCU enumerated
//When serial port downloading mode, this function will enumerate all the serial ports on the computer, and return the number of serial ports enumerated.
ULONG WCH55x_EnumDevices(Isp55xDevInforS *IspDevInfor,				    //array of MCU information parameters
									 UCHAR            MaxDevCnt,    	//maximum number of scanned MCU<=16
									 UCHAR            *BtChipSeries, 	//the last MCU series searched for
									 UCHAR            *BtChipType,      //the last MCU type searched for
									 BOOL             *IsPreBTV230);	

//Get MCU information parameters(Only used at serial port download or verify mode)
BOOL WCH55x_GetIspDeviceInfor(ULONG iIndex,	//specify serial number MCU information
                						PIsp55xDevInforS IspDevInfor);	//device Information

//Download data at MCU codeflash
BOOL WCH55x_FlashProgramB(ULONG  iIndex,					    //MCU serial number
									PUCHAR UserFileDataBuf,		//user download file data
									ULONG  UserFileDataLen,		//user download file data length
									UCHAR  UserFileDateType,	//user download file format 0:HEX ,1:BIN 
									PUCHAR IAPFileDataBuf,		//IAP download file data
									ULONG  IAPFileDataLen,		//IAP download file data length
									UCHAR  IAPFileDateType);	//IAP download file format 0:HEX ,1:BIN 

//Verify data at MCU codeflash
BOOL WCH55x_FlashVerifyB(ULONG iIndex,					        //MCU serial number
									PUCHAR UserFileDataBuf,		//user Verify file data
									ULONG  UserFileDataLen,		//user Verify file data length
									UCHAR  UserFileDateType,	//user Verify file format 0:HEX ,1:BIN 
									PUCHAR IAPFileDataBuf,		//IAP Verify file data
									ULONG  IAPFileDataLen,		//IAP Verify file data length
									UCHAR  IAPFileDateType);	//IAP Verify file format 0:HEX ,1:BIN 

//Read OTP data 
BOOL WCH55x_ReadOTP(ULONG  iIndex,				    //MCU serial number
							  UCHAR  OffSet,		//OTP Offset Value
							  UCHAR  *DataBuf);		//OTP data

//Write OTP data 
BOOL WCH55x_WriteOTP(ULONG  iIndex,				    //MCU serial number
							  UCHAR  OffSet,		//OTP Offset Value
							  UCHAR  *DataBuf);		//OTP data

//Stop the download or verify operation 
BOOL WCH55x_IspEnd(ULONG iIndex,				    //MCU serial number
							  BOOL  bIsDlReset);	//reset MCU 

//Enable the simulation interface of CH56X/CH57X/CH58X
BOOL WCH55x_IspSumilat(ULONG iIndex);		//MCU serial number


//Disable the code-readprotect of CH32FXXX/CH32VXXX
BOOL WCH55x_IspRemoveProtect(ULONG iIndex);	//MCU serial number


//Get the progress of download or verify
UINT WCH55x_IspGetOptPrograss(ULONG iIndex); //MCU serial number

#ifdef __cplusplus
}
#endif

#endif		// _WCH55XISP_DLL_H
