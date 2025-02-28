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

#ifndef __TFT_H
#define __TFT_H

#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

struct tft_priv;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

enum {
    LCD_ROTATE_0,
    LCD_ROTATE_90,
    LCD_ROTATE_180,
    LCD_ROTATE_270
};
#define BIT(nr)			(1UL << (nr))
#define MADCTL 0x36
#define MY  BIT(7)
#define MX  BIT(6)
#define MV  BIT(5)
#define ML  BIT(4)
#define BGR BIT(3)
#define MH  BIT(2)
#define FH  BIT(1)
#define FV  BIT(0)

struct tft_ops {
    void (*write_reg)(struct tft_priv *priv, int len, ...);
    void (*write_vmem)(struct tft_priv *priv, void *vmem, size_t len);

    int (*init_display)(struct tft_priv *priv);
    int (*reset)(struct tft_priv *priv);
    int (*clear)(struct tft_priv *priv, u16 clear);
    int (*sleep)(struct tft_priv *priv, bool on);
    int (*set_dir)(struct tft_priv *priv, u8 dir);
    int (*set_backlight)(struct tft_priv *priv, uint level);
    void (*set_addr_win)(struct tft_priv *priv, int xs, int ys, int xe, int ye);
    void (*video_sync)(struct tft_priv *priv, int xs, int ys, int xe, int ye, void *vmem, size_t len);
};

struct tft_display {
    u32                     xres;
    u32                     yres;
    u32                     bpp;
    u32                     rotate;
    u32                     backlight;

    struct tft_ops          tftops;
};

struct tft_priv {
    u8                      *buf;

    struct {
        int reset;
        int cs;   /* chip select */
        int rs;   /* register/data select */
        int wr;   /* write signal */
        int rd;   /* read signal */
        int bl;   /* backlight */
        int db[LCD_PIN_DB_COUNT];
    } gpio;
    
    /* device specific */
    struct tft_display    *display;
    struct tft_ops        *tftops;
} __attribute__((__aligned__(4)));

struct video_frame {
    int xs;
    int ys;
    int xe;
    int ye;
    void *vmem;
    size_t len;
};

#define TFT_REG_BUF_SIZE 64
#define TFT_X_RES LCD_HOR_RES
#define TFT_Y_RES LCD_VER_RES
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define dm_gpio_set_value(p,v) gpio_put(p, v)
#define mdelay(v) busy_wait_ms(v)

extern int tft_probe(struct tft_display *display);
extern int tft_driver_init();

extern int i80_pio_init(uint8_t db_base, uint8_t db_count, uint8_t pin_wr);
extern int i80_write_buf_rs(void *buf, size_t len, bool rs);

extern void fbtft_write_gpio16_wr_rs(struct tft_priv *priv, void *buf, size_t len, bool rs);

/* rs=0 means writing register, rs=1 means writing data */
#if DISP_OVER_PIO
    #define write_buf_rs(p, b, l, r) i80_write_buf_rs(b, l, r)
#else
    #define write_buf_rs(p, b, l, r) fbtft_write_gpio16_wr_rs(p, b, l, r)
#endif

extern void tft_video_flush(int xs, int ys, int xe, int ye, void *vmem, uint32_t len);
extern void tft_async_video_flush(struct video_frame *vf);

extern void tft_write_reg(struct tft_priv *priv, int len, ...);
#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__}) / sizeof(int))

extern void tft_write_reg8(struct tft_priv *priv, int len, ...);
extern void tft_write_reg16(struct tft_priv *priv, int len, ...);

// #if LCD_PIN_DB_COUNT == 8
// #define write_reg(priv, ...) \
//     tft_write_reg8(priv, NUMARGS(__VA_ARGS__), __VA_ARGS__)
// #elif LCD_PIN_DB_COUNT == 16
// #define write_reg(priv, ...) \
//     tft_write_reg16(priv, NUMARGS(__VA_ARGS__), __VA_ARGS__)
// #endif

#define write_reg(priv, ...) \
    priv->tftops->write_reg(priv, NUMARGS(__VA_ARGS__), __VA_ARGS__)

extern QueueHandle_t xToFlushQueue;
extern void call_lv_disp_flush_ready(void);
extern portTASK_FUNCTION(video_flush_task, pvParameters);

#endif