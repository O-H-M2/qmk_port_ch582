include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE QMK_PORTING_SOURCES
                "${CMAKE_CURRENT_LIST_DIR}/*.c"
                )