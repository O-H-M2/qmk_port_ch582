# default values
set(WS2812_REQUIRED OFF CACHE BOOL "QMK" FORCE)

# MOUSE_ENABLE
if(MOUSE_ENABLE)
    add_definitions(-DMOUSE_ENABLE -DMOUSEKEY_ENABLE)
    message(STATUS "MOUSE_ENABLE")
    list(APPEND quantum_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/mousekey.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/pointing_device_drivers.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/pointing_device.c"
    )
endif()

# NKRO_ENABLE
if(NKRO_ENABLE)
    add_definitions(-DNKRO_ENABLE -DFORCE_NKRO)
    message(STATUS "NKRO_ENABLE")
    list(APPEND quantum_SOURCES
    )
endif()

# ENCODER_ENABLE
if(ENCODER_ENABLE)
    add_definitions(-DENCODER_ENABLE)
    message(STATUS "ENCODER_ENABLE")
    list(APPEND quantum_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/encoder.c"
    )
endif()

# ENCODER_MAP_ENABLE
if(ENCODER_MAP_ENABLE)
    if(NOT ENCODER_ENABLE)
        message(FATAL_ERROR "ENCODER_MAP_ENABLE requires ENCODER_ENABLE, either disable ENCODER_MAP explicitly or enable ENCODER!")
    endif()

    add_definitions(-DENCODER_MAP_ENABLE)
    message(STATUS "ENCODER_MAP_ENABLE")
endif()

# VIA_ENABLE
if(VIA_ENABLE)
    add_definitions(-DVIA_ENABLE -DRAW_ENABLE -DDYNAMIC_KEYMAP_ENABLE)
    set(EEPROM_ENABLE ON CACHE BOOL "QMK" FORCE)
    set(BOOTMAGIC_ENABLE ON CACHE BOOL "QMK" FORCE)
    message(STATUS "VIA_ENABLE")
    list(APPEND quantum_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/dynamic_keymap.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/via.c"
    )
endif()

# COMMAND_ENABLE
if(COMMAND_ENABLE)
    add_definitions(-DCOMMAND_ENABLE)
    set(EEPROM_ENABLE ON CACHE BOOL "QMK" FORCE)
    message(STATUS "COMMAND_ENABLE")
    list(APPEND quantum_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/command.c"
    )
endif()

# BOOTMAGIC_ENABLE
if(BOOTMAGIC_ENABLE)
    add_definitions(-DBOOTMAGIC_LITE -DBOOTMAGIC_ENABLE)
    message(STATUS "BOOTMAGIC_ENABLE")
    list(APPEND quantum_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/bootmagic/magic.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/bootmagic/bootmagic_lite.c"
    )
endif()

# RGBLIGHT_ENABLE
if(RGBLIGHT_ENABLE)
    add_definitions(-DRGBLIGHT_ENABLE)

    # add_definitions(-DUSE_CIE1931_CURVE)
    set(EEPROM_ENABLE ON CACHE BOOL "QMK" FORCE)
    set(WS2812_REQUIRED ON CACHE BOOL "QMK" FORCE)
    message(STATUS "RGBLIGHT_ENABLE")
    list(APPEND quantum_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/rgblight/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/color.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/process_keycode/process_rgb.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/led_tables.c"
    )
endif()

