#include <dgfx.h>

#define clamp(x, upper, lower) (x >= upper ? upper : (x < lower ? lower : x))

#define set_draw_coordinates(draw_x, draw_y, draw_width, draw_height) \
    draw_x = draw_x < 0 ? 0 : draw_x; \
    draw_y = draw_y < 0 ? 0 : draw_y;

#define lower_right_corner_of(sprite) \
    (&(sprite->data[sprite->width * sprite->height]))

#define pointer_to_pixel_of(sprite, x, y) \
    (&(sprite->data[sprite->width * (y) + (x)]))

void get_clipping_bounds(int x, int y, int* clipped_width, int* clipped_height, int draw_width, int draw_height) {
    *clipped_width  = draw_width;
    *clipped_height = draw_height;

    int draw_x = x;
    int draw_y = y;

    // clipped on the left side
    if (draw_x < 0) {
        *clipped_width += draw_x;
        draw_x          = 0;
    }
    if ((draw_x + *clipped_width) > LCD_WIDTH) {
        *clipped_width = LCD_WIDTH - draw_x;
    }

    *clipped_width = *clipped_width < 0 ? 0 : *clipped_width;

    // clipped on the top
    if (draw_y < 0) {
        *clipped_height += draw_y;
        draw_y = 0;
        // reset draw_y, because we've accounted for clipping on the top already
        // if this is not done, then we might accidentally account for top clipping twice
    }
    if ((draw_y + *clipped_height) > LCD_HEIGHT) {
        *clipped_height = LCD_HEIGHT - (draw_y > 0 ? draw_y : 0);
    }

    *clipped_height = *clipped_height < 0 ? 0 : *clipped_height;
}

void dgfx_Sprite_FlipX(gfx_sprite_t* sprite, int x, int y) {
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;

    int width = sprite->width, height = sprite->height;
    get_clipping_bounds(x, y, &width, &height, sprite->width, sprite->height);

    // we now need to figure out where to start on the sprite
    // this is much easier, since we're drawing the whole sprite

    uint8_t* source_pointer = &sprite->data[0];
    int dest_offset         = LCD_WIDTH - width;
    int source_offset       = sprite->width + width;
    source_pointer         += sprite->width - 1;

    // we calculate the destination pointer later, to avoid accidentally pointing to something offscreen

    if (x < 0) {
        source_pointer += x;
    }

    if (y < 0) {
        source_pointer -= sprite->width * y;
    }

    int draw_x = x, draw_y = y;
    set_draw_coordinates(draw_x, draw_y, width, height);
    uint8_t* dest_pointer = &(gfx_vbuffer[draw_y][draw_x]);
    int draw_width;

    while (height) {
        draw_width = width;
        while (draw_width) {
            *dest_pointer = *source_pointer;
            source_pointer--;
            dest_pointer++;
            draw_width--;
        }
        dest_pointer   += dest_offset;
        source_pointer += source_offset;
        height--;
    }
}

void dgfx_TransparentSprite_FlipX(gfx_sprite_t* sprite, int x, int y) {
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;

    int width = sprite->width, height = sprite->height;
    get_clipping_bounds(x, y, &width, &height, sprite->width, sprite->height);

    // we now need to figure out where to start on the sprite
    // this is much easier, since we're drawing the whole sprite

    uint8_t* source_pointer = &sprite->data[0];
    int dest_offset         = LCD_WIDTH - width;
    int source_offset       = sprite->width + width;
    source_pointer         += sprite->width - 1;

    // we calculate the destination pointer later, to avoid accidentally pointing to something offscreen

    if (x < 0) {
        source_pointer += x;
    }

    if (y < 0) {
        source_pointer -= sprite->width * y;
    }
    
    int draw_x = x, draw_y = y;
    set_draw_coordinates(draw_x, draw_y, width, height);
    uint8_t* dest_pointer = &(gfx_vbuffer[draw_y][draw_x]);
    int draw_width;
    uint8_t pixel;

    while (height) {
        draw_width = width;
        while (draw_width) {
            pixel = *source_pointer;
            if (pixel) *dest_pointer = pixel;
            source_pointer--;
            dest_pointer++;
            draw_width--;
        }
        dest_pointer   += dest_offset;
        source_pointer += source_offset;
        height--;
    }
}

