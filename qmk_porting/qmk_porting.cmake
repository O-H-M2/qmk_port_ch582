include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE QMK_PORTING_SOURCES
                "${CMAKE_CURRENT_LIST_DIR}/keyboards/*.c"
                "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/bootloader.c"
                "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/platform.c"
                "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/suspend.c"
                "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/timer.c"
                "${CMAKE_CURRENT_LIST_DIR}/protocol/*.c"
                )