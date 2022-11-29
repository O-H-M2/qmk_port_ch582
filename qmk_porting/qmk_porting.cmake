include_directories(${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x)
include_directories(${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom)
include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/drivers)

# include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/platforms)
include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/protocol)

file(GLOB QMK_PORTING_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/bootloader.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/platform.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/suspend.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/timer.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/battery_measure.c"
    "${CMAKE_CURRENT_LIST_DIR}/protocol/extra_keycode.c"
    "${CMAKE_CURRENT_LIST_DIR}/protocol/protocol.c"
)

file(GLOB_RECURSE QMK_PORTING_IAP_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/IAP/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/bootloader.c"
)

list(APPEND QMK_PORTING_IAP_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/core/usbd_core.c"
    "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/port/ch32/usb_ch58x_dc_usbfs.c"
    "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/class/msc/usbd_msc.c"
)

if(${keyboard_type} STREQUAL "public")
    include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard})
    include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/keymaps/${keymap})
    list(APPEND QMK_PORTING_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/*.c"
    )

    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/keymaps/${keymap}/rules.cmake")
        message(STATUS "Found keymap specified rules.")
        include(${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/keymaps/${keymap}/rules.cmake)
    endif()
elseif(${keyboard_type} STREQUAL "private")
    include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard})
    include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard}/keymaps/${keymap})
    list(APPEND QMK_PORTING_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard}/*.c"
    )

    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard}/keymaps/${keymap}/rules.cmake")
        message(STATUS "Found keymap specified rules.")
        include(${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard}/keymaps/${keymap}/rules.cmake)
    endif()
endif()

if(BUILD_WIRELESS_LIB)
    message(STATUS "Building wireless LIB, BLE and ESB force on.")
    set(BLE_ENABLE ON CACHE BOOL "KB" FORCE)
    set(ESB_ENABLE ON CACHE BOOL "KB" FORCE)
endif()