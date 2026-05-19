cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(DTC_OVERLAY_FILE boards/esp32s3_devkitc.overlay)
set(BOARD our_board/esp32s3/procpu)

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_sources(app PRIVATE src/main.cpp)

add_subdirectory(drivers/our_driver)