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

#include "indev.h"
#include "debug.h"

static struct indev_priv g_indev_priv;

static bool __indev_is_pressed(struct indev_priv *priv)
{
    if (priv->ops->is_pressed)
        return priv->ops->is_pressed(priv);
}

bool indev_is_pressed(void)
{
    return __indev_is_pressed(&g_indev_priv);
}

static void __indev_set_dir(struct indev_priv *priv, indev_direction_t dir)
{
    priv->dir = dir;

    if (dir & INDEV_DIR_REVERT_X)
        priv->revert_x = true;
    else
        priv->revert_x = false;

    if (dir & INDEV_DIR_REVERT_Y)
        priv->revert_y = true;
    else
        priv->revert_y = false;

    if (dir & INDEV_DIR_SWITCH_XY) {
        priv->ops->read_x = priv->spec->ops.read_y;
        priv->ops->read_y = priv->spec->ops.read_x;

        bool revert_tmp = priv->revert_x;
        priv->revert_x = priv->revert_y;
        priv->revert_y = revert_tmp;

        priv->x_res = priv->spec->y_res;
        priv->y_res = priv->spec->x_res;
    } else {
        priv->ops->read_x = priv->spec->ops.read_x;
        priv->ops->read_y = priv->spec->ops.read_y;
    }
}

void indev_set_dir(indev_direction_t dir)
{
    __indev_set_dir(&g_indev_priv, dir);
}

static u16 __indev_read_x(struct indev_priv *priv)
{
    if (priv->ops->read_x)
        return priv->ops->read_x(priv);
}

u16 indev_read_x(void)
{
    return __indev_read_x(&g_indev_priv);
}

static u16 __indev_read_y(struct indev_priv *priv)
{
    if (priv->ops->read_y)
        return priv->ops->read_y(priv);
}

u16 indev_read_y(void)
{
    return __indev_read_y(&g_indev_priv);
}

static void indev_reset(struct indev_priv *priv)
{
    pr_debug("%s\n", __func__);
    dm_gpio_set_value(priv->spec->pin_rst, 1);
    mdelay(10);
    dm_gpio_set_value(priv->spec->pin_rst, 0);
    mdelay(10);
    dm_gpio_set_value(priv->spec->pin_rst, 1);
    mdelay(10);
}


static void indev_merge_ops(struct indev_ops *dst, struct indev_ops *src)
{
    if (src->write_reg)
        dst->write_reg = src->write_reg;
    if (src->read_reg)
        dst->read_reg = src->read_reg;

    if (src->init)
        dst->init = src->init;
    if (src->reset)
        dst->reset = src->reset;
    if (src->set_dir)
        dst->set_dir = src->set_dir;
    if (src->is_pressed)
        dst->is_pressed = src->is_pressed;
    if (src->read_x)
        dst->read_x = src->read_x;
    if (src->read_y)
        dst->read_y = src->read_y;
}

int indev_probe(struct indev_spec *spec)
{
    struct indev_priv *priv = &g_indev_priv;

    pr_debug("%s\n", __func__);

    priv->spec = spec;

    priv->ops = (struct indev_ops *)malloc(sizeof(struct indev_ops));
    if (!priv->ops) {
        pr_error("failed to allocate memory!\n");
        return -1;
    }

    priv->x_res = spec->x_res;
    priv->y_res = spec->y_res;

    priv->revert_x = false;
    priv->revert_y = false;

    priv->ops->reset = indev_reset;
    priv->ops->set_dir = __indev_set_dir;

    indev_merge_ops(priv->ops, &spec->ops);

    priv->ops->init(priv);

    return 0;
}
