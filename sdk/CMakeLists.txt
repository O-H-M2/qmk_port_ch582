# Copyright 2022 Huckies <https://github.com/Huckies>
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

cmake_minimum_required(VERSION 3.20)

# Debug message
message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

add_library(CH582_APP INTERFACE)
add_library(CH582_IAP INTERFACE)

target_include_directories(CH582_APP INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/BLE_LIB
    ${CMAKE_CURRENT_LIST_DIR}/HAL/include
    ${CMAKE_CURRENT_LIST_DIR}/LWNS
    ${CMAKE_CURRENT_LIST_DIR}/printf
    ${CMAKE_CURRENT_LIST_DIR}/RVMSIS
    ${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/inc

    # ${CMAKE_CURRENT_LIST_DIR}/USB_LIB
)
target_include_directories(CH582_IAP INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/BLE_LIB
    ${CMAKE_CURRENT_LIST_DIR}/HAL/include
    ${CMAKE_CURRENT_LIST_DIR}/LWNS
    ${CMAKE_CURRENT_LIST_DIR}/printf
    ${CMAKE_CURRENT_LIST_DIR}/RVMSIS
    ${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/inc

    # ${CMAKE_CURRENT_LIST_DIR}/USB_LIB
)

file(GLOB_RECURSE ch582_sdk_temp
    "${CMAKE_CURRENT_LIST_DIR}/printf/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/RVMSIS/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/*.c"

    # "${CMAKE_CURRENT_LIST_DIR}/USB_LIB/*.c"
)
list(REMOVE_ITEM ch582_sdk_temp
    "${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/CH58x_usbdev.c"
)

if(BUILD_WIRELESS_LIB)
    list(APPEND ch582_sdk_temp
        "${CMAKE_CURRENT_LIST_DIR}/HAL/MCU.c"
        "${CMAKE_CURRENT_LIST_DIR}/HAL/RTC.c"
    )
endif()

target_sources(CH582_APP INTERFACE
    ${ch582_sdk_temp}
    "${CMAKE_CURRENT_LIST_DIR}/Startup/startup_CH583.S"
)
target_sources(CH582_IAP INTERFACE
    ${ch582_sdk_temp}
    "${CMAKE_CURRENT_LIST_DIR}/Startup/startup_CH583.S"
)

target_link_options(CH582_APP INTERFACE
    -T ${CMAKE_CURRENT_LIST_DIR}/Ld/Link_APP.ld
)
target_link_options(CH582_IAP INTERFACE
    -T ${CMAKE_CURRENT_LIST_DIR}/Ld/Link_IAP.ld
)

target_link_libraries(CH582_APP INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/libISP583.a
    ${CMAKE_CURRENT_LIST_DIR}/BLE_LIB/LIBCH58xBLE.a

    # ${CMAKE_CURRENT_LIST_DIR}/USB_LIB/libRV3UFI.a
)
target_link_libraries(CH582_IAP INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/libISP583.a
    ${CMAKE_CURRENT_LIST_DIR}/BLE_LIB/LIBCH58xBLE.a

    # ${CMAKE_CURRENT_LIST_DIR}/USB_LIB/libRV3UFI.a
)

# Debug message
message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")