void dgfx_Sprite_FlipY(gfx_sprite_t* sprite, int x, int y) {
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;

    int width = sprite->width, height = sprite->height;
    get_clipping_bounds(x, y, &width, &height, sprite->width, sprite->height);

    // we now need to figure out where to start on the sprite
    // this is much easier, since we're drawing the whole sprite

    uint8_t* source_pointer = lower_right_corner_of(sprite);
    int source_offset       = sprite->width * sizeof(uint8_t);
    int copy_size           = width * sizeof(uint8_t);

    source_pointer -= source_offset;

    // we calculate the destination pointer later, to avoid accidentally pointing to something offscreen

    if (x < 0) {
        source_pointer += -1 * x;
    }

    if (y < 0) {
        source_pointer += sprite->width * y;
    }

    if (y >= LCD_HEIGHT) {
        source_pointer -= (y - LCD_HEIGHT) * sprite->width;
    }

    int draw_x = x, draw_y = y;
    set_draw_coordinates(draw_x, draw_y, width, height);
    uint8_t* dest_pointer = &(gfx_vbuffer[draw_y][draw_x]);

    while (height) {
        memcpy(dest_pointer, source_pointer, copy_size);
        dest_pointer   += LCD_WIDTH;
        source_pointer -= source_offset;
        height--;
    }
}

void dgfx_TransparentSprite_FlipY(gfx_sprite_t* sprite, int x, int y) {
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;

    int width = sprite->width, height = sprite->height;
    get_clipping_bounds(x, y, &width, &height, sprite->width, sprite->height);

    // we now need to figure out where to start on the sprite
    // this is much easier, since we're drawing the whole sprite

    uint8_t* source_pointer = lower_right_corner_of(sprite);
    int dest_offset         = LCD_WIDTH - width;
    int source_offset       = sprite->width + width;
    source_pointer         -= sprite->width;

    // we calculate the destination pointer later, to avoid accidentally pointing to something offscreen

    if (x < 0) {
        source_pointer += -1 * x;
    }

    if (y < 0) {
        source_pointer += sprite->width * y;
    }

    if (y >= LCD_HEIGHT) {
        source_pointer -= (y - LCD_HEIGHT) * sprite->width;
    }

    int draw_x = x, draw_y = y;
    set_draw_coordinates(draw_x, draw_y, width, height);
    uint8_t* dest_pointer  = &(gfx_vbuffer[draw_y][draw_x]);
    int draw_width         = width;
    uint8_t pixel;

    while (height) {
        draw_width = width;
        while (draw_width) {
            pixel = *source_pointer;
            if (pixel) {
                *dest_pointer = pixel;
            }
            source_pointer++;
            dest_pointer++;
            draw_width--;
        }
        source_pointer -= source_offset;
        dest_pointer   += dest_offset;
        height--;
    }
}

void dgfx_Sprite_FlipX_NoClip(gfx_sprite_t* sprite, int x, int y) {
    uint8_t width  = sprite->width;
    uint8_t height = sprite->height;

    uint8_t* source_pointer = &(sprite->data[width * (height - 1) + width - 1]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[y + height - 1][x]);

    uint8_t counter;

    while (height) {
        counter = width;
        while (counter) {
            *dest_pointer = *source_pointer;
            source_pointer--;
            dest_pointer++;
            counter--;
        }
        dest_pointer -= LCD_WIDTH + width;
        height--;
    }
}

void dgfx_TransparentSprite_FlipX_NoClip(gfx_sprite_t* sprite, int x, int y) {
    uint8_t width  = sprite->width;
    uint8_t height = sprite->height;

    uint8_t* source_pointer = &(sprite->data[width * (height - 1) + width - 1]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[y + height - 1][x]);

    uint8_t counter;
    uint8_t pixel;

    while (height) {
        counter = width;
        while (counter) {
            pixel = *source_pointer;
            if (pixel) *dest_pointer = pixel;
            source_pointer--;
            dest_pointer++;
            counter--;
        }
        dest_pointer -= LCD_WIDTH + width;
        height--;
    }
}

