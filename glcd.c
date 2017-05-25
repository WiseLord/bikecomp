#include "glcd.h"

#include <avr/pgmspace.h>
#include <util/delay.h>

GlcdOpts glcdOpts;
FontParam fp;

const uint8_t *_font;

static uint16_t _x, _y;

void glcdDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    int16_t sX, sY, dX, dY, err, err2;

    sX = x0 < x1 ? 1 : -1;
    sY = y0 < y1 ? 1 : -1;
    dX = sX > 0 ? x1 - x0 : x0 - x1;
    dY = sY > 0 ? y1 - y0 : y0 - y1;
    err = dX - dY;

    while (x0 != x1 || y0 != y1) {
        glcdDrawPixel(x0, y0, color);
        err2 = err * 2;
        if (err2 > -dY / 2) {
            err -= dY;
            x0 += sX;
        }
        if (err2 < dX) {
            err += dX;
            y0 += sY;
        }
    }
    glcdDrawPixel(x1, y1, color);
}

void glcdDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    glcdDrawHorizLine(x0, x1, y0, color);
    glcdDrawVertLine(x0, y0, y1, color);
    glcdDrawVertLine(x1, y0, y1, color);
    glcdDrawHorizLine(x0, x1, y1, color);
}
void glcdDrawRing(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ili9431DrawPixel(x0, y0 + r, color);
    ili9431DrawPixel(x0, y0 - r, color);
    ili9431DrawPixel(x0 + r, y0, color);
    ili9431DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        glcdDrawPixel(x0 + x, y0 + y, color);
        glcdDrawPixel(x0 - x, y0 + y, color);
        glcdDrawPixel(x0 + x, y0 - y, color);
        glcdDrawPixel(x0 - x, y0 - y, color);

        glcdDrawPixel(x0 + y, y0 + x, color);
        glcdDrawPixel(x0 - y, y0 + x, color);
        glcdDrawPixel(x0 + y, y0 - x, color);
        glcdDrawPixel(x0 - y, y0 - x, color);
    }
}

void glcdDrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    glcdDrawHorizLine(x0 - r, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        glcdDrawHorizLine(x0 - x, x0 + x, y0 + y, color);
        glcdDrawHorizLine(x0 - x, x0 + x, y0 - y, color);
        glcdDrawHorizLine(x0 - y, x0 + y, y0 + x, color);
        glcdDrawHorizLine(x0 - y, x0 + y, y0 - x, color);
    }
}

void glcdWriteChar(uint8_t code)
{
    uint8_t i;

    uint8_t spos = code - ((code >= 128) ? fp.oftna : fp.ofta);

    uint16_t oft = 0;               // Current symbol offset in array
    uint8_t swd = 0;                // Current symbol width
    uint8_t fwd = fp.fwd;           // Fixed width

    for (i = 0; i < spos; i++) {
        swd = pgm_read_byte(_font + i);
        oft += swd;
    }
    swd = pgm_read_byte(_font + spos);
    if (!fwd)
        fwd = swd;

    oft *= fp.height;
    oft += fp.ccnt;

    ili9341SetWindow(_x, _y, _x + fwd - 1, _y + fp.height * 8 - 1);

    ili9341WriteChar(_font + oft, fwd, swd);

    glcdSetXY(_x + fwd, _y);
}

void glcdWriteString(char *string)
{
    if (*string)
        glcdWriteChar(*string++);
    while (*string) {
        glcdWriteChar(fp.ltspPos);
        glcdWriteChar(*string++);
    }
}

void glcdSetXY(uint16_t x, uint16_t y)
{
    _x = x;
    _y = y;
}

void glcdLoadFont(const uint8_t *font, uint16_t color, uint16_t bgColor, FontDirection direction)
{
    _font = font + FONT_HEADER_END;

    fp.height = pgm_read_byte(font + 0);
    fp.ltspPos = pgm_read_byte(font + 1);
    fp.ccnt = pgm_read_byte(font + 2);
    fp.ofta = pgm_read_byte(font + 3);
    fp.oftna = pgm_read_byte(font + 4);
    fp.direction = direction;
    fp.color = color;
    fp.bgColor = bgColor;
}
