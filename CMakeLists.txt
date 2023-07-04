# Copyright 2022 Huckies <https://github.com/Huckies>
# Copyright 2023 Jactry Zeng
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.20)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(QMK_BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/qmk_firmware" CACHE STRING "ENV" FORCE)
set(UF2_FAMILY 0x1aa143c7)

find_program(CCACHE_FOUND ccache)

if(CCACHE_FOUND)
    message(STATUS "Found ccache: ${CCACHE_FOUND}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_FOUND}")
endif(CCACHE_FOUND)

file(GLOB NRF_BIN_PATH "${CMAKE_CURRENT_LIST_DIR}/utils/nrf*/bin")
find_program(MERGEHEX_PATH NAMES mergehex PATHS ${NRF_BIN_PATH} REQUIRED)
message(STATUS "Found mergehex: ${MERGEHEX_PATH}")

set(TOOLCHAIN_PREFIX riscv-none-embed)
set(ADDITIONAL_TOOLCHAIN_PATH "${CMAKE_CURRENT_LIST_DIR}/utils/riscv-none-embed-gcc/bin/")

find_program(CMAKE_C_COMPILER NAMES ${TOOLCHAIN_PREFIX}-gcc PATHS ${ADDITIONAL_TOOLCHAIN_PATH} REQUIRED)
message(STATUS "Found ${TOOLCHAIN_PREFIX}-gcc: ${CMAKE_C_COMPILER}")
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
find_program(CMAKE_CXX_COMPILER NAMES ${TOOLCHAIN_PREFIX}-g++ PATHS ${ADDITIONAL_TOOLCHAIN_PATH} REQUIRED)
message(STATUS "Found ${TOOLCHAIN_PREFIX}-g++: ${CMAKE_CXX_COMPILER}")
find_program(CMAKE_AR NAMES ${TOOLCHAIN_PREFIX}-ar PATHS ${ADDITIONAL_TOOLCHAIN_PATH} REQUIRED)
message(STATUS "Found ${TOOLCHAIN_PREFIX}-ar: ${CMAKE_AR}")
find_program(CMAKE_OBJCOPY NAMES ${TOOLCHAIN_PREFIX}-objcopy PATHS ${ADDITIONAL_TOOLCHAIN_PATH} REQUIRED)
message(STATUS "Found ${TOOLCHAIN_PREFIX}-objcopy: ${CMAKE_OBJCOPY}")
find_program(CMAKE_OBJDUMP NAMES ${TOOLCHAIN_PREFIX}-objdump PATHS ${ADDITIONAL_TOOLCHAIN_PATH} REQUIRED)
message(STATUS "Found ${TOOLCHAIN_PREFIX}-objdump: ${CMAKE_OBJDUMP}")

project(qmk_port_ch582 C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

add_compile_options(-march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -std=gnu99)
add_compile_options(-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common)
add_compile_options(--param highcode-gen-section-name=1)
add_compile_options(-Wall -Wno-comment -Wno-enum-compare)
add_compile_options(-fdiagnostics-color=always)

if(NOT CMAKE_BUILD_TYPE)
    message(WARNING "CMAKE_BUILD_TYPE not set, default to 'Release'")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif()

execute_process(COMMAND git describe --always --dirty
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    TIMEOUT 60
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(COMMAND git describe --abbrev=0 --tags
    WORKING_DIRECTORY ${QMK_BASE_DIR}
    TIMEOUT 5
    OUTPUT_VARIABLE QMK_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT DEFINED QMK_BUILDDATE)
    execute_process(COMMAND date +"%Y-%m-%d-%H:%M:%S"
        TIMEOUT 5
        OUTPUT_VARIABLE QMK_BUILDDATE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

# allow for manual override
if(NOT DEFINED keyboard OR(NOT DEFINED keymap AND NOT BUILD_WIRELESS_LIB))
    include(keyboard_target.cmake)
endif()

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/qmk_porting/keyboards/${keyboard}/rules.cmake")
    set(keyboard_type "public" CACHE STRING "KB" FORCE)
    set(KEYBOARD_ROOT "${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}" CACHE STRING "KB" FORCE)

    if(BUILD_WIRELESS_LIB)
        message(STATUS "Building library ${keyboard}:${keymap}")
    else()
        message(STATUS "Compiling ${keyboard}:${keymap}")
    endif()

    include(qmk_porting/keyboards/${keyboard}/rules.cmake)
elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/qmk_porting/keyboards_private/${keyboard}/rules.cmake")
    set(keyboard_type "private" CACHE STRING "KB" FORCE)
    set(KEYBOARD_ROOT "${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}" CACHE STRING "KB" FORCE)

    if(BUILD_WIRELESS_LIB)
        message(STATUS "Building library ${keyboard}:${keymap} from private")
    else()
        message(STATUS "Compiling ${keyboard}:${keymap} from private")
    endif()

    include(qmk_porting/keyboards_private/${keyboard}/rules.cmake)
else()
    message(FATAL_ERROR "You must select a keyboard and its keymap!")
endif()

add_definitions(-DKEYMAP_C="keymap.c")
add_definitions(-D__GIT_VERSION__=${GIT_VERSION})
add_definitions(-DBOARD_UF2_FAMILY_ID=${UF2_FAMILY})

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

include(qmk_porting/qmk_porting.cmake)
include(qmk_porting/quantum.cmake)
include(CherryUSB_porting/CherryUSB.cmake)
include(qmk_porting/tmk_core.cmake)
include(qmk_porting/default_rules.cmake)
include(qmk_porting/common_features.cmake)
add_subdirectory(sdk)

# get_property(dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# message("include_dirs=${dirs}")
file(GLOB APP_SOURCES
    ${quantum_SOURCES}
    ${tmk_core_SOURCES}
    ${QMK_PORTING_SOURCES}
)
file(GLOB IAP_SOURCES
    ${QMK_PORTING_IAP_SOURCES}
)

add_library(mbedtls INTERFACE)
target_include_directories(mbedtls INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/mcuboot_porting"
    "${CMAKE_CURRENT_LIST_DIR}/mcuboot/ext/mbedtls/include"
    "${CMAKE_CURRENT_LIST_DIR}/mcuboot/ext/mbedtls/library"
)
file(GLOB mbedtls_sources
    "${CMAKE_CURRENT_LIST_DIR}/mcuboot/ext/mbedtls/library/[^x][^5]*.c"
)
target_sources(mbedtls INTERFACE
    ${mbedtls_sources}
)

if(NOT ESB_ENABLE AND ESB_ROLE STREQUAL "dongle")
    message(FATAL_ERROR "Invalid dongle role configuration, will skip.")
endif()

if(BUILD_WIRELESS_LIB)
    include(${WIRELESS_SUBMODULE_PATH}/gen_lib.cmake)
else()
    if(NOT ESB_ENABLE OR(ESB_ENABLE AND ESB_ROLE STREQUAL "keyboard"))
        set(WIRELESS_LIB_INCLUDE ${KEYBOARD_ROOT}/wireless/include)
        set(WIRELESS_LIB_FILE ${KEYBOARD_ROOT}/wireless/lib${keyboard}.a)
        set(HEX_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}.hex)
        set(HEX_FILE_APP_SIGNED ${PROJECT_BINARY_DIR}/${keyboard}_signed.hex)
        set(BIN_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}.bin)
        set(UF2_FILE_APP ${CMAKE_CURRENT_LIST_DIR}/${keyboard}_upgrade_${GIT_VERSION}.uf2)
        set(HEX_FILE_FACTORY ${CMAKE_CURRENT_LIST_DIR}/${keyboard}_factory_${GIT_VERSION}.hex)
        set(HEX_FILE_IAP ${PROJECT_BINARY_DIR}/${keyboard}_IAP.hex)
        set(BIN_FILE_IAP ${PROJECT_BINARY_DIR}/${keyboard}_IAP.bin)
    else()
        set(WIRELESS_LIB_INCLUDE ${KEYBOARD_ROOT}/wireless/include)
        set(WIRELESS_LIB_FILE ${KEYBOARD_ROOT}/wireless/lib${keyboard}_dongle.a)
        set(HEX_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}_dongle.hex)
        set(HEX_FILE_APP_SIGNED ${PROJECT_BINARY_DIR}/${keyboard}_dongle_signed.hex)
        set(BIN_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}_dongle.bin)
        set(UF2_FILE_APP ${CMAKE_CURRENT_LIST_DIR}/${keyboard}_dongle_upgrade_${GIT_VERSION}.uf2)
        set(HEX_FILE_FACTORY ${CMAKE_CURRENT_LIST_DIR}/${keyboard}_dongle_factory_${GIT_VERSION}.hex)
        set(HEX_FILE_IAP ${PROJECT_BINARY_DIR}/${keyboard}_dongle_IAP.hex)
        set(BIN_FILE_IAP ${PROJECT_BINARY_DIR}/${keyboard}_dongle_IAP.bin)
    endif()

    add_library(mcuboot_porting INTERFACE)
    target_include_directories(mcuboot_porting INTERFACE
        "${CMAKE_CURRENT_LIST_DIR}/mcuboot_porting"
        "${CMAKE_CURRENT_LIST_DIR}/mcuboot/boot/bootutil/include"
        "${CMAKE_CURRENT_LIST_DIR}/mcuboot/boot/bootutil/src"
    )
    file(GLOB bootutil_sources
        "${CMAKE_CURRENT_LIST_DIR}/mcuboot/boot/bootutil/src/*.c"
    )
    target_sources(mcuboot_porting INTERFACE
        ${bootutil_sources}
        "${CMAKE_CURRENT_LIST_DIR}/mcuboot_porting/mcuboot_port.c"
    )
    target_link_libraries(mcuboot_porting INTERFACE mbedtls)
    target_link_libraries(mcuboot_porting INTERFACE CH582_IAP)

    add_executable(${keyboard}.elf ${APP_SOURCES})
    add_executable(${keyboard}_IAP.elf ${IAP_SOURCES})

    target_compile_definitions(${keyboard}.elf PUBLIC __BUILDING_APP__=1)
    target_compile_definitions(${keyboard}_IAP.elf PUBLIC __BUILDING_IAP__=1)

    target_link_libraries(${keyboard}.elf PUBLIC CH582_APP)

    # target_link_libraries(${keyboard}_IAP.elf PUBLIC CH582_IAP)
    target_link_libraries(${keyboard}_IAP.elf PUBLIC mcuboot_porting)

    target_link_options(${keyboard}.elf PRIVATE
        -march=rv32imac -mabi=ilp32
        -nostartfiles
        -Xlinker --gc-sections
        -Wl,--print-memory-usage
        -Wl,-Map,${keyboard}.map
        --specs=nano.specs
        --specs=nosys.specs
    )
    target_link_options(${keyboard}_IAP.elf PRIVATE
        -march=rv32imac -mabi=ilp32
        -nostartfiles
        -Xlinker --gc-sections
        -Wl,--print-memory-usage
        -Wl,-Map,${keyboard}_IAP.map
        --specs=nano.specs
        --specs=nosys.specs
    )

    target_precompile_headers(${keyboard}.elf PUBLIC
        ${KEYBOARD_ROOT}/qmk_config.h
    )
    target_precompile_headers(${keyboard}_IAP.elf PUBLIC
        ${KEYBOARD_ROOT}/qmk_config.h
    )

    if(EXISTS "${KEYBOARD_ROOT}/keymaps/${keymap}/qmk_config.h")
        message(STATUS "Found keymap specified configs.")
        target_precompile_headers(${keyboard}.elf PUBLIC
            ${KEYBOARD_ROOT}/keymaps/${keymap}/qmk_config.h
        )
        target_precompile_headers(${keyboard}_IAP.elf PUBLIC
            ${KEYBOARD_ROOT}/keymaps/${keymap}/qmk_config.h
        )
    endif()

    if(EXISTS "${KEYBOARD_ROOT}/mcuconf.h")
        target_precompile_headers(${keyboard}.elf PUBLIC
            ${KEYBOARD_ROOT}/mcuconf.h
        )
        target_precompile_headers(${keyboard}_IAP.elf PUBLIC
            ${KEYBOARD_ROOT}/mcuconf.h
        )
    endif()

    if(EXISTS "${KEYBOARD_ROOT}/halconf.h")
        target_precompile_headers(${keyboard}.elf PUBLIC
            ${KEYBOARD_ROOT}/halconf.h
        )
        target_precompile_headers(${keyboard}_IAP.elf PUBLIC
            ${KEYBOARD_ROOT}/halconf.h
        )
    endif()

    target_precompile_headers(${keyboard}.elf PUBLIC
        ${PROJECT_SOURCE_DIR}/qmk_porting/protocol/pre_handler.h
    )
    target_precompile_headers(${keyboard}_IAP.elf PUBLIC
        ${PROJECT_SOURCE_DIR}/qmk_porting/protocol/pre_handler.h
    )

    if(BLE_ENABLE OR(ESB_ENABLE AND((ESB_ROLE STREQUAL "keyboard") OR(ESB_ROLE STREQUAL "dongle"))))
        add_library(wireless_lib STATIC IMPORTED)
        set_property(TARGET wireless_lib PROPERTY IMPORTED_LOCATION ${WIRELESS_LIB_FILE})
        target_include_directories(wireless_lib INTERFACE ${WIRELESS_LIB_INCLUDE})

        target_link_libraries(${keyboard}.elf PUBLIC wireless_lib)
        target_link_libraries(${keyboard}_IAP.elf PUBLIC wireless_lib)
    endif()

    add_custom_command(TARGET ${keyboard}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${keyboard}.elf> ${HEX_FILE_APP}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${keyboard}.elf> ${BIN_FILE_APP}
    )
    add_custom_command(TARGET ${keyboard}_IAP.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${keyboard}_IAP.elf> ${HEX_FILE_IAP}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${keyboard}_IAP.elf> ${BIN_FILE_IAP}
    )

    if((BLE_ENABLE OR(ESB_ENABLE AND((ESB_ROLE STREQUAL "keyboard") OR(ESB_ROLE STREQUAL "dongle"))))
        AND WIRELESS_SUBMODULE_PATH
        AND EXISTS "${WIRELESS_SUBMODULE_PATH}/post_build.cmake")
        include(${WIRELESS_SUBMODULE_PATH}/post_build.cmake)
    else()
        add_custom_target(post_build ALL
            COMMAND python3 ${PROJECT_SOURCE_DIR}/mcuboot/scripts/imgtool.py sign --header-size 0x1000 --align 4 --slot-size 0x5e000 --version 1.0.0 --pad-header ${HEX_FILE_APP} ${HEX_FILE_APP_SIGNED}
            COMMAND python3 ${PROJECT_SOURCE_DIR}/utils/uf2conv.py ${HEX_FILE_APP_SIGNED} -c -f ${UF2_FAMILY} -o ${UF2_FILE_APP}
            COMMAND ${MERGEHEX_PATH} --merge ${HEX_FILE_APP_SIGNED} ${HEX_FILE_IAP} --output ${HEX_FILE_FACTORY}
            DEPENDS ${keyboard}.elf ${keyboard}_IAP.elf
        )
    endif()
endif()
