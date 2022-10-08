include_sub_directories_recursively(${CMAKE_CURRENT_LIST_DIR})

file(GLOB QMK_PORTING_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/keyboards/${keyboard}/keymaps/${keymap}/*.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/bootloader.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/platform.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/suspend.c"
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/timer.c"
    "${CMAKE_CURRENT_LIST_DIR}/protocol/*.c"
)

file(GLOB_RECURSE CH582_IAP_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/IAP/*.c"
)

# list(APPEND CH582_SOURCES
#     "${CMAKE_CURRENT_LIST_DIR}/platforms/ch58x/platform.c"
# )
