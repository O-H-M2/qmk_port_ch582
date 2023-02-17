# Copyright 2022 Huckies <https://github.com/Huckies>
#                zhaqian <https://github.com/zhaqian12>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
