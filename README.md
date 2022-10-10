# qmk_port_ch582

## Overview

This is a porting of QMK keyboard firmware for CH58x series, currently support CH582.

## Directory Structure

- CherryUSB, lwrb, qmk_firmware: Components introduced as submodules, **NO any file** is modified. 

  *The qmk_firmware submodule should be able to keep up with upstream.*
- CherryUSB porting: Files used to add CherryUSB submodule into the building system.
- qmk_porting: The adapter layer for QMK to running on the CH58x chip.
- SDK: Directory for placing WCH SDK, so no IDE is needed.
- template: TODO

## Code Branch

- Master: Support for USB and ws2812 is done, still in development.
- debug: Used for presenting specific issues, ignore if use of QMK is all you want.

## Highlights

- BLE/USB/2.4G triple mode (in progress)
- Capability to update along with the QMK repo
- Low power comsumption in wireless modes.

## Hardware supporting

Currently only CH582M is tested.

## Compile

Firstly, clone this repo recursively using `git clone --recursive https://github.com/Huckies/qmk_port_ch582.git`

And then, get `riscv-none-embed-gcc` prepared, it should be added to your `PATH` environment variable. 

- While you can use the official toolchain from [here](https://github.com/riscv-collab/riscv-gnu-toolchain), the WCH version is recommended so you can benefit from hardware features like fast interrupt which can be downloaded from [here](http://www.mounriver.com/download).
- *If you insist on using the official one, add a global macro `INT_SOFT` or ISR handlers might not working properly.*

Then install `CMake` and `Ninja`. I also recommend you to use `VS Code` with its cmake extension as they provide a GUI over the command line interface.

## Flashing

Currently you could use the official [flasing utility](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html).

## 概述

这是QMK固件向CH58x平台的移植，主要工作集中在应用层（QMK）和底层硬件之间的接合。

## 目录结构

- CherryUSB, lwrb, qmk_firmware: 子仓库，**没有修改任何代码**。

  *其中，QMK固件应当能够随上游仓库随时更新。*
- CherryUSB porting: CherryUSB的配置文件。
- qmk_porting: QMK和硬件之间的接合层
- SDK: WCH的SDK, 目前版本V1.5。
- template: 新键盘的模板，待做。

## 分支说明

- Master: 完成了有线键盘所需的基本移植，包括VIA支持。灯目前只支持ws2812.
- debug: 如果你只是来看QMK的，当它是空气即可。

## 功能亮点

- 三模支持 （无线功能还在写）
- 可随QMK上游仓库随时更新，支持QMK的绝大多数功能
- 无线低功耗

## 硬件支持

目前只测试了CH582M，CH582F应当能够正常工作。

## 编译

首先克隆本仓库以及所有子仓库`git clone --recursive https://github.com/Huckies/qmk_port_ch582.git`

**无线部分子仓库未完成，可以通过从.gitmodules移除相关仓库来克隆并编译有线部分**

然后，下载`riscv-none-embed-gcc`编译器，并将它加入环境变量。
- 你可以使用[公版编译器](https://github.com/riscv-collab/riscv-gnu-toolchain)，但我更推荐使用[WCH提供的版本](http://www.mounriver.com/download)，它支持一些独有的特性，比方说硬件压栈。
- *如果你一定要头铁，在主CMakeLists.txt里加一个全局的宏定义`INT_SOFT`，否则会报编译警告，并且中断回调很可能无法正常工作。*

再安装`CMake`和`Ninja`. 同时我推荐使用`VS Code`及其Cmake扩展，配置完工具链后就可以使用图形界面了。

Windows下推荐使用MSYS2搭配官方的windows工具链，**不要使用WSL和Linux工具链**。

### 烧录

目前推荐使用[WCH提供的工具](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html)。

### 详细教程

项目尚未完工，详细的步骤之后再补，这有个交流群：860356332，随缘解答。
