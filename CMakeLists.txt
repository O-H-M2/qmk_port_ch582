set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
cmake_minimum_required(VERSION 3.20)

# 工具链设置
set(TOOLPATH /Users/huckies/wch-toolchain/wch-riscv-gnu-toolchain/bin/riscv-none-embed-)

if(WIN32)
    MESSAGE(STATUS "Now is windows!")
    set(CMAKE_C_COMPILER ${TOOLPATH}gcc.exe)
    set(CMAKE_CXX_COMPILER ${TOOLPATH}g++.exe)
    set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc.exe)
    set(CMAKE_AR ${TOOLPATH}ar.exe)
    set(CMAKE_OBJCOPY ${TOOLPATH}objcopy.exe)
    set(CMAKE_OBJDUMP ${TOOLPATH}objdump.exe)
    set(SIZE ${TOOLPATH}size.exe)
elseif(UNIX)
    MESSAGE(STATUS "Now is UNIX-like OS!")
    set(CMAKE_C_COMPILER ${TOOLPATH}gcc)
    set(CMAKE_CXX_COMPILER ${TOOLPATH}g++)
    set(CMAKE_ASM_COMPILER ${TOOLPATH}gcc)
    set(CMAKE_AR ${TOOLPATH}ar)
    set(CMAKE_OBJCOPY ${TOOLPATH}objcopy)
    set(CMAKE_OBJDUMP ${TOOLPATH}objdump)
    set(SIZE ${TOOLPATH}size)
else()
    MESSAGE(STATUS "Unsupported system!")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 项目设置
project(ch582-ninja C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

# 编译参数 一般不用改
add_compile_options(-march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -std=gnu99)
add_compile_options(-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common)

# 编译等级
add_compile_options(-O3)

# 编译信息等级

# 头文件路径
include_directories(BLE_LIB
    USB_LIB
    RVMSIS
    StdPeriphDriver/inc
    printf
    APP/include
    HAL/include
    Profile/include
)

# 宏定义
add_definitions(-DDEBUG=1 -DHAL_SLEEP=1)

# 源码文件
file(GLOB_RECURSE SOURCES
    "Startup/*.S"
    "USB_LIB/*.c"
    "RVMSIS/*.c"
    "StdPeriphDriver/*.c"
    "printf/printf.c"
    "APP/*.c"
    "HAL/*.c"
    "Profile/*.c")

# 链接参数
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/Ld/Link.ld)
add_link_options(-nostartfiles
    -Xlinker --gc-sections
    -Wl,--print-memory-usage
    -Wl,-Map,${PROJECT_NAME}.map
    --specs=nano.specs
    --specs=nosys.specs)
add_link_options(-T ${LINKER_SCRIPT})

# 编译可执行文件
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

target_precompile_headers(${PROJECT_NAME}.elf PUBLIC ${PROJECT_SOURCE_DIR}/printf/printf.h)

# 链接静态库
target_link_libraries(${PROJECT_NAME}.elf
    ${CMAKE_SOURCE_DIR}/StdPeriphDriver/libISP583.a
    ${CMAKE_SOURCE_DIR}/BLE_LIB/LIBCH58xBLE.a
    ${CMAKE_SOURCE_DIR}/USB_LIB/libRV3UFI.a)

# 输出hex和bin
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
)
