*Other languages: [English](README.md), [简体中文](README.zh-cn.md)*

## Table of Contents

- [Overview](#overview)
- [Directory Structure](#directory-structure)
- [Code Branches](#code-branches)
- [Highlights](#highlights)
- [Supported hardware](#supported-hardware)
- [Building](#building)
  - [Keyboard manufacturers/QMK firmware users](#keyboard-manufacturersqmk-firmware-users)
  - [Developers](#developers)
    - [Debian GNU/Linux or Ubuntu](#debian-gnulinux-or-ubuntu)
- [Flashing](#flashing)
- [Community](#community)

## Overview

This is a porting of [QMK](https://github.com/qmk/qmk_firmware) keyboard firmware for CH58x series, with the main focus on bridging the application layer (QMK) and the underlying hardware.

## Directory Structure

- CherryUSB, qmk_firmware, mcuboot: Components introduced as submodules, **NO any file** is modified.

  *The qmk_firmware submodule should be able to keep up with upstream.*
- CherryUSB_porting, mcuboot_porting: Files used to configure submodules and add them into the building system.
- qmk_porting: The adapter layer for QMK to running on the CH58x chip.
- sdk: Directory for placing WCH SDK.

## Code Branches

- via: All the basic functions needed by wired keyboards are done, including VIA support. For lights, WS2812 (both SPI and PWM driven) and AW20216S (SPI driven) are supported currently.
- debug: Used for presenting specific issues, ignore if use of QMK is all you want.

## Highlights

- Support wired, Bluetooth, and triple mode 2.4G (WIP).
- Capability to update along with the QMK repo, and supports most of the features of QMK.
- Low power comsumption in wireless modes.

## Supported hardware

Currently only CH582M is tested, but it should also work for CH582F.

## Building

- A WCH-specified toolchain is provided in this repo, if you want to use the public version, you may find it [here](https://xpack.github.io/blog/2019/07/31/riscv-none-embed-gcc-v8-2-0-3-1-released). Note that you need to add it to your `PATH` environment variable manually.
- *A global defination `INT_SOFT` is needed or the ISR handlers might not working properly.*

### Keyboard manufacturers/QMK firmware users

Fork this repository then manually upload you keyboard configuration file to [keyboards](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards). You'll be able to use the GitHub Actions to build your firmware online.

*Note that currently the configuration file is slightly different from the original QMK ones, you may take [this keyboard](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards/m2wired) as a start point.*

### Developers

[Visual Studio Code](https://code.visualstudio.com/) is recommended.

You can follow this [guide](./VSCODE_DEVELOPMENT.md) to set up a development environment locally. Or you can also use Codespace instead.

Or follow these steps below to build it on your system:

#### Debian GNU/Linux or Ubuntu

1. Install some dependencies:

```
$ sudo apt update
$ sudo apt install git cmake ccache python3 python3-click python3-cbor2 python3-intelhex
```

2. Clone this repository by:
```
$ git clone https://github.com/O-H-M2/qmk_port_ch582.git
$ cd qmk_port_ch582
$ git -c submodule."qmk_porting/keyboards_private".update=none submodule update --recursive --init
```

3. Create a directory for the building:
```
$ mkdir build
$ cd build
```

4. Running cmake for dependenies checking and generating Makefile:
```
$ cmake -Dkeyboard=ezy64 -Dkeymap=default ..
```
You may replace `ezy64` and `default` in the above command with the names of your own keyboard and keymap.

5. Build:
```
$ make -j$(nproc)
```
`.uf2` and `.hex` will be generated in the top directory of the project if the build succeeds.


## Flashing

For end users: Use [Bootmagic Lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite) with `.uf2` only, or take your own risk of bricking your keyboard.

For developers: You may use the [flashing utility](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html) which is provided by WCH.

## Community

- QQ group: 860356332
- [Discord](https://discord.gg/kaH6eRUFZS)
