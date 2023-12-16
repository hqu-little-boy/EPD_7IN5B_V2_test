cmake_minimum_required(VERSION 3.20)
project(e_papaer_c) 

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
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
)

# add_compile_options(-fpermissive)

target_link_libraries(e_papaer_c m)
target_link_libraries(e_papaer_c cap)
target_link_libraries(e_papaer_c  -Wl,--gc-sections)