void dgfx_Sprite_FlipY_NoClip(gfx_sprite_t* sprite, int x, int y) {
    uint8_t width  = sprite->width;
    uint8_t height = sprite->height;

    uint8_t* source_pointer = &(sprite->data[width * (height - 1)]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[y][x]);

    size_t copy_size = width * sizeof(uint8_t);
    
    while (height) {
        memcpy(dest_pointer, source_pointer, copy_size);
        height--;
        dest_pointer   += LCD_WIDTH;
        source_pointer -= width;
    }
}

void dgfx_TransparentSprite_FlipY_NoClip(gfx_sprite_t* sprite, int x, int y) {
    uint8_t width  = sprite->width;
    uint8_t height = sprite->height;

    uint8_t* source_pointer = &(sprite->data[width * (height - 1)]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[y][x]);

    uint8_t counter;
    uint8_t pixel;
    
    while (height) {
        counter = width;
        while (counter) {
            pixel = *source_pointer;
            if (pixel) *dest_pointer = pixel;
            source_pointer++;
            dest_pointer++;
            counter--;
        }
        height--;
        dest_pointer   += LCD_WIDTH - width;
        source_pointer -= 2 * width;
    }
}

void dgfx_Sprite_Section(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    if (dest_x >= LCD_WIDTH || dest_y >= LCD_HEIGHT) return;
    int draw_width, draw_height;

    get_clipping_bounds(dest_x, dest_y, &draw_width, &draw_height, width, height);

    uint8_t* source_pointer = pointer_to_pixel_of(sprite, src_x, src_y);
    int source_offset       = sprite->width;
    int copy_size           = sizeof(uint8_t) * draw_width;

    if (dest_x < 0) {
        source_pointer -= dest_x;
    }

    if (dest_y < 0) {
        source_pointer -= sprite->width * dest_y;
    }

    int draw_x = dest_x, draw_y = dest_y;
    set_draw_coordinates(draw_x, draw_y, draw_width, draw_height);
    uint8_t* dest_pointer = &(gfx_vbuffer[draw_y][draw_x]);

    while (draw_height) {
        memcpy(dest_pointer, source_pointer, copy_size);
        dest_pointer   += LCD_WIDTH;
        source_pointer += source_offset;
        draw_height--;
    }
}

void dgfx_TransparentSprite_Section(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    if (dest_x >= LCD_WIDTH || dest_y >= LCD_HEIGHT) return;
    int draw_width, draw_height;

    get_clipping_bounds(dest_x, dest_y, &draw_width, &draw_height, width, height);

    uint8_t* source_pointer = pointer_to_pixel_of(sprite, src_x, src_y);
    int source_offset       = sprite->width - draw_width;

    if (dest_x < 0) {
        source_pointer -= dest_x;
    }

    if (dest_y < 0) {
        source_pointer -= sprite->width * dest_y;
    }

    int draw_x = dest_x, draw_y = dest_y;
    set_draw_coordinates(draw_x, draw_y, draw_width, draw_height);
    uint8_t* dest_pointer = &(gfx_vbuffer[draw_y][draw_x]);
    int dest_offset       = LCD_WIDTH - draw_width;

    int counter;
    uint8_t pixel;
    while (draw_height) {
        counter = draw_width;
        while (counter) {
            pixel = *source_pointer;
            if (pixel) *dest_pointer = pixel;
            dest_pointer++;
            source_pointer++;
            counter--;
        }
        dest_pointer   += dest_offset;
        source_pointer += source_offset;
        draw_height--;
    }
}

void dgfx_Sprite_Section_FlipX(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    if (dest_x >= LCD_WIDTH || dest_y >= LCD_HEIGHT) return;

    int draw_width, draw_height;
    get_clipping_bounds(dest_x, dest_y, &draw_width, &draw_height, width, height);

    // we now need to figure out where to start on the sprite

    uint8_t* source_pointer = pointer_to_pixel_of(sprite, src_x, src_y);
    int dest_offset         = LCD_WIDTH - draw_width;
    int source_offset       = sprite->width + draw_width;
    source_pointer         += width - 1;

    // we calculate the destination pointer later, to avoid accidentally pointing to something offscreen

    if (dest_x < 0) {
        source_pointer += dest_x;
    }

    if (dest_y < 0) {
        source_pointer -= sprite->width * dest_y;
    }

    int draw_x = dest_x, draw_y = dest_y;
    set_draw_coordinates(draw_x, draw_y, width, height);
    uint8_t* dest_pointer = &(gfx_vbuffer[draw_y][draw_x]);
    int counter;

    while (draw_height) {
        counter = draw_width;
        while (counter) {
            *dest_pointer = *source_pointer;
            source_pointer--;
            dest_pointer++;
            counter--;
        }
        dest_pointer   += dest_offset;
        source_pointer += source_offset;
        draw_height--;
    }
}

