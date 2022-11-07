include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard})
include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/keymaps/${keymap})
include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/drivers)
include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/platforms)
include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/protocol)

file(GLOB QMK_PORTING_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/bootloader.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/platform.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/suspend.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/timer.c"
    "${CMAKE_CURRENT_LIST_DIR}/protocol/*.c"
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