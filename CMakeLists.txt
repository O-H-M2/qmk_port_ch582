set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.20)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 项目设置
project(qmk_port_ch582 C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

set(keyboard "mk01" CACHE STRING "QMK" FORCE)

if(NOT keyboard)
    message(FATAL_ERROR "You must select a keyboard!")
endif()

# add_definitions(-DQMK_KEYBOARD_H="${keyboard}.h")
if(NOT keymap)
    message(WARNING "No keymap defined, will use default.")
    set(keymap "default" CACHE STRING "QMK" FORCE)
endif()

# 编译参数 一般不用改
add_compile_options(-march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -std=gnu99)
add_compile_options(-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common)
add_compile_options(-Wall -Wno-comment -Wno-enum-compare)

# add_compile_options(-Wno-unused-function -Wno-unused-variable -Wno-type-limits)
add_compile_options(-fdiagnostics-color=always)

# 编译等级
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif()

function(include_sub_directories_recursively root_dir)
    if(IS_DIRECTORY ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*)

    foreach(sub ${ALL_SUB})
        if(IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub})
        endif()
    endforeach()
endfunction()

include(qmk_porting/keyboards/${keyboard}/rules.cmake)
include(qmk_porting/qmk_porting.cmake)
include(qmk_porting/quantum.cmake)
include(sdk/riscv.cmake)
include(CherryUSB_porting/CherryUSB.cmake)
include(qmk_porting/tmk_core.cmake)
include(qmk_porting/common_features.cmake)

# get_property(dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# message("include_dirs=${dirs}")

# 宏定义
# add_definitions(-include qmk_config.h)

# 源码文件
file(GLOB APP_SOURCES
    ${CH582_SOURCES}
    ${quantum_SOURCES}
    ${tmk_core_SOURCES}
    ${QMK_PORTING_SOURCES}
)
file(GLOB IAP_SOURCES
    ${CH582_SOURCES}
    ${CH582_IAP_SOURCES}
)

# 编译可执行文件
add_executable(${keyboard}.elf ${APP_SOURCES} ${LINKER_SCRIPT_APP})
add_executable(${keyboard}_IAP.elf ${IAP_SOURCES} ${LINKER_SCRIPT_IAP})

target_link_options(${keyboard}.elf PRIVATE
    -nostartfiles
    -Xlinker --gc-sections
    -Wl,--print-memory-usage
    -Wl,-Map,${keyboard}.map
    --specs=nano.specs
    --specs=nosys.specs
    -T ${LINKER_SCRIPT_APP}
)
target_link_options(${keyboard}_IAP.elf PRIVATE
    -nostartfiles
    -Xlinker --gc-sections
    -Wl,--print-memory-usage
    -Wl,-Map,${keyboard}_IAP.map
    --specs=nano.specs
    --specs=nosys.specs
    -T ${LINKER_SCRIPT_IAP}
)

target_precompile_headers(${keyboard}.elf PRIVATE
    ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/qmk_config.h
)
target_precompile_headers(${keyboard}_IAP.elf PRIVATE
    ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/qmk_config.h
)

# 链接静态库
target_link_libraries(${keyboard}.elf ${CH582_LIB})
target_link_libraries(${keyboard}_IAP.elf ${CH582_LIB})

if(BLE_ENABLE OR(ESB_ENABLE AND((ESB_ROLE STREQUAL "keyboard") OR(ESB_ROLE STREQUAL "dongle"))))
    add_subdirectory(${PROJECT_SOURCE_DIR}/qmk_porting/platforms/ch58x/wireless/lwrb)
    target_link_libraries(${keyboard}.elf lwrb)
    target_link_libraries(${keyboard}_IAP.elf lwrb)
endif()

# 输出hex和bin
set(HEX_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}.hex)
set(BIN_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}.bin)
add_custom_command(TARGET ${keyboard}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${keyboard}.elf> ${HEX_FILE_APP}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${keyboard}.elf> ${BIN_FILE_APP}
)
set(HEX_FILE_IAP ${PROJECT_BINARY_DIR}/${keyboard}_IAP.hex)
set(BIN_FILE_IAP ${PROJECT_BINARY_DIR}/${keyboard}_IAP.bin)
add_custom_command(TARGET ${keyboard}_IAP.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${keyboard}_IAP.elf> ${HEX_FILE_IAP}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${keyboard}_IAP.elf> ${BIN_FILE_IAP}
)