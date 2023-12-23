cmake_minimum_required(VERSION 3.20)
project(e_papaer_c) 

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS -fpermissive)

add_executable(e_papaer_c main.cpp
        ./lib/Fonts/fonts.h
        ./lib/Config/Debug.h
        ./lib/Config/bcm2835.h
        ./lib/Config/DEV_Config.h
        ./lib/Config/sysfs_software_spi.h
        ./lib/e-Paper/EPD_5in83_V2.h
        ./src/EPD_Test.h
        ./src/ImageData.h
        ./lib/GUI/GUI_Paint.h
        ./lib/Fonts/fonts.h
        ./lib/Config/Debug.h
        ./lib/Config/DEV_Config.h
        ./lib/Config/sysfs_software_spi.h
        ./lib/Config/sysfs_gpio.h
        ./src/EPD_Test.h
        ./src/ImageData.h
        ./lib/GUI/GUI_Paint.cpp
        ./lib/Fonts/font8.cpp
        ./lib/Fonts/font12.cpp
        ./lib/Fonts/font16.cpp
        ./lib/Fonts/font20.cpp
        ./lib/Fonts/font24.cpp
        ./lib/Config/DEV_Config.cpp
        ./lib/Config/bcm2835.c
        ./lib/Config/sysfs_software_spi.cpp
        ./lib/e-Paper/EPD_5in83_V2.cpp
        ./src/ImageData.cpp
        ./src/ImageData2.cpp
        ./src/EPD_5in83_V2_test.cpp
        ./lib/Config/sysfs_gpio.cpp
        ./entity/Pic/Config.cpp
        ./entity/Pic/Config.h
        ./entity/Qweather/Weather.cpp
        ./entity/Qweather/Weather.h
        ./entity/Qweather/Date.cpp
        ./entity/Qweather/Date.h
        ./entity/Qweather/Time_MinAndSec.cpp
        ./entity/Qweather/Time_MinAndSec.h
        ./entity/Calendar/Calendar.h
        ./entity/Calendar/Calendar.cpp
        ./entity/Pic/EinkPic.cpp
        ./entity/Pic/EinkPic.h
)

# add_compile_options(-fpermissive)

target_link_libraries(e_papaer_c m) # 链接数学库，这里假设库名为m
target_link_libraries(e_papaer_c cap) # 链接数学库，这里假设库名为m

target_link_libraries(e_papaer_c  -Wl,--gc-sections)

set(CPR_BUILD_SHARED_LIBS OFF)
set(CPR_INSTALL OFF)
set(CPR_SOURCE_DIR "lib/cpr")
add_subdirectory(${CPR_SOURCE_DIR})
target_link_libraries(e_papaer_c cpr)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(nlohmann_json_DIR "lib/json")
add_subdirectory(${nlohmann_json_DIR})
target_link_libraries(e_papaer_c nlohmann_json::nlohmann_json)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(e_papaer_c ${OpenCV_LIBS})

add_subdirectory("lib/fmt" EXCLUDE_FROM_ALL)
target_link_libraries(e_papaer_c fmt::fmt)

