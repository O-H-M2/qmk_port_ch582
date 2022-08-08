include_directories(${CMAKE_CURRENT_LIST_DIR}
                    ${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/class/hid
                    ${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/class/printer
                    ${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/common
                    ${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/core
                    ${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/port/ch32
                    )

file(GLOB_RECURSE CherryUSB_SOURCES 
                    "${CMAKE_CURRENT_LIST_DIR}/*.c"
                    "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/core/usbd_core.c"
                    "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/port/ch32/usb_dc_ch58x.c"
                    "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/class/hid/usbd_hid.c"
                    )