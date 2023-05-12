*其他语言：[English](README.md)，[简体中文](README.zh-cn.md)*

## 概述

这是 QMK 固件向 CH58x 平台的移植，主要工作集中在应用层（QMK）和底层硬件之间的接合。

## 目录结构

- CherryUSB、qmk_firmware、mcuboot：子仓库，**没有修改任何代码**。

  *其中，QMK 固件应当能够随上游仓库随时更新。*
- CherryUSB porting、mcuboot_porting：CherryUSB 的配置文件。
- qmk_porting：QMK 和硬件之间的接合层
- SDK：WCH 的 SDK，目前版本 V1.5。

## 分支说明

- Master：完成了有线键盘所需的基本移植，包括 VIA 支持。灯目前只支持 ws2812。
- debug：如果你只是来看 QMK 的，当它是空气即可。

## 功能亮点

- 三模支持（无线功能暂不开放）
- 可随 QMK 上游仓库随时更新，支持 QMK 的绝大多数功能
- 无线低功耗

## 硬件支持

目前只测试了 CH582M，CH582F 应当能够正常工作。

## 编译

- WCH 的工具链已经随附，当然你也可以选择使用[公版编译器](https://xpack.github.io/blog/2019/07/31/riscv-none-embed-gcc-v8-2-0-3-1-released)。但需要你自行处理构建系统调用问题。
- *如果你确定要头铁，加一个全局宏定义 `INT_SOFT`，否则中断很有可能不会正常工作*

### 键盘生产商/QMK 固件用户

Fork 我的仓库，手动将你的键盘配置文件上传到[keyboards](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards)目录下，然后使用页面上方的 Actions 来在线编译你的固件。

*需要注意本仓库目前使用的配置文件与 QMK 的有一点轻微差异，你可以用[这个](https://github.com/O-H-M2/qmk_port_ch582/tree/via/qmk_porting/keyboards/m2wired)作为模板自行修改。*

### 开发者

推荐使用 [Visual Studio Code](https://code.visualstudio.com/)。

参照[这个](./VSCODE_DEVELOPMENT.md)搭建你的本机开发环境，也可选择 Codespace.

## 烧录

用户：不要使用除 [Bootmagic Lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite) 以外的方式。

开发者：推荐使用 [WCH 提供的工具](http://www.wch.cn/downloads/WCHISPTool_Setup_exe.html)。

## 还不会？

这有个交流群：860356332，欢迎加入。
