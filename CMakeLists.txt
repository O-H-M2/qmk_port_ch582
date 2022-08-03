set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.20)

if (APPLE)
    add_definitions(-DINT_SOFT)
endif ()
set(CMAKE_C_COMPILER riscv-none-embed-gcc)
set(CMAKE_CXX_COMPILER riscv-none-embed-g++)
set(CMAKE_ASM_COMPILER riscv-none-embed-gcc)
set(CMAKE_AR riscv-none-embed-ar)
set(CMAKE_OBJCOPY riscv-none-embed-objcopy)
set(CMAKE_OBJDUMP riscv-none-embed-objdump)
set(SIZE riscv-none-embed-size)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 项目设置
project(qmk_port_ch582 C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

# 编译参数 一般不用改
add_compile_options(-march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -std=gnu99)
add_compile_options(-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common)

# 编译等级
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*)
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub})
        endif()
    endforeach()
endfunction()

include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/qmk_firmware/quantum)

include_directories(BLE_LIB 
                    USB_LIB
                    RVMSIS  
                    StdPeriphDriver/inc
                    APP/inc
                    HAL/include

                    CherryUSB_porting
                    CherryUSB/class/hid
                    CherryUSB/class/printer
                    CherryUSB/common
                    CherryUSB/core
                    CherryUSB/port/ch32

                    qmk_firmware
                    qmk_firmware/lib/printf

                    tmk_core/protocol

                    # qmk_firmware/tmk_core/protocol
                    )

include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/qmk_firmware/tmk_core)
include_sub_directories_recursively(${PROJECT_SOURCE_DIR}/qmk_porting)

# get_property(dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# message("include_dirs=${dirs}")

# 宏定义
add_definitions(-DDEBUG=1)
# add_definitions(-DDCDC_ENABLE=1 -DHAL_SLEEP=1)
add_definitions(-DMATRIX_ROWS=5 -DMATRIX_COLS=14 -DMOUSE_ENABLE=1 -DRAW_ENABLE=1)
add_definitions(-DRGBLED_NUM=61 -DEEPROM_ENABLE=1 -DRGBLIGHT_ENABLE=1 -DRGBLIGHT_USE_TIMER=1 -DCOMMAND_ENABLE=1)
add_definitions(-DQMK_VERSION="0.0.1" -DQMK_BUILDDATE="Today")
add_definitions(-DQMK_KEYBOARD_H="mk02.h")
add_definitions(-DBOOTMAGIC_LITE_ROW=1 -DBOOTMAGIC_LITE_COLUMN=1)

