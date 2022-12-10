set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.20)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(QMK_BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/qmk_firmware" CACHE STRING "ENV" FORCE)
include(utils/toolchain/toolchain.cmake)

project(qmk_port_ch582 C CXX ASM)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

execute_process(COMMAND git rev-parse --short HEAD
    TIMEOUT 5
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# allow for manual override
if(NOT keyboard OR(NOT keymap AND NOT BUILD_WIRELESS_LIB))
    include(keyboard_target.cmake)
endif()

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/qmk_porting/keyboards/${keyboard}/rules.cmake")
    set(keyboard_type "public" CACHE STRING "KB" FORCE)
    message(STATUS "Compiling ${keyboard}:${keymap}")
    include(qmk_porting/keyboards/${keyboard}/rules.cmake)
elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/qmk_porting/keyboards_private/${keyboard}/rules.cmake")
    set(keyboard_type "private" CACHE STRING "KB" FORCE)
    message(STATUS "Compiling ${keyboard}:${keymap} from private")
    include(qmk_porting/keyboards_private/${keyboard}/rules.cmake)
else()
    message(FATAL_ERROR "You must select a keyboard and its keymap!")
endif()

add_definitions(-DKEYMAP_C="keymap.c")
add_definitions(-D__GIT_VERSION__=${GIT_VERSION})

add_compile_options(-march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -std=gnu99)
add_compile_options(-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common)
add_compile_options(--param highcode-gen-section-name=1)
add_compile_options(-Wall -Wno-comment -Wno-enum-compare)
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

include(qmk_porting/qmk_porting.cmake)
include(qmk_porting/quantum.cmake)
include(CherryUSB_porting/CherryUSB.cmake)
include(qmk_porting/tmk_core.cmake)
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

if(BUILD_WIRELESS_LIB)
    add_library(${keyboard} STATIC)
    target_link_libraries(${keyboard} PRIVATE CH582_APP)
    target_compile_definitions(${keyboard} PRIVATE __BUILDING_APP__=1)

    # target_link_options(${keyboard} PRIVATE
    # -nostartfiles
    # --specs=nano.specs
    # )
    if(${keyboard_type} STREQUAL "public")
        target_precompile_headers(${keyboard} PUBLIC
            ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/qmk_config.h
        )
    elseif(${keyboard_type} STREQUAL "private")
        target_precompile_headers(${keyboard} PUBLIC
            ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}/qmk_config.h
        )
    endif()

    if(BLE_ENABLE OR(ESB_ENABLE AND((ESB_ROLE STREQUAL "keyboard") OR(ESB_ROLE STREQUAL "dongle"))))
        add_subdirectory(${PROJECT_SOURCE_DIR}/${BUILD_WIRELESS_LIB})
        target_link_libraries(${keyboard} PRIVATE wireless)
    else()
        message(FATAL_ERROR "Remove defination of BUILD_WIRELESS_LIB!")
    endif()

    if(${keyboard_type} STREQUAL "public")
        add_custom_command(TARGET ${keyboard} POST_BUILD
            COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/wireless
            COMMAND mv ${CMAKE_BINARY_DIR}/lib${keyboard}.a ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/wireless/lib${keyboard}.a
            COMMAND cp -r ${PROJECT_SOURCE_DIR}/${BUILD_WIRELESS_LIB}/ble/include ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/wireless
            COMMAND cp -r ${PROJECT_SOURCE_DIR}/${BUILD_WIRELESS_LIB}/esb/include ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/wireless
        )
    elseif(${keyboard_type} STREQUAL "private")
        add_custom_command(TARGET ${keyboard} POST_BUILD
            COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}/wireless
            COMMAND mv ${CMAKE_BINARY_DIR}/lib${keyboard}.a ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}/wireless/lib${keyboard}.a
            COMMAND cp -r ${PROJECT_SOURCE_DIR}/${BUILD_WIRELESS_LIB}/ble/include ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}/wireless
            COMMAND cp -r ${PROJECT_SOURCE_DIR}/${BUILD_WIRELESS_LIB}/esb/include ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}/wireless
        )
    endif()

else()
    add_executable(${keyboard}.elf ${APP_SOURCES})
    add_executable(${keyboard}_IAP.elf ${IAP_SOURCES})

    target_link_libraries(${keyboard}.elf PRIVATE CH582_APP)
    target_link_libraries(${keyboard}_IAP.elf PRIVATE CH582_IAP)

    target_compile_definitions(${keyboard}.elf PRIVATE __BUILDING_APP__=1)
    target_compile_definitions(${keyboard}_IAP.elf PRIVATE __BUILDING_IAP__=1)

    target_link_options(${keyboard}.elf PRIVATE
        -nostartfiles
        -Xlinker --gc-sections
        -Wl,--print-memory-usage
        -Wl,-Map,${keyboard}.map
        --specs=nano.specs
        --specs=nosys.specs
    )
    target_link_options(${keyboard}_IAP.elf PRIVATE
        -nostartfiles
        -Xlinker --gc-sections
        -Wl,--print-memory-usage
        -Wl,-Map,${keyboard}_IAP.map
        --specs=nano.specs
        --specs=nosys.specs
    )

    if(${keyboard_type} STREQUAL "public")
        target_precompile_headers(${keyboard}.elf PUBLIC
            ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/qmk_config.h
        )
        target_precompile_headers(${keyboard}_IAP.elf PUBLIC
            ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/qmk_config.h
        )

        if(BLE_ENABLE OR(ESB_ENABLE AND((ESB_ROLE STREQUAL "keyboard") OR(ESB_ROLE STREQUAL "dongle"))))
            target_include_directories(${keyboard}.elf PRIVATE
                ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/wireless/include
            )
            target_link_libraries(${keyboard}.elf PRIVATE
                ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards/${keyboard}/wireless/lib${keyboard}.a
            )
        endif()
    elseif(${keyboard_type} STREQUAL "private")
        target_precompile_headers(${keyboard}.elf PUBLIC
            ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}/qmk_config.h
        )
        target_precompile_headers(${keyboard}_IAP.elf PUBLIC
            ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}/qmk_config.h
        )

        if(BLE_ENABLE OR(ESB_ENABLE AND((ESB_ROLE STREQUAL "keyboard") OR(ESB_ROLE STREQUAL "dongle"))))
            target_include_directories(${keyboard}.elf PRIVATE
                ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}/wireless/include
            )
            target_link_libraries(${keyboard}.elf PRIVATE
                ${PROJECT_SOURCE_DIR}/qmk_porting/keyboards_private/${keyboard}/wireless/lib${keyboard}.a
            )
        endif()
    endif()

    set(HEX_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}.hex)
    set(BIN_FILE_APP ${PROJECT_BINARY_DIR}/${keyboard}.bin)
    add_custom_command(TARGET ${keyboard}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${keyboard}.elf> ${HEX_FILE_APP}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${keyboard}.elf> ${BIN_FILE_APP}
        COMMAND python3 ${CMAKE_CURRENT_LIST_DIR}/utils/uf2conv.py ${HEX_FILE_APP} -c -f 0x1aa143c7 -o ${CMAKE_CURRENT_LIST_DIR}/${keyboard}_upgrade.uf2
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
endif()
