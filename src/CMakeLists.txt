# Copyright (c) 2024 embeddedboys developers

# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:

# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# This is the main CMakeLists file of the project. It is used to build the project
# using the CMake build system. You can find the documentation of CMake at
# https:#cmake.org/cmake/help/latest/index.html

set(OVERCLOCK_ENABLED 1)    # 1: enable, 0: disable

if(OVERCLOCK_ENABLED)

    message(WARNING "Overclocking is enabled. This may damage your device. Use at own risk.")

    if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
        # Overclocking profiles
        #      SYS_CLK  | FLASH_CLK | Voltage
        #  1  | 240MHz  |  120MHZ   |  1.10(V) (default, stable, recommended for most devices)
        #  2  | 266MHz  |  133MHz   |  1.10(V)
        #  3  | 360MHz  |  90MHz    |  1.20(V)
        #  4  | 400MHz  |  100MHz   |  1.30(V)
        #  5  | 416MHz  |  104MHz   |  1.30(V)
        set(OVERCLOCK_PROFILE 1)

        if(OVERCLOCK_PROFILE EQUAL 1)
            set(SYS_CLK_KHZ 240000)             # CPU clock speed
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
        elseif(OVERCLOCK_PROFILE EQUAL 2)
            set(SYS_CLK_KHZ 266000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        elseif(OVERCLOCK_PROFILE EQUAL 3)
            set(SYS_CLK_KHZ 360000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        elseif(OVERCLOCK_PROFILE EQUAL 4)
            set(SYS_CLK_KHZ 400000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        elseif(OVERCLOCK_PROFILE EQUAL 5)
            set(SYS_CLK_KHZ 416000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        else()
            message(FATAL_ERROR "Invalid overclocking profile")
        endif()
    elseif(${PICO_BOARD} STREQUAL "pico2" OR ${PICO_PLATFORM} STREQUAL "rp2350")
        # Overclocking profiles
        #      SYS_CLK  | FLASH_CLK | Voltage
        #  1  | 366MHz  |  183MHz   |  1.20(V)
        set(OVERCLOCK_PROFILE 1)

        if(OVERCLOCK_PROFILE EQUAL 1)
            set(SYS_CLK_KHZ 366000)             # CPU clock speed
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
        else()
            message(FATAL_ERROR "Invalid overclocking profile")
        endif()
    endif()

else()  # OVERCLOCK_ENABLED
    message(WARNING "Overclocking is disabled.")

    if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
        set(SYS_CLK_KHZ 125000) # CPU clock speed
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
    elseif(${PICO_BOARD} STREQUAL "pico2" OR ${PICO_PLATFORM} STREQUAL "rp2350")
        set(SYS_CLK_KHZ 150000) # CPU clock speed
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
    endif()

endif() # OVERCLOCK_ENABLED

# Default configurations for 8080 interface LCD Pins
set(LCD_PIN_DB_BASE  0)  # 8080 LCD data bus base pin
set(LCD_PIN_DB_COUNT 16) # 8080 LCD data bus pin count
set(LCD_PIN_CS  18)  # 8080 LCD chip select pin
set(LCD_PIN_WR  19)  # 8080 LCD write pin
set(LCD_PIN_RS  20)  # 8080 LCD register select pin
set(LCD_PIN_RD  21)  # 8080 LCD read pin
set(LCD_PIN_RST 22)  # 8080 LCD reset pin
set(LCD_PIN_BL  28)  # 8080 LCD backlight pin
set(DISP_OVER_PIO 1) # 1: PIO, 0: GPIO
set(PIO_USE_DMA   1)   # 1: use DMA, 0: not use DMA
set(I80_BUS_WR_CLK_KHZ 18000)

# LCD driver type
set(LCD_DRV_USE_ILI9488 1)
set(LCD_DRV_USE_ILI9806 0)
set(LCD_DRV_USE_R61581  0)
set(LCD_DRV_USE_ST6201  0)
set(LCD_DRV_USE_1P5623  0)
set(LCD_DRV_USE_LG4572B 0)
set(LCD_DRV_USE_ST7789  0)

if(LCD_DRV_USE_ILI9488)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/ili9488.cmake)
elseif(LCD_DRV_USE_ILI9806)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/ili9806.cmake)
elseif(LCD_DRV_USE_R61581)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/r61581.cmake)
elseif(LCD_DRV_USE_ST6201)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/st6201.cmake)
elseif(LCD_DRV_USE_1P5623)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/1p5623.cmake)
elseif(LCD_DRV_USE_LG4572B)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/lg4572b.cmake)
elseif(LCD_DRV_USE_ST7789)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/st7789.cmake)
endif()

