set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.20)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Darwin")
    message(STATUS "Platform: Darwin")
    set(TOOLCHAIN_PATH ${CMAKE_CURRENT_LIST_DIR}/utils/toolchain/macOS/bin/riscv-none-embed-)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}g++)
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}gcc)
    set(CMAKE_AR ${TOOLCHAIN_PATH}ar)
    set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}objcopy)
    set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}objdump)
    set(SIZE ${TOOLCHAIN_PATH}size)
elseif("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "")
    message(STATUS "Platform: Linux")
    set(TOOLCHAIN_PATH ${CMAKE_CURRENT_LIST_DIR}/utils/toolchain/Linux/bin/riscv-none-embed-)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}g++)
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}gcc)
    set(CMAKE_AR ${TOOLCHAIN_PATH}ar)
    set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}objcopy)
    set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}objdump)
    set(SIZE ${TOOLCHAIN_PATH}size)
elseif("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "")
    message(STATUS "Platform: Windows")
    set(TOOLCHAIN_PATH ${CMAKE_CURRENT_LIST_DIR}/utils/toolchain/Windows/bin/riscv-none-embed-)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}gcc.exe)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}g++.exe)
    set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}gcc.exe)
    set(CMAKE_AR ${TOOLCHAIN_PATH}ar.exe)
    set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}objcopy.exe)
    set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}objdump.exe)
    set(SIZE ${TOOLCHAIN_PATH}size.exe)
endif()

project(qmk_port_ch582 C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

execute_process(COMMAND git rev-parse --short HEAD
    TIMEOUT 5
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

include(qmk_porting/keyboards/keyboard_target.cmake)

add_definitions(-DKEYMAP_C="keymap.c")
add_definitions(-D__GIT_VERSION__=${GIT_VERSION})
add_definitions(-D__KEY__=${keyboard}_key.h)
add_definitions(-D__ADDR__=${keyboard}_address.h)

add_compile_options(-march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -std=gnu99)
add_compile_options(-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common)
add_compile_options(--param highcode-gen-section-name=1)
add_compile_options(-Wall -Wno-comment -Wno-enum-compare)

# add_compile_options(-Wno-unused-function -Wno-unused-variable -Wno-type-limits)
add_compile_options(-fdiagnostics-color=always)

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

if(NOT keyboard)
    message(FATAL_ERROR "You must select a keyboard!")
endif()

if(NOT keymap)
    message(FATAL_ERROR "You must select a keymap!")
endif()

# get_property(dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# message("include_dirs=${dirs}")
file(GLOB APP_SOURCES
    ${CH582_SOURCES}
    ${quantum_SOURCES}
    ${tmk_core_SOURCES}
    ${QMK_PORTING_SOURCES}
)
file(GLOB IAP_SOURCES
    ${CH582_IAP_SOURCES}
    ${QMK_PORTING_IAP_SOURCES}
)

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

target_link_libraries(${keyboard}.elf ${CH582_LIB})
target_link_libraries(${keyboard}_IAP.elf ${CH582_LIB})

if(BLE_ENABLE OR(ESB_ENABLE AND((ESB_ROLE STREQUAL "keyboard") OR(ESB_ROLE STREQUAL "dongle"))))
    add_subdirectory(${PROJECT_SOURCE_DIR}/qmk_porting/platforms/ch58x/wireless)
    target_link_libraries(${keyboard}.elf wireless)

    # target_link_libraries(${keyboard}_IAP.elf wireless)
endif()

set(HEX_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}.hex)
set(BIN_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}.bin)
add_custom_command(TARGET ${keyboard}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${keyboard}.elf> ${HEX_FILE_APP}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${keyboard}.elf> ${BIN_FILE_APP}
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/utils/uf2conv.py ${HEX_FILE_APP} -c -f 0x1aa143c7 -o ${CMAKE_CURRENT_LIST_DIR}/${keyboard}_upgrade.uf2
)
set(HEX_FILE_IAP ${PROJECT_BINARY_DIR}/${keyboard}_IAP.hex)
set(BIN_FILE_IAP ${PROJECT_BINARY_DIR}/${keyboard}_IAP.bin)
add_custom_command(TARGET ${keyboard}_IAP.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${keyboard}_IAP.elf> ${HEX_FILE_IAP}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${keyboard}_IAP.elf> ${BIN_FILE_IAP}
)
add_custom_target(post_build ALL
    COMMAND mergehex --merge ${HEX_FILE_APP} ${HEX_FILE_IAP} --output ${CMAKE_CURRENT_LIST_DIR}/${keyboard}_factory.hex
    DEPENDS ${keyboard}.elf ${keyboard}_IAP.elf
)