# RGB_MATRIX_ENABLE
if(RGB_MATRIX_ENABLE)
    add_definitions(-DRGB_MATRIX_ENABLE)

    # add_definitions(-DUSE_CIE1931_CURVE)
    set(EEPROM_ENABLE ON CACHE BOOL "QMK" FORCE)
    set(WS2812_REQUIRED ON CACHE BOOL "QMK" FORCE)
    message(STATUS "RGB_MATRIX_ENABLE")
    include_directories(${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/lib/lib8tion)
    list(APPEND quantum_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/rgb_matrix/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/color.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/process_keycode/process_rgb.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/lib/lib8tion/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/led_tables.c"
    )
endif()

# EEPROM_ENABLE
if(EEPROM_ENABLE)
    add_definitions(-DEEPROM_ENABLE -DEEPROM_DRIVER)
    message(STATUS "EEPROM_ENABLE")
    message(STATUS "EEPROM_DRIVER = ${EEPROM_DRIVER}")
    list(APPEND quantum_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/drivers/eeprom/eeprom_driver.c"
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/eeconfig.c"
    )

    if(EEPROM_DRIVER STREQUAL "custom")
        add_definitions(-DEEPROM_CUSTOM)
        include_directories(${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/drivers/eeprom)
        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/eeprom_ch58x_custom.c"
        )
        list(APPEND QMK_PORTING_IAP_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/eeprom_ch58x_custom.c"
        )
    elseif(EEPROM_DRIVER STREQUAL "wear_leveling")
        add_definitions(-DEEPROM_WEAR_LEVELING)
        include_directories(${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/drivers/eeprom)
        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/eeprom_wear_leveling.c"
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/wear_leveling_ch58x.c"
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/wear_leveling.c"
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/fnv/*.c"
        )
        list(APPEND QMK_PORTING_IAP_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/eeprom_wear_leveling.c"
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/wear_leveling_ch58x.c"
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/wear_leveling.c"
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/fnv/*.c"
        )
    else()
        message(FATAL_ERROR "Unsupported EEprom Driver!")
    endif()
endif()

# WS2812 REQUIRED
if(WS2812_REQUIRED)
    add_definitions(-DWS2812)
    message(STATUS "WS2812_REQUIRED")
    message(STATUS "WS2812_DRIVER = ${WS2812_DRIVER}")

    if(WS2812_DRIVER STREQUAL "spi")
        add_definitions(-DWS2812_DRIVER_SPI)
        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/drivers/ws2812/ws2812_spi.c"
        )
    elseif(WS2812_DRIVER STREQUAL "pwm")
        add_definitions(-DWS2812_DRIVER_PWM)
        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/drivers/ws2812/ws2812_pwm.c"
        )
    else()
        message(FATAL_ERROR "Unsupported WS2812 Driver!")
    endif()
endif()

# USB_ENABLE
if(USB_ENABLE)
    add_definitions(-DUSB_ENABLE -DEP_NUMS=8)
    message(STATUS "USB_ENABLE")
    list(APPEND QMK_PORTING_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/core/usbd_core.c"
        "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/port/ch32/usb_ch58x_dc_usbfs.c"
        "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/class/hid/usbd_hid.c"
    )
endif()

# BLE_ENABLE
if(BLE_ENABLE)
    add_definitions(-DBLE_ENABLE)
    message(STATUS "BLE_ENABLE: Nkro is forced enable")
    add_definitions(-DNKRO_ENABLE -DFORCE_NKRO)

    list(APPEND QMK_PORTING_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/wireless/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/wireless/ble/*.c"
    )
endif()

# ESB_ENABLE
if(ESB_ENABLE)
    if(ESB_ROLE STREQUAL "keyboard")
        message(STATUS "ESB_ENABLE")
        add_definitions(-DESB_ENABLE=1)
        message(STATUS "ESB_ROLE = keyboard")
        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/wireless/*.c"
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/wireless/esb/*.c"
        )
    elseif(ESB_ROLE STREQUAL "dongle")
        message(STATUS "ESB_ENABLE")
        add_definitions(-DESB_ENABLE=2)
        message(STATUS "ESB_ROLE = dongle")

        if(BLE_ENABLE)
            remove_definitions(-DBLE_ENABLE)
            message(WARNING "Removed BLE_ENABLE")
        endif()

        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/wireless/*.c"
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/wireless/esb/*.c"
        )

        if(NOT USB_ENABLE)
            message(FATAL_ERROR "ESB_ROLE: dongle requires USB_ENABLE.")
        endif()
    endif()
endif()

# CUSTOM_MATRIX
if(CUSTOM_MATRIX)
    add_definitions(-DCUSTOM_MATRIX)
    message(STATUS "CUSTOM_MATRIX: ${CUSTOM_MATRIX}")
    list(APPEND QMK_PORTING_SOURCES
        "${CUSTOM_MATRIX}"
    )
else()
    list(APPEND quantum_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/matrix.c"
    )
endif()