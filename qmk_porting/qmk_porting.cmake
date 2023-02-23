# Copyright 2022 Huckies <https://github.com/Huckies>
# zhaqian <https://github.com/zhaqian12>
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

include_directories(${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x)
include_directories(${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/eeprom)
include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/drivers)

# include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/platforms)
include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR}/protocol)

file(GLOB QMK_PORTING_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/bootloader.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/gpio.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/platform.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/suspend.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/timer.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/battery_measure.c"
    "${CMAKE_CURRENT_LIST_DIR}/protocol/extra_keycode.c"
    "${CMAKE_CURRENT_LIST_DIR}/protocol/protocol.c"
)

file(GLOB_RECURSE QMK_PORTING_IAP_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/IAP/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/battery_measure.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/bootloader.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/gpio.c"
)

list(APPEND QMK_PORTING_IAP_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/core/usbd_core.c"
    "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/port/ch32/usb_ch58x_dc_usbfs.c"
    "${CMAKE_CURRENT_LIST_DIR}/../CherryUSB/class/msc/usbd_msc.c"
)

if(${keyboard_type} STREQUAL "public")
    include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard})
    include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/keymaps/${keymap})
    list(APPEND QMK_PORTING_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/*.c"
    )

    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/keymaps/${keymap}/rules.cmake")
        message(STATUS "Found keymap specified rules.")
        include(${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/keymaps/${keymap}/rules.cmake)
    endif()
elseif(${keyboard_type} STREQUAL "private")
    include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard})
    include_directories(${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard}/keymaps/${keymap})
    list(APPEND QMK_PORTING_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard}/*.c"
    )

    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard}/keymaps/${keymap}/rules.cmake")
        message(STATUS "Found keymap specified rules.")
        include(${CMAKE_CURRENT_LIST_DIR}/keyboards_private/${keyboard}/keymaps/${keymap}/rules.cmake)
    endif()
endif()

add_definitions(-DQMK_VERSION="${QMK_VERSION}" -DQMK_KEYBOARD_H="${keyboard}.h")
message(STATUS "QMK Version ${QMK_VERSION}")

if(NOT BUILD_WIRELESS_LIB)
    add_definitions(-DQMK_BUILDDATE=${QMK_BUILDDATE})
    message(STATUS "QMK Build date ${QMK_BUILDDATE}")
endif()
