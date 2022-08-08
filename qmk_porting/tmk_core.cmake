include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core)

file(GLOB_RECURSE tmk_core_SOURCES 
                # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core/*.c"
                "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core/protocol/host.c"
                "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core/protocol/report.c"
                # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core/protocol/serial_soft.c"
                # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core/protocol/serial_uart.c"
                # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core/protocol/usb_descriptor.c"
                "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core/protocol/usb_device_state.c"
                # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core/protocol/usb_util.c"
                )

list(APPEND tmk_core_SOURCES
                "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/tmk_core/protocol/usb_util.c"
                )