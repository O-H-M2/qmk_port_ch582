# QMK

with wireless support in a single chip.

## Overview

This is a porting of QMK keyboard firmware for CH58x series, currently support CH582.

## Directory Structure

- CherryUSB, qmk_firmware, mcuboot: Components introduced as submodules, **NO any file** is modified. 

  *The qmk_firmware submodule should be able to keep up with upstream.*
- CherryUSB_porting, mcuboot_porting: Files used to configure submodules and add them into the building system.
- qmk_porting: The adapter layer for QMK to running on the CH58x chip.
- SDK: Directory for placing WCH SDK, so no IDE is used.

## Code Branch

- Master: Support for USB and ws2812 is done, still in development.
- debug: Used for presenting specific issues, ignore if use of QMK is all you want.

## Highlights

- BLE/USB/2.4G triple mode (in progress)
- Capability to update along with the QMK repo
- Low power comsumption in wireless modes.

## Hardware supporting

Currently only CH582M is tested.

## Building

- A WCH-specified toolchain is provided in this repo, if you want to use the public version, you may find it [here](https://xpack.github.io/blog/2019/07/31/riscv-none-embed-gcc-v8-2-0-3-1-released). Note that you need to add it to your `PATH` environment variable manually.
- *A global defination `INT_SOFT` is needed or the ISR handlers might not working properly.*

### Keyboard manufacturers/QMK firmware users

Fork this repository then manually upload you keyboard configuration file to [keyboards](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards). You'll be able to use the GitHub Actions to build your firmware online.

*Note that currently the configuration file is slightly different from the original QMK ones, you may take [this keyboard](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards/m2wired) as a start point.*

### Platform developers

Documentation about standard build & development flows using [Visual Studio Code](https://code.visualstudio.com/) can be found in [the development guide](./VSCODE_DEVELOPMENT.md).

## Flashing

End users：Use [Bootmagic Lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite) with `.uf2` only, or take your own risk of bricking your keyboard.

Developers：You may use the [flashing utility](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html).

## Community

[Discord](https://discord.gg/kaH6eRUFZS)

## 概述

这是QMK固件向CH58x平台的移植，主要工作集中在应用层（QMK）和底层硬件之间的接合。

## 目录结构

- CherryUSB, qmk_firmware, mcuboot: 子仓库，**没有修改任何代码**。

  *其中，QMK固件应当能够随上游仓库随时更新。*
- CherryUSB porting, mcuboot_porting: CherryUSB的配置文件。
- qmk_porting: QMK和硬件之间的接合层
- SDK: WCH的SDK, 目前版本V1.5。

## 分支说明

- Master: 完成了有线键盘所需的基本移植，包括VIA支持。灯目前只支持ws2812.
- debug: 如果你只是来看QMK的，当它是空气即可。

## 功能亮点

- 三模支持 （无线功能暂不开放）
- 可随QMK上游仓库随时更新，支持QMK的绝大多数功能
- 无线低功耗

## 硬件支持

目前只测试了CH582M，CH582F应当能够正常工作。

## 编译

- WCH的工具链已经随附，当然你也可以选择使用[公版编译器](https://xpack.github.io/blog/2019/07/31/riscv-none-embed-gcc-v8-2-0-3-1-released). 但需要你自行处理构建系统调用问题。
- *如果你确定要头铁，加一个全局宏定义`INT_SOFT`，否则中断很有可能不会正常工作*

### 键盘生产商/QMK固件用户

Fork我的仓库，手动将你的键盘配置文件上传到[keyboards](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards)目录下，然后使用页面上方的Actions来在线编译你的固件。

*需要注意本仓库目前使用的配置文件与QMK的有一点轻微差异，你可以用[这个](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards/m2wired)作为模板自行修改。*

### 开发者

推荐使用[Visual Studio Code](https://code.visualstudio.com/)。

参照[这个](./VSCODE_DEVELOPMENT.md)搭建你的本机开发环境，也可选择Codespace.

## 烧录

用户：不要使用除[Bootmagic Lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite)以外的方式。

开发者：推荐使用[WCH提供的工具](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html)。

## 还不会？

这有个交流群：860356332，欢迎加入。
