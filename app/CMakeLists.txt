cmake_minimum_required(VERSION 3.20.0)

set(DTC_OVERLAY_FILE boards/esp32s3_devkitc.overlay)
set(BOARD esp32s3_devkitc/esp32s3/procpu)

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_sources(app PRIVATE src/main.cpp)
