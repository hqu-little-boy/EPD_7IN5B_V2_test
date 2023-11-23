cmake_minimum_required(VERSION 3.20)
project(e_papaer_c C)

set(CMAKE_C_STANDARD 11)

add_executable(e_papaer_c main.c
        ./lib/GUI/GUI_Paint.h
        ./lib/GUI/GUI_BMPfile.h
        ./lib/Fonts/fonts.h
        ./lib/Config/Debug.h
        ./lib/Config/DEV_Config.h
        ./lib/Config/dev_hardware_SPI.h
        ./lib/Config/sysfs_software_spi.h
        ./lib/e-Paper/EPD_7in5_V2.h
        ./lib/e-Paper/EPD_7in5b_V2.h
        ./src/EPD_Test.h
        ./src/ImageData.h
        ./lib/GUI/GUI_Paint.h
        ./lib/GUI/GUI_BMPfile.h
        ./lib/Fonts/fonts.h
        ./lib/Config/Debug.h
        ./lib/Config/DEV_Config.h
        ./lib/Config/dev_hardware_SPI.h
        ./lib/Config/sysfs_software_spi.h
        ./lib/e-Paper/EPD_7in5_V2.h
        ./lib/e-Paper/EPD_7in5b_V2.h
        ./src/EPD_Test.h
        ./src/ImageData.h
        ./lib/GUI/GUI_Paint.c
        ./lib/GUI/GUI_BMPfile.c
        ./lib/Fonts/font8.c
        ./lib/Fonts/font12.c
        ./lib/Fonts/font16.c
        ./lib/Fonts/font20.c
        ./lib/Fonts/font24.c
        ./lib/Fonts/font12CN.c
        ./lib/Fonts/font24CN.c
        ./lib/Config/DEV_Config.c
        ./lib/Config/dev_hardware_SPI.c
        ./lib/Config/sysfs_software_spi.c
        ./lib/e-Paper/EPD_7in5_V2.c
        ./lib/e-Paper/EPD_7in5b_V2.c
        ./src/ImageData.c
        ./src/ImageData2.c
        ./src/EPD_7in5b_V2_test.c
        ./lib/Config/sysfs_gpio.c
        ./lib/Config/sysfs_gpio.h
)


target_link_libraries(e_papaer_c m) # 链接数学库，这里假设库名为m
target_link_libraries(e_papaer_c bcm2835) # 链接数学库，这里假设库名为m

target_link_libraries(e_papaer_c  -Wl,--gc-sections)
