#include "glcd.h"

#include <avr/pgmspace.h>
#include <util/delay.h>

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

    glcdDrawPixel(x0, y0 + r, color);
    glcdDrawPixel(x0, y0 - r, color);
    glcdDrawPixel(x0 + r, y0, color);
    glcdDrawPixel(x0 - r, y0, color);

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

void glcdSetXY(uint16_t x, uint16_t y)
{
    glcdOpts.x = x;
    glcdOpts.y = y;
}

void glcdSetY(uint16_t y)
{
    glcdOpts.y = y;
}

void glcdLoadFont(const uint8_t *font, uint16_t color, uint16_t bgColor)
{
    glcdOpts.font = font + FONT_HEADER_END;

    memcpy_P(&glcdOpts.fp, font, FONT_HEADER_END);
    glcdOpts.fp.color = color;
    glcdOpts.fp.bgColor = bgColor;
}

void glcdWriteChar(uint8_t code)
{
    uint8_t i;

    uint8_t spos = code - ((code >= 128) ? glcdOpts.fp.oftna : glcdOpts.fp.ofta);

    uint16_t oft = 0;               // Current symbol offset in array
    uint8_t swd = 0;                // Current symbol width
    uint8_t fwd = glcdOpts.fp.fwd;           // Fixed width

    for (i = 0; i < spos; i++) {
        swd = pgm_read_byte(glcdOpts.font + i);
        oft += swd;
    }
    swd = pgm_read_byte(glcdOpts.font + spos);
    if (!fwd)
        fwd = swd;

    oft *= glcdOpts.fp.height;
    oft += glcdOpts.fp.ccnt;

#if GLCD_TYPE == 9341
    ili9341SetWindow(glcdOpts.x, glcdOpts.y, glcdOpts.x + fwd - 1, glcdOpts.y + glcdOpts.fp.height * 8 - 1);
    ili9341WriteChar(glcdOpts.font + oft, fwd, swd);
#elif GLCD_TYPE == 1306
    glcdSetXY(_x, _y);
    ssd1306WriteChar(glcdOpts.font + oft, fwd, swd);
#else
#error "Implement _WriteChar in display driver!"
#endif

    glcdSetXY(glcdOpts.x + fwd, glcdOpts.y);
}

void glcdWriteIcon(const uint8_t *icon, uint16_t color, uint16_t bgColor)
{
#if GLCD_TYPE == 9341
    ili9341SetWindow(glcdOpts.x, glcdOpts.y, glcdOpts.x + pgm_read_byte(&icon[0]) - 1, glcdOpts.y + pgm_read_byte(&icon[1]) - 1);
    ili9341WriteIcon(icon, color, bgColor);
#elif GLCD_TYPE == 1306
    ssd1306WriteIcon(icon, color, bgColor);
#else
#error "Implement _WriteIcon in display driver!"
#endif
}

void glcdWriteString(char *string)
{
    if (*string)
        glcdWriteChar(*string++);
    while (*string) {
        glcdWriteChar(glcdOpts.fp.ltspPos);
        glcdWriteChar(*string++);
    }
}

void glcdLoadLcdFont(const uint8_t *font, uint16_t color, uint16_t bgColor)
{
    glcdOpts.font = font + FONT_LCD_HEADER_END;

    memcpy_P(&glcdOpts.fp, font, FONT_LCD_HEADER_END);
    glcdOpts.fp.color = color;
    glcdOpts.fp.bgColor = bgColor;
}

void glcdSkipLcdChar(uint8_t code)
{
    if ((code >= '0' && code <= '9') ||
            (code == ' ') || (code == '-') ||
            (code >= 'A' && code <= 'F') ||
            (code >= 'a' && code <= 'f')) {
        glcdSetXY(glcdOpts.x + glcdOpts.fp.width + glcdOpts.fp.thickness, glcdOpts.y);
    } else if (code == '.' || code == ':') {
        glcdSetXY(glcdOpts.x + 2 * glcdOpts.fp.thickness, glcdOpts.y);
    }
}

void glcdWriteLcdChar(uint8_t code)
{
    uint8_t dirMask = 0b01001001; // 1 - vertical, 0 - horisontal segment
    uint16_t segColor;

    if (code >= '0' && code <= '9') {
        code = pgm_read_byte(lcdChar + (code - '0'));
    } else if (code == ' ') {
        code = pgm_read_byte(lcdChar + 17);
    } else if (code == '-') {
        code = pgm_read_byte(lcdChar + 16);
    } else if (code == '.' || code == ':') {
        const uint8_t *digStart = glcdOpts.font + 7 * (glcdOpts.fp.thickness * 2 + 1);
        uint8_t startLine = pgm_read_byte(digStart++);
        uint8_t point1, point2;
        for (uint8_t line = 0; line < glcdOpts.fp.thickness; line++) {
            point1 = pgm_read_byte(digStart++);
            point2 = pgm_read_byte(digStart++);
            if (code == '.') {
                glcdDrawHorizLine(glcdOpts.x + point1, glcdOpts.x + point2, glcdOpts.y + startLine + line, glcdOpts.fp.color);
            } else {
                glcdDrawHorizLine(glcdOpts.x + point1, glcdOpts.x + point2, glcdOpts.y + startLine - 2 * glcdOpts.fp.thickness + line, glcdOpts.fp.color);
                glcdDrawHorizLine(glcdOpts.x + point1, glcdOpts.x + point2, glcdOpts.y + 2 * glcdOpts.fp.thickness + line, glcdOpts.fp.color);
            }
        }
        glcdSetXY(glcdOpts.x + 2 * glcdOpts.fp.thickness, glcdOpts.y);
        return;
    } else if (code >= 'A' && code <= 'F') {
        code = pgm_read_byte(lcdChar + (code - 'A' + 10));
    } else if (code >= 'a' && code <= 'f') {
        code = pgm_read_byte(lcdChar + (code - 'a' + 10));
    } else {
        return;
    }

    for (uint8_t seg = 0; seg < 7; seg++) {
        segColor = code & (1 << seg) ? glcdOpts.fp.color : glcdOpts.fp.bgColor;
        const uint8_t *digStart = glcdOpts.font + seg * (glcdOpts.fp.thickness * 2 + 1);
        uint8_t startLine = pgm_read_byte(digStart++);
        uint8_t point1, point2;
        for (uint8_t line = 0; line < glcdOpts.fp.thickness; line++) {
            point1 = pgm_read_byte(digStart++);
            point2 = pgm_read_byte(digStart++);
            if (dirMask & (1 << seg)) {
                glcdDrawHorizLine(glcdOpts.x + point1, glcdOpts.x + point2, glcdOpts.y + startLine + line, segColor);
            } else {
                glcdDrawVertLine(glcdOpts.x + startLine + line, glcdOpts.y + point1, glcdOpts.y + point2, segColor);
            }
        }
    }
    glcdSetXY(glcdOpts.x + glcdOpts.fp.width + glcdOpts.fp.thickness, glcdOpts.y);
}

void glcdWriteLcdString(char *string)
{
    while (*string) {
        glcdWriteLcdChar(*string++);
    }
}
