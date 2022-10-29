# QMK

with wireless support in a single chip.

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
2. Install extension `CMake` and `Cmake Tools`.
3. Check CMake configurations on status bar which you can find at the bottom of you GUI:

- Check `build type` to `Release`
- Check `active kit` to `No active kit` if you are using the WCH toolchain, or to the corresponding toolchain you have downloaded.

4. Click the `Build` button.
5. Firmwares will be generated to the root directory in the name of your keyboard. **Choose `xxx_upgrade.uf2` if you are the end user, `xxx_factory.hex` is for developers only.**

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

克隆我的仓库，使用`git clone --recursive https://github.com/Huckies/qmk_port_ch582.git`

**无线部分子仓库未完成，可以通过从.gitmodules移除相关仓库来克隆并编译有线部分**

- WCH的工具链已经随附，当然你也可以选择使用[公版编译器](https://xpack.github.io/blog/2019/07/31/riscv-none-embed-gcc-v8-2-0-3-1-released). 但需要你自行加进环境变量。
- *如果你确定要头铁，加一个全局宏定义`INT_SOFT`，否则中断很有可能不会正常工作*

### 环境配置

#### macOS

1. 安装`Homebrew`
2. 打开终端，运行

```
brew install cmake ninja
```

3. 安装[VS Code](https://code.visualstudio.com/#alt-downloads)

#### Linux

TODO

#### Windows

如果你很熟悉`MSYS2`(或者`Cygwin`, **WSL2除外**)，你应该可以轻易配好环境，需要安装的东西包括`ninja`，`cmake`和`python`，注意msys2里的cmake版本很老，请百度解决方法。

新手上路：安装[CMake](https://cmake.org/download/)，[ninja](https://github.com/ninja-build/ninja/releases)，[Python 3](https://www.python.org/getit/)和[VS Code](https://code.visualstudio.com/#alt-downloads)。其中ninja只需要把其所在的目录添加进环境变量即可。

### 编译

1. 在VS Code里打开本仓库的文件夹
2. 安装插件`CMake`和`Cmake Tools`
3. 在底部状态栏上寻找

- `build type` 并将其设置为 `Release`
- `active kit` 并将其设置为 `No active kit`
- *如果你选择了公版编译器，需要找到对应的名字并选择*

3. 点击 `Build` .
4. 固件会以你键盘的名字命名并生成在本仓库的根目录下。 **对于用户来说，强烈推荐使用xxx_upgrade.uf2，否则你很有可能把键盘刷成砖。**

### 烧录

开发者：推荐使用[WCH提供的工具](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html)。

最终用户：不要使用除[Bootmagic Lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite)以外的方式。

### 详细教程

项目尚未完工，详细的步骤之后再补，这有个交流群：860356332，随缘解答。
