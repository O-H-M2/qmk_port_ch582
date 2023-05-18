*其他语言：[English](README.md)，[简体中文](README.zh-cn.md)*

## 目录

- [概述](#概述)
- [目录结构](#目录结构)
- [分支说明](#分支说明)
- [功能亮点](#功能亮点)
- [硬件支持](#硬件支持)
- [编译](#编译)
  - [键盘生产商/QMK 固件用户](#键盘生产商qmk-固件用户)
  - [开发者](#开发者)
- [烧录](#烧录)
- [社区](#社区)

## 概述

这是 [QMK](https://github.com/qmk/qmk_firmware) 固件向 CH58x 平台的移植，主要工作集中在应用层（QMK）和底层硬件之间的接合。

## 目录结构

- CherryUSB、qmk_firmware、mcuboot：子仓库，**没有修改任何代码**。

  *其中，QMK 固件应当能够随上游仓库随时更新。*
- CherryUSB_porting、mcuboot_porting：用于配置子模块并将它们添加到构建系统中的文件。
- qmk_porting：QMK 和硬件之间的接合层。
- sdk：WCH 的 SDK。

## 分支说明

- via：完成了有线键盘所需的基本移植，包括 VIA 支持。灯方面，目前支持 WS2812（SPI 和 PWM）和 AW20216S（SPI）。
- debug：如果你只是来看 QMK 的，当它是空气即可。

## 功能亮点

- 支持有线、蓝牙、无线 2.4G（无线功能暂不开放）。
- 可随 QMK 上游仓库随时更新，支持 QMK 的绝大多数功能。
- 无线低功耗。

## 硬件支持

目前只测试了 CH582M，CH582F 应当能够正常工作。

## 编译

- WCH 的工具链已经随附，当然你也可以选择使用[公版编译器](https://xpack.github.io/blog/2019/07/31/riscv-none-embed-gcc-v8-2-0-3-1-released)。请记得将它的路径手动添加到环境变量 `PATH`。
- *如果你确定要头铁，加一个全局宏定义 `INT_SOFT`，否则中断很有可能不会正常工作*。

### 键盘生产商/QMK 固件用户

Fork 我的仓库，手动将你的键盘配置文件上传到 [keyboards](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards) 目录下，然后使用页面上方的 Actions 来在线编译你的固件。

*需要注意本仓库目前使用的配置文件与 QMK 的有一点轻微差异，你可以用[这个](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards/m2wired)作为模板自行修改。*

### 开发者

推荐使用 [Visual Studio Code](https://code.visualstudio.com/)。

参照[这个](./VSCODE_DEVELOPMENT.md)搭建你的本机开发环境，也可选择 Codespace.

或参照以下步骤在 Linux 系统上构建：

1. 安装编译器和 nrf 命令行工具：

    下载、配置 WCH 提供的[编译器](http://mounriver.com/download)和 [nrf 命令行工具](https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download)。

    - 基于 deb 的 Linux 发行版用户（Debian GNU/Linux、Ubuntu 或 Ubuntu on WSL2）：

        以下命令将从他们的官网下载他们，并解压到 `$HOME/.local/opt` 目录。

        ```bash
        $ wget http://file.mounriver.com/tools/MRS_Toolchain_Linux_X64_V170.tar.xz
        $ wget https://nsscprodmedia.blob.core.windows.net/prod/software-and-other-downloads/desktop-software/nrf-command-line-tools/sw/versions-10-x-x/10-21-0/nrf-command-line-tools-10.21.0_linux-amd64.tar.gz
        $ mkdir -p $HOME/.local/opt
        $ tar xvf MRS_Toolchain_Linux_X64_V170.tar.xz -C $HOME/.local/opt
        $ tar xvf nrf-command-line-tools-10.21.0_linux-amd64.tar.gz -C $HOME/.local/opt
        ```

        然后，将这些程序添加到你的 `$PATH`。对于 `bash` 用户来说，通常可以通过这些命令完成：

        ```bash
        $ echo 'export PATH=$HOME/.local/opt/MRS_Toolchain_Linux_x64_V1.70/RISC-V\ Embedded\ GCC/bin/:$HOME/.local/opt/nrf-command-line-tools/bin/:$PATH' >> $HOME/.bashrc
        $ source $HOME/.bashrc
        ```

    - macOS 用户：

        先从[这里](https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download)下载、安装 nrf 命令行工具。然后使用以下命令下载安装编译器：

        ```bash
        $ curl -O http://file.mounriver.com/tools/MRS_Toolchain_MAC_V180.zip
        $ unzip MRS_Toolchain_MAC_V180.zip -d $HOME/.local/opt
        $ unzip $HOME/.local/opt/MRS_Toolchain_MAC_V180/xpack-riscv-none-embed-gcc-8.2.0.zip -d $HOME/.local/opt
        ```

        最后，将这些程序添加到你的 `$PATH`。对于 `bash` 用户来说，通常可以通过这些命令完成：

        ```bash
        $ echo 'export PATH=$HOME/.local/opt/xpack-riscv-none-embed-gcc-8.2.0/bin/:$PATH' >> $HOME/.bashrc
        $ source $HOME/.bashrc
        ```

2. 安装其他编译依赖：

    - 基于 deb 的 Linux 发行版用户：

        ```bash
        $ sudo apt update
        $ sudo apt install git cmake ccache python3 python3-click python3-cbor2 python3-intelhex
        ```

    - macOS 用户，假设已安装 XCode:

        ```bash
        $ pip3 install --user cryptography click cbor2 intelhex
        ```

3. Clone 代码仓库：

```bash
$ git clone https://github.com/O-H-M2/qmk_port_ch582.git
$ cd qmk_port_ch582
$ git -c submodule."qmk_porting/keyboards_private".update=none submodule update --recursive --init
```

4. 创建构建目录：

```bash
$ mkdir build
$ cd build
```

5. 运行 `cmake` 检查依赖和生成 Makefile

```bash
$ cmake -DCMAKE_BUILD_TYPE=Release -Dkeyboard=ezy64 -Dkeymap=default ..
```

你可以把 `ezy64` 和 `default` 替换成你的键盘和 keymap。

6. 编译：

```bash
$ make -j$(nproc)
```

如果编译成功，`.uf2` and `.hex` 会在项目的最顶层目录被生成。

## 烧录

用户：不要使用除 [Bootmagic Lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite) 以外的方式。

开发者：推荐使用 [WCH 提供的工具](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html)。

## 社区

- QQ 群：860356332
- [Discord](https://discord.gg/kaH6eRUFZS)
