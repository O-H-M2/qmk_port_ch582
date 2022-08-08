include_directories(${CMAKE_CURRENT_LIST_DIR}/BLE_LIB
                    ${CMAKE_CURRENT_LIST_DIR}/HAL/include
                    ${CMAKE_CURRENT_LIST_DIR}/RVMSIS  
                    ${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/inc
                    ${CMAKE_CURRENT_LIST_DIR}/USB_LIB
                    )

file(GLOB_RECURSE CH582_SOURCES 
                "${CMAKE_CURRENT_LIST_DIR}/HAL/*.c"
                "${CMAKE_CURRENT_LIST_DIR}/RVMSIS/*.c"
                "${CMAKE_CURRENT_LIST_DIR}/Startup/*.S" 
                "${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/*.c" 
                "${CMAKE_CURRENT_LIST_DIR}/USB_LIB/*.c"
                )

list(REMOVE_ITEM CH582_SOURCES
                "${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/CH58x_usbdev.c"
                )

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/Ld/Link.ld)
set(CH582_LIB
    ${CMAKE_CURRENT_LIST_DIR}/StdPeriphDriver/libISP583.a 
    ${CMAKE_CURRENT_LIST_DIR}/BLE_LIB/LIBCH58xBLE.a 
    ${CMAKE_CURRENT_LIST_DIR}/USB_LIB/libRV3UFI.a
    )

add_link_options(
                -nostartfiles 
                -Xlinker --gc-sections  
                -Wl,--print-memory-usage
                -Wl,-Map,${PROJECT_NAME}.map 
                --specs=nano.specs 
                --specs=nosys.specs
                )
add_link_options(-T ${LINKER_SCRIPT})