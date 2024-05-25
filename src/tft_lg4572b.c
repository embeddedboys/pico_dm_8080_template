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

#if LCD_DRV_USE_LG4572B

static int tft_lg4572b_init_display(struct tft_priv *priv)
{
    pr_debug("%s, writing initial sequence...\n", __func__);
    priv->tftops->reset(priv);
    dm_gpio_set_value(priv->gpio.rd, 1);
    mdelay(150);
    
    write_reg(priv, 0x11);
    mdelay(40);
    write_reg(priv, 0x20);
    write_reg(priv, 0x29);
    write_reg(priv, 0x36, (1 << 5) | (1 << 1));
    write_reg(priv, 0x3A, 0x55);
    
    write_reg(priv, 0xB2, 0x20, 0xC8);
    write_reg(priv, 0xB3, 0x00);
    write_reg(priv, 0xB4, 0x04);
    write_reg(priv, 0xB5, 0x10);
    write_reg(priv, 0xB6, 0x01, 0x18, 0x02, 0x40, 0x10, 0x00);
    write_reg(priv, 0xB7, 0x46, 0x06, 0x0C, 0x00, 0x00);
    
    write_reg(priv, 0xC0, 0x01, 0x11);
    write_reg(priv, 0xC3, 0x07, 0x03, 0x04, 0x05, 0x04);
    write_reg(priv, 0xC4, 0x32, 0x24, 0x10, 0x10, 0x01, 0x0A);
    write_reg(priv, 0xC5, 0x6A);
    write_reg(priv, 0xC6, 0x24, 0x50);
    
    write_reg(priv, 0xD0, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD2, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD4, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD1, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD3, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    
    write_reg(priv, 0xD5, 0x02, 0x76, 0x54, 0x15, 0x12, 0x03, 0x42, 0x43, 0x03);
    mdelay(60);

    return 0;
}

static struct tft_display lg4572b = {
    .xres   = TFT_X_RES,
    .yres   = TFT_Y_RES,
    .bpp    = 16,
    .backlight = 100,
    .tftops = {
        .write_reg = tft_write_reg16,
        .init_display = tft_lg4572b_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&lg4572b);
    return 0;
}

#endif