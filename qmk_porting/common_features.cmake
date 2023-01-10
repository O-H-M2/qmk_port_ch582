# MOUSE_ENABLE
if(MOUSE_ENABLE)
    add_definitions(-DMOUSE_ENABLE -DMOUSEKEY_ENABLE)
    message(STATUS "MOUSE_ENABLE")
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/mousekey.c"
        "${QMK_BASE_DIR}/quantum/pointing_device_drivers.c"
        "${QMK_BASE_DIR}/quantum/pointing_device.c"
    )
endif()

# NKRO_ENABLE
if(NKRO_ENABLE)
    add_definitions(-DNKRO_ENABLE -DFORCE_NKRO)
    message(STATUS "NKRO_ENABLE")
    list(APPEND quantum_SOURCES
    )
endif()

# GRAVE_ESC_ENABLE
if(GRAVE_ESC_ENABLE)
    message(STATUS "GRAVE_ESC_ENABLE")
    add_definitions(-DGRAVE_ESC_ENABLE)
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/process_keycode/process_grave_esc.c"
    )
endif()

# ENCODER_ENABLE
if(ENCODER_ENABLE)
    add_definitions(-DENCODER_ENABLE)
    message(STATUS "ENCODER_ENABLE")
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/encoder.c"
    )

    # ENCODER_MAP_ENABLE
    if(ENCODER_MAP_ENABLE)
        add_definitions(-DENCODER_MAP_ENABLE)
        message(STATUS "ENCODER_MAP_ENABLE")
    endif()
endif()

# VIA_ENABLE
if(VIA_ENABLE)
    add_definitions(-DVIA_ENABLE -DRAW_ENABLE -DDYNAMIC_KEYMAP_ENABLE)
    set(EEPROM_ENABLE ON CACHE BOOL "QMK" FORCE)
    set(BOOTMAGIC_ENABLE ON CACHE BOOL "QMK" FORCE)
    message(STATUS "VIA_ENABLE")
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/dynamic_keymap.c"
        "${QMK_BASE_DIR}/quantum/via.c"
    )
endif()

# COMMAND_ENABLE
if(COMMAND_ENABLE)
    add_definitions(-DCOMMAND_ENABLE)
    set(EEPROM_ENABLE ON CACHE BOOL "QMK" FORCE)
    message(STATUS "COMMAND_ENABLE")
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/command.c"
    )
endif()

# BOOTMAGIC_ENABLE
if(BOOTMAGIC_ENABLE)
    add_definitions(-DBOOTMAGIC_LITE -DBOOTMAGIC_ENABLE)
    message(STATUS "BOOTMAGIC_ENABLE")
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/bootmagic/magic.c"
        "${QMK_BASE_DIR}/quantum/bootmagic/bootmagic_lite.c"
    )
endif()