void dgfx_TransparentSprite_Section_FlipX(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
if (dest_x >= LCD_WIDTH || dest_y >= LCD_HEIGHT) return;

    int draw_width, draw_height;
    get_clipping_bounds(dest_x, dest_y, &draw_width, &draw_height, width, height);

    // we now need to figure out where to start on the sprite

    uint8_t* source_pointer = pointer_to_pixel_of(sprite, src_x, src_y);
    int dest_offset         = LCD_WIDTH - draw_width;
    int source_offset       = sprite->width + draw_width;
    source_pointer         += width - 1;

    // we calculate the destination pointer later, to avoid accidentally pointing to something offscreen

    if (dest_x < 0) {
        source_pointer += dest_x;
    }

    if (dest_y < 0) {
        source_pointer -= sprite->width * dest_y;
    }

    int draw_x = dest_x, draw_y = dest_y;
    set_draw_coordinates(draw_x, draw_y, width, height);
    uint8_t* dest_pointer = &(gfx_vbuffer[draw_y][draw_x]);
    int counter;
    uint8_t pixel;

    while (draw_height) {
        counter = draw_width;
        while (counter) {
            pixel = *source_pointer;
            if (pixel) *dest_pointer = pixel;
            source_pointer--;
            dest_pointer++;
            counter--;
        }
        dest_pointer   += dest_offset;
        source_pointer += source_offset;
        draw_height--;
    }
}

void dgfx_Sprite_Section_FlipY(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    if (dest_x >= LCD_WIDTH || dest_y >= LCD_HEIGHT) return;

    int draw_width, draw_height;
    get_clipping_bounds(dest_x, dest_y, &draw_width, &draw_height, width, height);

    // we now need to figure out where to start on the sprite
    uint8_t* source_pointer = pointer_to_pixel_of(sprite, src_x, src_y + height - 1);
    size_t copy_size        = sizeof(uint8_t) * draw_width;
    int source_offset       = sprite->width;

    if (dest_x < 0) {
        source_pointer -= dest_x;
    }
    if (dest_y < 0) {
        source_pointer += dest_y * sprite->width;
    }

    int draw_x = dest_x, draw_y = dest_y;
    set_draw_coordinates(draw_x, draw_y, draw_width, draw_height);
    uint8_t* dest_pointer = &(gfx_vbuffer[draw_y][draw_x]);

    while (draw_height) {
        memcpy(dest_pointer, source_pointer, copy_size);
        dest_pointer   += LCD_WIDTH;
        source_pointer -= source_offset;
        draw_height--;
    }
}

void dgfx_TransparentSprite_Section_FlipY(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    if (dest_x >= LCD_WIDTH || dest_y >= LCD_HEIGHT) return;

    int draw_width, draw_height;
    get_clipping_bounds(dest_x, dest_y, &draw_width, &draw_height, width, height);

    // we now need to figure out where to start on the sprite
    uint8_t* source_pointer = pointer_to_pixel_of(sprite, src_x, src_y + height - 1);
    int source_offset       = sprite->width + draw_width;

    if (dest_x < 0) {
        source_pointer -= dest_x;
    }
    if (dest_y < 0) {
        source_pointer += dest_y * sprite->width;
    }

    int draw_x = dest_x, draw_y = dest_y;
    set_draw_coordinates(draw_x, draw_y, draw_width, draw_height);
    uint8_t* dest_pointer = &(gfx_vbuffer[draw_y][draw_x]);
    int dest_offset       = LCD_WIDTH - draw_width;

    int counter;
    uint8_t pixel;

    while (draw_height) {
        counter = draw_width;
        while (counter) {
            pixel = *source_pointer;
            if (pixel) *dest_pointer = pixel;
            dest_pointer++;
            source_pointer++;
            counter--;
        }
        dest_pointer   += dest_offset;
        source_pointer -= source_offset;
        draw_height--;
    }
}

