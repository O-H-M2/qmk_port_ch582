# CH582-CMake

摆脱eclipse编译环境，使用vscode编辑代码。

此仓库为cmake模板，请根据自己的工程进行适当修改。

请在CmakeLists.txt文件内更改`TOOLPATH`的变量为你的工具链路径。

```shell
A和B方法任选其一即可

A、使用cmake生成eclipse Makefile工程
cmake -G"Eclipse CDT4 - Unix Makefiles" ..
如果没有把make.exe加入系统path:
cmake -G"Eclipse CDT4 - Unix Makefiles" -D"你的路径/make.exe"  ..

B、使用cmake生成ninja工程
cmake -G"Ninja" ..
如果没有把ninja.exe加入系统path:
cmake -G"Ninja" -D"你的路径/ninja.exe" ..
```

