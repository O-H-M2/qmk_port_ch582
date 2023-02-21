# WCHISPTool_CMD 
ISP Command Tool for WCH MCU

WCHISPTool_CMD is used to operate WCH MCU, which supports flash erase/verify/download, dataflash erase/download etc.

The Linux directory contains src, driver, lib and bin directories.

src: source file corresponding to command tool

driver: device driver for USB download method

lib: ISP development dynamic library and related header files

bin: executable target program

The bin/WCHISPTool_CMD can quickly experience the use of the WCHISPTool_CMD tool on Linux.

Use the command line tool as described in the Linux Introduction section of documentation.



### Development Overview

1. Copy the dynamic library file from lib directory to System default library location "/usr/lib".

2. ISPCmdTool.cpp in the src directory is the source code of this command tool, users could modify it directly or create a new project. The development functions and instructions were all included in the header files WCH55XISPDLL.H etc. 

3. Use g++ toolchain to compile the source file in Step2.

   exp: g++ IspCmdTool.cpp -o WCHISPTool_CMD -lwch55xisp

   when the compilation instruction is executed successfully, users could operate with the new program, at the same time, please note that it needs to run in root mode by default.

