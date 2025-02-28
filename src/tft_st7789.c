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

#if LCD_DRV_USE_ST7789

static int tft_st7789_init_display(struct tft_priv *priv)
{
    pr_debug("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);
    dm_gpio_set_value(priv->gpio.rd, 1);
    mdelay(150);

    write_reg(priv, 0x11);
    mdelay(120);

    // write_reg(priv, 0x36, 0x00);

    write_reg(priv, 0x3A, 0x05);

    write_reg(priv, 0xB2, 0x0C, 0x0C, 0x00, 0x33, 0x33);

    write_reg(priv, 0xB7, 0x35);

    write_reg(priv, 0xBB, 0x37);

    write_reg(priv, 0xC0, 0x2C);

    write_reg(priv, 0xC2, 0x01);

    write_reg(priv, 0xC3, 0x12);

    //VDV, 0x20:0v
    write_reg(priv, 0xC4, 0x20);

    //0x0F:60Hz
    write_reg(priv, 0xC6, 0x0F);

    write_reg(priv, 0xD0, 0xA4, 0xA1);

    //after sleeping in，gate output as GND
    write_reg(priv, 0xD6, 0xA1);

    write_reg(priv, 0xE0, 0xD0, 0x08, 0x0E, 0x09, 0x09, 0x05, 0x31, 0x33, 0x48, 0x17, 0x14, 0x15, 0x31, 0x34);

    write_reg(priv, 0xE1, 0xD0, 0x08, 0x0E, 0x09, 0x09, 0x15, 0x31, 0x33, 0x48, 0x17, 0x14, 0x15, 0x31, 0x34);

    write_reg(priv, 0x21);
    write_reg(priv, 0x29);

    return 0;
}

/* TODO: update the rotation properly */
static int tft_set_dir(struct tft_priv *priv, u8 dir)
{
    printf("setting display rotation to %d\n", dir);

    switch (dir) {
    case LCD_ROTATE_0:
        write_reg(priv, MADCTL, BGR);
        break;
    case LCD_ROTATE_90:
        write_reg(priv, MADCTL, MV);
        break;
    case LCD_ROTATE_180:
        write_reg(priv, MADCTL, MY);
        break;
    case LCD_ROTATE_270:
        write_reg(priv, MADCTL, MY | MX | MV);
        break;
    default:
        break;
    }

    return 0;
}

static struct tft_display st7789 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 16,
    .rotate = LCD_ROTATION,
    .backlight = 100,
    .tftops = {
        .write_reg    = tft_write_reg8,
        .set_dir = tft_set_dir,
        .init_display = tft_st7789_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&st7789);
    return 0;
}

#endif