set(MOUSE_ENABLE OFF CACHE BOOL "KB" FORCE)
set(NKRO_ENABLE OFF CACHE BOOL "KB" FORCE)
set(ENCODER_ENABLE OFF CACHE BOOL "KB" FORCE) # TODO: fix this
set(VIA_ENABLE OFF CACHE BOOL "KB" FORCE)
set(BOOTMAGIC_ENABLE ON CACHE BOOL "KB" FORCE)
set(COMMAND_ENABLE ON CACHE BOOL "KB" FORCE)
set(RGBLIGHT_ENABLE OFF CACHE BOOL "KB" FORCE)
set(RGB_MATRIX_ENABLE OFF CACHE BOOL "KB" FORCE)
set(EEPROM_DRIVER "custom" CACHE STRING "KB" FORCE)
set(WS2812_DRIVER "spi" CACHE STRING "KB" FORCE)

# set(CUSTOM_MATRIX "${CMAKE_CURRENT_LIST_DIR}/matrix.c" CACHE STRING "KB" FORCE)
set(BLE_ENABLE ON CACHE BOOL "KB" FORCE)
set(ESB_ENABLE OFF CACHE BOOL "KB" FORCE)
