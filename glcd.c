#include "glcd.h"

GlcdOpts glcdOpts;

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
