include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware
    ${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/lib/printf
)

file(GLOB_RECURSE quantum_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/lib/printf/*.c"

    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/audio/*.c"
    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/backlight/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/debounce/sym_eager_pk.c"

    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/debounce/asym_eager_defer_pk.c"
    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/encoder/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/keymap_extras/*.c"

    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/led_matrix/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/logging/*.c"

    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/painter/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/process_keycode/process_magic.c"

    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/sequencer/*.c"
    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/split_common/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/action_layer.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/action_tapping.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/action_util.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/action.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/bitwise.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/caps_word.c"

    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/crc.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/deferred_exec.c"

    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/digitizer.c"
    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/dip_switch.c"
    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/haptic.c"
    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/joystick.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/keyboard.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/keycode_config.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/keymap_common.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/led.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/main.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/matrix_common.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/matrix.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/programmable_button.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/quantum.c"

    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/secure.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/send_string.c"
    "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/sync_timer.c"

    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/utf8.c"
    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/variable_trace.c"
    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/velocikey.c"
    # "${CMAKE_CURRENT_LIST_DIR}/../qmk_firmware/quantum/wpm.c"
)