# Rotation configuration
set(LCD_ROTATION 1)  # 0: normal, 1: 90 degree, 2: 180 degree, 3: 270 degree
if(${LCD_ROTATION} EQUAL 0 OR ${LCD_ROTATION} EQUAL 2)
    set(LCD_HOR_RES ${LCD_X_RES})
    set(LCD_VER_RES ${LCD_Y_RES})
elseif(${LCD_ROTATION} EQUAL 1 OR ${LCD_ROTATION} EQUAL 3)
    set(LCD_HOR_RES ${LCD_Y_RES})
    set(LCD_VER_RES ${LCD_X_RES})
else()
    message(FATAL_ERROR "ERROR: Invalid display rotation")
endif()

# Display buffer size configuration
if(DEFINED MY_DISP_BUF_SIZE AND NOT "${MY_DISP_BUF_SIZE}" STREQUAL "")
    message(WARNING "User defined display buffer size is used.")
else()
    if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
    math(EXPR MY_DISP_BUF_SIZE "${LCD_HOR_RES} * ${LCD_VER_RES} / 4")
    elseif(${PICO_BOARD} STREQUAL "pico2" OR ${PICO_PLATFORM} STREQUAL "rp2350")
    math(EXPR MY_DISP_BUF_SIZE "${LCD_HOR_RES} * ${LCD_VER_RES} / 2")
    endif()
endif()

# Input device driver type
set(INDEV_DRV_USED        1)
set(INDEV_DRV_USE_FT6236  0)
set(INDEV_DRV_USE_NS2009  0)
set(INDEV_DRV_USE_TSC2007 1)
set(INDEV_DRV_USE_GT911   0)

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--print-memory-usage")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--print-memory-usage")

include_directories(. ../include)

# add lvgl library here
add_subdirectory(lvgl)
target_compile_definitions(lvgl PRIVATE LCD_PIN_DB_COUNT=${LCD_PIN_DB_COUNT})

# lv_conf.h need pico header files e.g. the custom tick
# target_link_libraries(lvgl PUBLIC pico_stdlib)

# include PIO library here
add_subdirectory(pio)
target_compile_definitions(pio_i80 PUBLIC LCD_PIN_RS=${LCD_PIN_RS})
target_compile_definitions(pio_i80 PUBLIC LCD_PIN_CS=${LCD_PIN_CS})
target_compile_definitions(pio_i80 PUBLIC DEFAULT_PIO_CLK_KHZ=${PERI_CLK_KHZ})
target_compile_definitions(pio_i80 PUBLIC PIO_USE_DMA=${PIO_USE_DMA})
target_compile_definitions(pio_i80 PUBLIC I80_BUS_WR_CLK_KHZ=${I80_BUS_WR_CLK_KHZ})

# include factory test library here
add_subdirectory(factory)

# user define common source files
file(GLOB_RECURSE COMMON_SOURCES
    main.c
    tft.c
    tft_st7789.c
    tft_ili9488.c
    tft_ili9806.c
    tft_r61581.c
    tft_st6201.c
    tft_1p5623.c
    tft_lg4572b.c
    indev.c
    ns2009.c
    tsc2007.c
    ft6236.c
    gt911.c
    porting/lv_port_disp_template.c
    porting/lv_port_indev_template.c
    i2c_tools.c
    backlight.c
)

add_executable(${PROJECT_NAME} ${COMMON_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    pico_multicore
    FreeRTOS-Kernel-Heap3
    pico_bootsel_via_double_reset
    pio_i80
    hardware_i2c
    hardware_pwm
    lvgl lvgl::demos lvgl::examples
    factory_test
    )

# add target common defines here
target_compile_definitions(${PROJECT_NAME} PUBLIC DEFAULT_SYS_CLK_KHZ=${SYS_CLK_KHZ})
target_compile_definitions(${PROJECT_NAME} PUBLIC DEFAULT_PERI_CLK_KHZ=${PERI_CLK_KHZ})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_PIN_DB_BASE=${LCD_PIN_DB_BASE})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_PIN_DB_COUNT=${LCD_PIN_DB_COUNT})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_PIN_CS=${LCD_PIN_CS})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_PIN_WR=${LCD_PIN_WR})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_PIN_RS=${LCD_PIN_RS})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_PIN_RD=${LCD_PIN_RD})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_PIN_RST=${LCD_PIN_RST})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_PIN_BL=${LCD_PIN_BL})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_ROTATION=${LCD_ROTATION})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_HOR_RES=${LCD_HOR_RES})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_VER_RES=${LCD_VER_RES})
target_compile_definitions(${PROJECT_NAME} PUBLIC DISP_OVER_PIO=${DISP_OVER_PIO})
target_compile_definitions(${PROJECT_NAME} PUBLIC MY_DISP_BUF_SIZE=${MY_DISP_BUF_SIZE})