void dgfx_Sprite_Section_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    uint8_t* source_pointer = &(sprite->data[src_y * sprite->width + src_x]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[dest_y][dest_x]);

    uint8_t sprite_width = sprite->width;
    uint8_t draw_height  = height;
    size_t draw_size     = width * sizeof(char);

    while (draw_height) {
        memcpy(dest_pointer, source_pointer, draw_size);
        draw_height--;
        source_pointer += sprite_width;
        dest_pointer   += LCD_WIDTH;
    }
}

void dgfx_TransparentSprite_Section_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    uint8_t* source_pointer = &(sprite->data[src_y * sprite->width + src_x]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[dest_y][dest_x]);

    int draw_height   = height;
    int draw_width    = width;
    int source_offset = sprite->width - width;
    int dest_offset   = LCD_WIDTH - width;
    uint8_t pixel;

    while (draw_height) {
        while (draw_width) {
            draw_width--;
            pixel = *source_pointer;
            if (pixel) *dest_pointer = pixel;
            dest_pointer++;
            source_pointer++;
        }

        dest_pointer   += dest_offset;
        source_pointer += source_offset;

        draw_height--;
        draw_width = width;
    }
}

void dgfx_Sprite_Section_FlipX_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    uint8_t* source_pointer = &(sprite->data[src_y * sprite->width + src_x + width - 1]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[dest_y][dest_x]);

    int dest_offset = (LCD_WIDTH - width) * sizeof(uint8_t);
    int src_offset  = (sprite->width + width) * sizeof(uint8_t);
    int draw_height = height;
    int draw_width  = width;

    while (draw_height) {
        while (draw_width) {
            *dest_pointer = *source_pointer;
            dest_pointer++;
            source_pointer--;
            draw_width--;
        }
        source_pointer += src_offset;
        dest_pointer   += dest_offset;
        draw_height--;
        draw_width = width;
    }
}

void dgfx_TransparentSprite_Section_FlipX_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    uint8_t* source_pointer = &(sprite->data[src_y * sprite->width + src_x + width - 1]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[dest_y][dest_x]);

    int dest_offset = (LCD_WIDTH - width) * sizeof(uint8_t);
    int src_offset  = (sprite->width + width) * sizeof(uint8_t);
    int draw_height = height;
    int draw_width  = width;
    uint8_t pixel;

    while (draw_height) {
        while (draw_width) {
            pixel = *source_pointer;
            if (pixel) *dest_pointer = pixel;
            dest_pointer++;
            source_pointer--;
            draw_width--;
        }
        source_pointer += src_offset;
        dest_pointer   += dest_offset;
        draw_height--;
        draw_width = width;
    }
}

void dgfx_Sprite_Section_FlipY_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    size_t draw_size = width * sizeof(uint8_t);

    uint8_t* source_pointer = &(sprite->data[sprite->width * (src_y + height - 1) + src_x]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[dest_y][dest_x]);

    int src_offset  = (sprite->width) * sizeof(uint8_t);
    int draw_height = height;
    while (draw_height) {
        memcpy(dest_pointer, source_pointer, draw_size);
        draw_height--;
        source_pointer -= src_offset;
        dest_pointer   += LCD_WIDTH;
    }
}

void dgfx_TransparentSprite_Section_FlipY_NoClip(gfx_sprite_t* sprite, int src_x, int src_y, int width, int height, int dest_x, int dest_y) {
    uint8_t* source_pointer = &(sprite->data[sprite->width * (src_y + height - 1) + src_x]);
    uint8_t* dest_pointer   = &(gfx_vbuffer[dest_y][dest_x]);

    int src_offset  = (sprite->width + width);
    int dest_offset = (LCD_WIDTH - width);
    int draw_height = height;
    int draw_width  = width;
    uint8_t pixel;

    while (draw_height) {
        while (draw_width) {
            pixel = *source_pointer;
            if (pixel) *dest_pointer = pixel;
            dest_pointer++;
            source_pointer++;
            draw_width--;
        }
        draw_width = width;
        draw_height--;
        source_pointer -= src_offset;
        dest_pointer   += dest_offset;
    }
}