*Other languages: [English](README.md), [简体中文](README.zh-cn.md)*

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

End users: Use [Bootmagic Lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite) with `.uf2` only, or take your own risk of bricking your keyboard.

Developers: You may use the [flashing utility](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html).

## Community

[Discord](https://discord.gg/kaH6eRUFZS)
