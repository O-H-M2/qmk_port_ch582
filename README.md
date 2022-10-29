# QMK

`with wireless support in a single chip.

## Overview

This is a porting of QMK keyboard firmware for CH58x series, currently support CH582.

## Directory Structure

- CherryUSB, qmk_firmware: Components introduced as submodules, **NO any file** is modified. 

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

## Building

First of all, clone this repo recursively using `git clone --recursive https://github.com/Huckies/qmk_port_ch582.git`

- A WCH-specified toolchain is provided in this repo, if you want to use the public version, you may find it [here](https://xpack.github.io/blog/2019/07/31/riscv-none-embed-gcc-v8-2-0-3-1-released). Note that you need to add it to your `PATH` environment variable manually.
- *A global defination `INT_SOFT` is needed or the ISR handlers might not working properly.*

### Environment setup

#### macOS

1. Install `Homebrew` if you haven't.
2. Open your terminal and run

```
brew install cmake ninja
```

3. Install [VS Code](https://code.visualstudio.com/#alt-downloads)

#### Linux

TODO

#### Windows

If you are familiar enough with the `MSYS2` subsystem (or `Cygwin`, but **NO WSL2**), you should be able to figure it out without many efforts. I'm only leaving a simple tutorial here.

Install [CMake](https://cmake.org/download/), [ninja](https://github.com/ninja-build/ninja/releases), [Python 3](https://www.python.org/getit/) and [VS Code](https://code.visualstudio.com/#alt-downloads).

### Compiling

1. Open this repository with VS Code.
2. Check CMake configurations on status bar which you can find at the bottom of you GUI:

- Check `build type` to `Release`
- Check `active kit` to `No active kit` if you are using the WCH toolchain, or to the corresponding toolchain you have downloaded.

3. Click the `Build` button.
4. Firmwares will be generated to the root directory in the name of your keyboard. **Choose `xxx_upgrade.uf2` if you are the end user, `xxx_factory.hex` is for developers only.**

## Flashing

Developers：You may use the [flashing utility](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html).

End users：Use [Bootmagic Lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite) with `.uf2` only, or take your own risk of bricking your keyboard.

## 概述

这是QMK固件向CH58x平台的移植，主要工作集中在应用层（QMK）和底层硬件之间的接合。

## 目录结构

- CherryUSB, qmk_firmware: 子仓库，**没有修改任何代码**。

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
- 你可以使用[公版编译器](https://xpack.github.io/blog/2019/07/31/riscv-none-embed-gcc-v8-2-0-3-1-released)，但我更推荐使用[WCH提供的版本](http://www.mounriver.com/download)，它支持一些独有的特性，比方说硬件压栈。
- *如果你一定要头铁，在主CMakeLists.txt里加一个全局的宏定义`INT_SOFT`，否则会报编译警告，并且中断回调很可能无法正常工作。*

再安装`CMake`和`Ninja`. 同时我推荐使用`VS Code`及其Cmake扩展，配置完工具链后就可以使用图形界面了。
- 如果你是用的是`VS Code`，那么只需要在下面的状态栏把工具链配置成`GCC 8.2.0 riscv-none-embed-gcc`并按下边上的`Build`
- 如果你坚持使用CLI，试着敲

```
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_C_COMPILER:FILEPATH=riscv-none-embed-gcc -DCMAKE_CXX_COMPILER:FILEPATH=riscv-none-embed-g++ -S. -B./build -G Ninja

cmake --build ./build --config Release --target all -- -j 4
```

固件会生成在本仓库的根目录下。

Windows下推荐使用MSYS2搭配官方的windows工具链，**不要使用WSL和Linux工具链**。

**对于用户而言，只可搭配bootmagic使用.uf2文件来升级，否则你很有可能把键盘刷成砖。**

### 烧录

开发者：推荐使用[WCH提供的工具](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html)。

最终用户：不要使用除[Bootmagic Lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite)以外的方式。

### 详细教程

项目尚未完工，详细的步骤之后再补，这有个交流群：860356332，随缘解答。
