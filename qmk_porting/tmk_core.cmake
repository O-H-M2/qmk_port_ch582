include_sub_directories_recursively(${QMK_BASE_DIR}/tmk_core)

file(GLOB_RECURSE tmk_core_SOURCES
    "${QMK_BASE_DIR}/tmk_core/protocol/host.c"
    "${QMK_BASE_DIR}/tmk_core/protocol/report.c"
    "${QMK_BASE_DIR}/tmk_core/protocol/usb_device_state.c"
)

list(APPEND tmk_core_SOURCES
    "${QMK_BASE_DIR}/tmk_core/protocol/usb_util.c"
)