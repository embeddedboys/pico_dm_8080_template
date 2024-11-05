// Copyright (c) 2024 embeddedboys developers

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "tft.h"
#include "debug.h"

#if LCD_DRV_USE_ILI9488

static int tft_ili9488_init_display(struct tft_priv *priv)
{
    pr_debug("%s, writing initial sequence...\n", __func__);
    priv->tftops->reset(priv);
    // dm_gpio_set_value(priv->gpio.rd, 1);
    // mdelay(120);

    // Positive Gamma Control
    write_reg(priv, 0xE0, 0x00, 0x03, 0x09, 0x08, 0x16, 0x0A, 0x3F, 0x78, 0x4C, 0x09, 0x0A, 0x08, 0x16, 0x1A, 0x0F);

    // Negative Gamma Control
    write_reg(priv, 0xE1, 0x00, 0x16, 0x19, 0x03, 0x0F, 0x05, 0x32, 0x45, 0x46, 0x04, 0x0E, 0x0D, 0x35, 0x37, 0x0F);

    write_reg(priv, 0xC0, 0x17, 0x15);          // Power Control 1
    write_reg(priv, 0xC1, 0x41);                // Power Control 2
    write_reg(priv, 0xC5, 0x00, 0x12, 0x80);    // VCOM Control
    write_reg(priv, 0x36, 0x28);                // Memory Access Control
    write_reg(priv, 0x3A, 0x55);                // Pixel Interface Format RGB565 8080 16-bit
    write_reg(priv, 0xB0, 0x00);                // Interface Mode Control

    // Frame Rate Control
    // write_reg(priv, 0xB1, 0xD0, 0x11);       // 60Hz
    write_reg(priv, 0xB1, 0xD0, 0x14);          // 90Hz

    write_reg(priv, 0xB4, 0x02);                // Display Inversion Control
    write_reg(priv, 0xB6, 0x02, 0x02, 0x3B);    // Display Function Control
    write_reg(priv, 0xB7, 0xC6);                // Entry Mode Set
    write_reg(priv, 0xF7, 0xA9, 0x51, 0x2C, 0x82);  // Adjust Control 3
    write_reg(priv, 0x11);                      // Exit Sleep
    mdelay(60);
    write_reg(priv, 0x29);                      // Display on

    return 0;
}

#if LCD_PIN_DB_COUNT == 8
static void tft_video_sync(struct tft_priv *priv, int xs, int ys, int xe, int ye, void *vmem, size_t len)
{
    // printf("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);
    priv->tftops->set_addr_win(priv, xs, ys, xe, ye);

    /* 
     * 8080 8-bit Data Bus for 16-bit/pixel (RGB 5-6-5 Bits Input)
     *      DB 7     R4  G2
     *      DB 6     R3  G1
     *      DB 5     R2  G0
     *      DB 4     R1  B4
     *      DB 3     R0  B3
     *      DB 2     G5  B2
     *      DB 1     G4  B1
     *      DB 0     G3  B0
     * But a 16-bit Data Bus RGB565 order like this:
     * B0 - B4, G0 - G5, R0 - R4 from DB0 to DB16
     * So simply swap 2 bytes here from pixel buffer.
     */
    u16 *p = (u16 *)vmem;
    for (size_t i = 0; i < len; i++)
        p[i] = (p[i] << 8) | (p[i] >> 8);

    write_buf_rs(priv, vmem, len * 2, 1);
}
#endif

static struct tft_display ili9488 = {
    .xres   = TFT_X_RES,
    .yres   = TFT_Y_RES,
    .bpp    = 16,
    .backlight = 100,
    .tftops = {
#if LCD_PIN_DB_COUNT == 8
        .write_reg = tft_write_reg8,
        .video_sync = tft_video_sync,
#else
        .write_reg = tft_write_reg16,
#endif
        .init_display = tft_ili9488_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&ili9488);
    return 0;
}

#endif