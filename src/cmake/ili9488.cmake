set(LCD_PIN_DB_BASE  0)  # 8080 LCD data bus base pin
set(LCD_PIN_DB_COUNT 16) # 8080 LCD data bus pin count
set(LCD_PIN_CS  29)  # 8080 LCD chip select pin
set(LCD_PIN_WR  19)  # 8080 LCD write pin
set(LCD_PIN_RS  20)  # 8080 LCD register select pin
set(LCD_PIN_RD  29)  # 8080 LCD read pin
set(LCD_PIN_RST 22)  # 8080 LCD reset pin
set(LCD_PIN_BL  28)  # 8080 LCD backlight pin
set(LCD_X_RES 320)  # LCD X resolution
set(LCD_Y_RES 480)  # LCD Y resolution
set(DISP_OVER_PIO 1) # 1: PIO, 0: GPIO
set(PIO_USE_DMA   1)   # 1: use DMA, 0: not use DMA

if(${LCD_PIN_DB_COUNT} EQUAL 8)
    set(I80_BUS_WR_CLK_KHZ 75000)
elseif(${LCD_PIN_DB_COUNT} EQUAL 16)
    set(I80_BUS_WR_CLK_KHZ 50000)
endif()

# Panel Model selection
set(TFT_MODEL_QD3503728 1)
set(TFT_MODEL_ZT350IT008 0)
add_definitions(-DTFT_MODEL_QD3503728=${TFT_MODEL_QD3503728})
add_definitions(-DTFT_MODEL_ZT350IT008=${TFT_MODEL_ZT350IT008})