# TFT drivers
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_DRV_USE_ILI9488=${LCD_DRV_USE_ILI9488})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_DRV_USE_ILI9806=${LCD_DRV_USE_ILI9806})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_DRV_USE_R61581=${LCD_DRV_USE_R61581})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_DRV_USE_ST6201=${LCD_DRV_USE_ST6201})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_DRV_USE_1P5623=${LCD_DRV_USE_1P5623})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_DRV_USE_LG4572B=${LCD_DRV_USE_LG4572B})
target_compile_definitions(${PROJECT_NAME} PUBLIC LCD_DRV_USE_ST7789=${LCD_DRV_USE_ST7789})

# Input drivers
target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USED=${INDEV_DRV_USED})
target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_FT6236=${INDEV_DRV_USE_FT6236})
target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_NS2009=${INDEV_DRV_USE_NS2009})
target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_TSC2007=${INDEV_DRV_USE_TSC2007})
target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_GT911=${INDEV_DRV_USE_GT911})

# Note: If you are using a NOR flash like "w25q16". Just keep the following content.
# The maximum speed of "w25q16" is 133MHz, However, the clock speed of XIP QSPI is divided from "sys_clk".
# So, when your "sys_clk" is greater than 266MHz and default PICO_FLASH_SPI_CLKDIV=2, It will exceed the
# maximum speed, because PICO_FLASH_SPI_CLKDIV must be even, So 4 is good for most purpose, Otherwise,
# nothing should be done. These things will only useful when you overclocking the rp2040.
if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
    if(${SYS_CLK_KHZ} GREATER 266000)
        set(PICO_FLASH_SPI_CLKDIV 4)
    else()
        set(PICO_FLASH_SPI_CLKDIV 2)
    endif()
elseif(${PICO_BOARD} STREQUAL  "pico2" OR ${PICO_PLATFORM} STREQUAL  "rp2350")
    if(${SYS_CLK_KHZ} GREATER 366000)
        set(PICO_FLASH_SPI_CLKDIV 4)
    else()
        set(PICO_FLASH_SPI_CLKDIV 2)
    endif()
endif()

math(EXPR FLASH_CLK_KHZ "${SYS_CLK_KHZ} / ${PICO_FLASH_SPI_CLKDIV}")
math(EXPR FLASH_CLK_MHZ "${FLASH_CLK_KHZ} / 1000")
math(EXPR SYS_CLK_MHZ "${SYS_CLK_KHZ} / 1000")
math(EXPR DISP_BUF_SIZE "${MY_DISP_BUF_SIZE} * 2") # rgb565 cost 2 bytes
math(EXPR I80_BUS_WR_CLK_MHZ "${I80_BUS_WR_CLK_KHZ} / 1000")
message(WARNING "
    CPU speed   : ${SYS_CLK_MHZ} MHz
    Flash speed : ${FLASH_CLK_MHZ} MHz
    LCD_X       : ${LCD_HOR_RES}
    LCD_Y       : ${LCD_VER_RES}
    LCD rotation: ${LCD_ROTATION}   //0: normal, 1: 90 degree, 2: 180 degree, 3: 270 degree
    BUS speed   : ${I80_BUS_WR_CLK_MHZ} MHz
    Buffer size : ${DISP_BUF_SIZE} bytes (LVGL Draw Buffer)
")
target_compile_definitions(bs2_default PRIVATE PICO_FLASH_SPI_CLKDIV=${PICO_FLASH_SPI_CLKDIV})
target_compile_definitions(${PROJECT_NAME} PRIVATE FLASH_CLK_KHZ=${FLASH_CLK_KHZ})


pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

pico_add_extra_outputs(${PROJECT_NAME})