# 源码文件
file(GLOB_RECURSE SOURCES 
                "Startup/*.S" 
                "USB_LIB/*.c"
                "RVMSIS/*.c"
                "StdPeriphDriver/*.c" 
                "APP/*.c"
                "HAL/*.c"

                "CherryUSB_porting/usb_porting.c"
                "CherryUSB/core/usbd_core.c"
                "CherryUSB/port/ch32/usb_dc_ch58x.c"
                "CherryUSB/class/hid/usbd_hid.c"

                "qmk_firmware/lib/printf/*.c"

                # "qmk_firmware/quantum/audio/*.c"
                # "qmk_firmware/quantum/backlight/*.c"
                "qmk_firmware/quantum/bootmagic/*.c"
                "qmk_firmware/quantum/debounce/sym_eager_pk.c"
                # "qmk_firmware/quantum/debounce/asym_eager_defer_pk.c"
                # "qmk_firmware/quantum/encoder/*.c"
                "qmk_firmware/quantum/keymap_extras/*.c"
                # "qmk_firmware/quantum/led_matrix/*.c"
                "qmk_firmware/quantum/logging/*.c"
                # "qmk_firmware/quantum/painter/*.c"
                "qmk_firmware/quantum/process_keycode/process_rgb.c"
                # "qmk_firmware/quantum/rgb_matrix/*.c"
                "qmk_firmware/quantum/rgblight/*.c"
                # "qmk_firmware/quantum/sequencer/*.c"
                # "qmk_firmware/quantum/split_common/*.c"
                "qmk_firmware/quantum/action_layer.c"
                "qmk_firmware/quantum/action_tapping.c"
                "qmk_firmware/quantum/action_util.c"
                "qmk_firmware/quantum/action.c"
                "qmk_firmware/quantum/bitwise.c"
                "qmk_firmware/quantum/caps_word.c"
                "qmk_firmware/quantum/color.c"
                "qmk_firmware/quantum/command.c"
                # "qmk_firmware/quantum/crc.c"
                "qmk_firmware/quantum/deferred_exec.c"
                # "qmk_firmware/quantum/digitizer.c"
                # "qmk_firmware/quantum/dip_switch.c"
                # "qmk_firmware/quantum/dynamic_keymap.c"
                "qmk_firmware/quantum/eeconfig.c"
                # "qmk_firmware/quantum/encoder.c"
                # "qmk_firmware/quantum/haptic.c"
                # "qmk_firmware/quantum/joystick.c"
                "qmk_firmware/quantum/keyboard.c"
                "qmk_firmware/quantum/keycode_config.c"
                "qmk_firmware/quantum/keymap_common.c"
                "qmk_firmware/quantum/led_tables.c"
                "qmk_firmware/quantum/led.c"
                "qmk_firmware/quantum/main.c"
                "qmk_firmware/quantum/matrix_common.c"
                "qmk_firmware/quantum/matrix.c"
                "qmk_firmware/quantum/mousekey.c"
                "qmk_firmware/quantum/pointing_device_drivers.c"
                "qmk_firmware/quantum/pointing_device.c"
                "qmk_firmware/quantum/programmable_button.c"
                "qmk_firmware/quantum/quantum.c"
                # "qmk_firmware/quantum/secure.c"
                "qmk_firmware/quantum/send_string.c"
                "qmk_firmware/quantum/sync_timer.c"
                # "qmk_firmware/quantum/utf8.c"
                # "qmk_firmware/quantum/variable_trace.c"
                # "qmk_firmware/quantum/velocikey.c"
                # "qmk_firmware/quantum/via.c"
                # "qmk_firmware/quantum/wpm.c"

                # "qmk_firmware/tmk_core/*.c"
                "qmk_firmware/tmk_core/protocol/host.c"
                "qmk_firmware/tmk_core/protocol/report.c"
                # "qmk_firmware/tmk_core/protocol/serial_soft.c"
                # "qmk_firmware/tmk_core/protocol/serial_uart.c"
                # "qmk_firmware/tmk_core/protocol/usb_descriptor.c"
                "qmk_firmware/tmk_core/protocol/usb_device_state.c"
                # "qmk_firmware/tmk_core/protocol/usb_util.c"
                
                "qmk_porting/*.c"
                )

list(APPEND SOURCES
                "qmk_firmware/tmk_core/protocol/usb_util.c"
                )

list(REMOVE_ITEM SOURCES
                "${PROJECT_SOURCE_DIR}/StdPeriphDriver/CH58x_usbdev.c"
                "${PROJECT_SOURCE_DIR}/qmk_firmware/quantum/process_keycode/process_joystick.c"
                "${PROJECT_SOURCE_DIR}/qmk_firmware/quantum/process_keycode/process_printer.c"
                "${PROJECT_SOURCE_DIR}/qmk_firmware/quantum/process_keycode/process_printer_bb.c"
                )

# message("include_dirs=${SOURCES}")

# 链接参数
set(LINKER_SCRIPT  ${CMAKE_SOURCE_DIR}/Ld/Link.ld)
add_link_options(-nostartfiles 
                -Xlinker --gc-sections  
                -Wl,--print-memory-usage
                -Wl,-Map,${PROJECT_NAME}.map 
                --specs=nano.specs 
                --specs=nosys.specs)
add_link_options(-T ${LINKER_SCRIPT})

# 编译可执行文件
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

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
