include_sub_directories_recursively(${QMK_BASE_DIR}/quantum)

include_directories(${QMK_BASE_DIR})

file(GLOB_RECURSE quantum_SOURCES

    # "${QMK_BASE_DIR}/quantum/audio/*.c"
    # "${QMK_BASE_DIR}/quantum/backlight/*.c"
    "${QMK_BASE_DIR}/quantum/debounce/sym_eager_pk.c"

    # "${QMK_BASE_DIR}/quantum/debounce/asym_eager_defer_pk.c"
    "${QMK_BASE_DIR}/quantum/keymap_extras/*.c"

    # "${QMK_BASE_DIR}/quantum/led_matrix/*.c"
    "${QMK_BASE_DIR}/quantum/logging/*.c"

    # "${QMK_BASE_DIR}/quantum/painter/*.c"
    "${QMK_BASE_DIR}/quantum/process_keycode/process_magic.c"

    # "${QMK_BASE_DIR}/quantum/sequencer/*.c"
    # "${QMK_BASE_DIR}/quantum/split_common/*.c"
    "${QMK_BASE_DIR}/quantum/action_layer.c"
    "${QMK_BASE_DIR}/quantum/action_tapping.c"
    "${QMK_BASE_DIR}/quantum/action_util.c"
    "${QMK_BASE_DIR}/quantum/action.c"
    "${QMK_BASE_DIR}/quantum/bitwise.c"
    "${QMK_BASE_DIR}/quantum/caps_word.c"

    # "${QMK_BASE_DIR}/quantum/crc.c"
    "${QMK_BASE_DIR}/quantum/deferred_exec.c"

    # "${QMK_BASE_DIR}/quantum/digitizer.c"
    # "${QMK_BASE_DIR}/quantum/dip_switch.c"
    # "${QMK_BASE_DIR}/quantum/haptic.c"
    # "${QMK_BASE_DIR}/quantum/joystick.c"
    "${QMK_BASE_DIR}/quantum/keyboard.c"
    "${QMK_BASE_DIR}/quantum/keycode_config.c"
    "${QMK_BASE_DIR}/quantum/keymap_common.c"
    "${QMK_BASE_DIR}/quantum/keymap_introspection.c"
    "${QMK_BASE_DIR}/quantum/led.c"
    "${QMK_BASE_DIR}/quantum/main.c"
    "${QMK_BASE_DIR}/quantum/matrix_common.c"
    "${QMK_BASE_DIR}/quantum/programmable_button.c"
    "${QMK_BASE_DIR}/quantum/quantum.c"

    # "${QMK_BASE_DIR}/quantum/secure.c"
    "${QMK_BASE_DIR}/quantum/send_string.c"
    "${QMK_BASE_DIR}/quantum/sync_timer.c"

    # "${QMK_BASE_DIR}/quantum/utf8.c"
    # "${QMK_BASE_DIR}/quantum/variable_trace.c"
    # "${QMK_BASE_DIR}/quantum/velocikey.c"
    # "${QMK_BASE_DIR}/quantum/wpm.c"
)