# RGBLIGHT_ENABLE
if(RGBLIGHT_ENABLE)
    add_definitions(-DRGBLIGHT_ENABLE)
    set(EEPROM_ENABLE ON CACHE BOOL "QMK" FORCE)

    if(RGBLIGHT_DRIVER STREQUAL "WS2812")
        set(WS2812_REQUIRED ON CACHE BOOL "QMK" FORCE)
    elseif(RGBLIGHT_DRIVER STREQUAL "AW20216")
        set(AW20216_REQUIRED ON CACHE BOOL "QMK" FORCE)
    endif()

    message(STATUS "RGBLIGHT_ENABLE")
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/rgblight/*.c"
        "${QMK_BASE_DIR}/quantum/color.c"
        "${QMK_BASE_DIR}/quantum/process_keycode/process_rgb.c"
        "${QMK_BASE_DIR}/quantum/led_tables.c"
    )
endif()

# RGB_MATRIX_ENABLE
if(RGB_MATRIX_ENABLE)
    add_definitions(-DRGB_MATRIX_ENABLE)
    set(EEPROM_ENABLE ON CACHE BOOL "QMK" FORCE)

    if(RGB_MATRIX_DRIVER STREQUAL "WS2812")
        set(WS2812_REQUIRED ON CACHE BOOL "QMK" FORCE)
    elseif(RGB_MATRIX_DRIVER STREQUAL "AW20216")
        set(AW20216_REQUIRED ON CACHE BOOL "QMK" FORCE)
    endif()

    message(STATUS "RGB_MATRIX_ENABLE")
    include_directories(${QMK_BASE_DIR}/lib/lib8tion)
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/rgb_matrix/*.c"
        "${QMK_BASE_DIR}/quantum/color.c"
        "${QMK_BASE_DIR}/quantum/process_keycode/process_rgb.c"
        "${QMK_BASE_DIR}/lib/lib8tion/*.c"
        "${QMK_BASE_DIR}/quantum/led_tables.c"
    )
endif()

# AUXILIARY_RGB_ENABLE
if(AUXILIARY_RGB_ENABLE)
    if(NOT RGB_MATRIX_ENABLE)
        message(FATAL_ERROR "AUXILIARY_RGB_ENABLE requires RGB_MATRIX_ENABLE!")
    else()
        message(STATUS "AUXILIARY_RGB_ENABLE")
        add_definitions(-DRGB_MATRIX_CUSTOM_KB -DRGB_RAW_ENABLE)
        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/protocol/auxiliary_rgb.c"
            "${CMAKE_CURRENT_LIST_DIR}/protocol/openrgb.c"
        )
    endif()
endif()

# SPLIT_KEYBOARD
if(SPLIT_KEYBOARD)
    add_definitions(-DSPLIT_KEYBOARD)
    include_directories(${QMK_BASE_DIR}/quantum/split_common)
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/split_common/split_util.c"
    )

    if(NOT SPLIT_TRANSPORT STREQUAL "custom")
        message(FATAL_ERROR "Unsupported split keyboard driver!")
    else()
        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/split/transactions.c"
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/split/transport.c"
        )
    endif()
endif()

# CIE1931_CURVE
if(CIE1931_CURVE)
    add_definitions(-DCIE1931_CURVE)
    message(STATUS "CIE1931_CURVE")
endif()

# EEPROM_ENABLE
if(EEPROM_ENABLE)
    add_definitions(-DEEPROM_ENABLE -DEEPROM_DRIVER)
    message(STATUS "EEPROM_ENABLE")
    message(STATUS "EEPROM_DRIVER = ${EEPROM_DRIVER}")
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/drivers/eeprom/eeprom_driver.c"
        "${QMK_BASE_DIR}/quantum/eeconfig.c"
    )

    if(EEPROM_DRIVER STREQUAL "custom")
        add_definitions(-DEEPROM_CUSTOM)
        include_directories(${QMK_BASE_DIR}/drivers/eeprom)
        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/eeprom_ch58x_custom.c"
        )
        list(APPEND QMK_PORTING_IAP_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom/eeprom_ch58x_custom.c"
        )
    elseif(EEPROM_DRIVER STREQUAL "wear_leveling")
        add_definitions(-DEEPROM_WEAR_LEVELING)
        include_directories(${QMK_BASE_DIR}/drivers/eeprom)
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
        message(FATAL_ERROR "Unsupported EEPROM driver!")
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

# OLED ENABLE
if(OLED_ENABLE)
    if(OLED_DRIVER STREQUAL "SSD1306")
        add_definitions(-DOLED_ENABLE -DOLED_DRIVER_SSD1306)
        message(STATUS "OLED_ENABLE")
        message(STATUS "OLED_DRIVER = ${OLED_DRIVER}")
        include_directories(${QMK_BASE_DIR}/drivers/oled)
        list(APPEND quantum_SOURCES
            "${QMK_BASE_DIR}/drivers/oled/ssd1306_sh1106.c"
        )
        list(APPEND QMK_PORTING_SOURCES
            "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/i2c_master.c"
        )
    else()
        message(FATAL_ERROR "Unsupported OLED driver!")
    endif()
endif()

# WPM ENABLE
if(WPM_ENABLE)
    add_definitions(-DWPM_ENABLE)
    message(STATUS "WPM_ENABLE")
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/wpm.c"
    )
endif()

# AW20216 REQUIRED
if(AW20216_REQUIRED)
    add_definitions(-DAW20216)
    message(STATUS "AW20216_REQUIRED")
    include_directories(${QMK_BASE_DIR}/drivers/led)
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/drivers/led/aw20216.c"
    )
    list(APPEND QMK_PORTING_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/drivers/aw20216/aw20216_supplement.c"
        "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/spi_master.c"
    )
endif()

# USB_ENABLE
if(USB_ENABLE)
    add_definitions(-DUSB_ENABLE -DEP_NUMS=8)
    message(STATUS "USB_ENABLE")
    list(APPEND QMK_PORTING_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/core/usbd_core.c"
        "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/port/ch32/usb_ch58x_dc_usbfs.c"
        "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/class/hid/usbd_hid.c"
        "${CMAKE_CURRENT_LIST_DIR}/protocol/protocol_usb.c"
        "${CMAKE_CURRENT_LIST_DIR}/protocol/usb_main.c"
    )
endif()

# BLE_ENABLE
if(BLE_ENABLE)
    if(ESB_ENABLE AND ESB_ROLE STREQUAL "dongle")
        message(STATUS "Dongle should have BLE_ENABLE, ignore")
    else()
        add_definitions(-DBLE_ENABLE)
        message(STATUS "BLE_ENABLE: Nkro is forced enable")
        add_definitions(-DNKRO_ENABLE -DFORCE_NKRO)
    endif()
endif()

# ESB_ENABLE
if(ESB_ENABLE)
    if(ESB_ROLE STREQUAL "keyboard")
        message(STATUS "ESB_ENABLE")
        add_definitions(-DESB_ENABLE=1)
        message(STATUS "ESB_ROLE = keyboard")
    elseif(ESB_ROLE STREQUAL "dongle")
        message(STATUS "ESB_ENABLE")
        add_definitions(-DESB_ENABLE=2)
        message(STATUS "ESB_ROLE = dongle")

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
        "${QMK_BASE_DIR}/quantum/matrix.c"
    )
endif()

# DIP_SWITCH_ENABLE
if(DIP_SWITCH_ENABLE)
    add_definitions(-DDIP_SWITCH_ENABLE)
    message(STATUS "DIP_SWITCH_ENABLE")
    list(APPEND quantum_SOURCES
        "${QMK_BASE_DIR}/quantum/dip_switch.c"
    )
endif()
