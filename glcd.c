#include "glcd.h"

#include <avr/pgmspace.h>
#include <util/delay.h>

GlcdOpts glcdOpts;

FontParam fp;
FontLcdParam flp;

static const uint8_t *_font;
static const uint8_t *_fontLcd = font_lcd_90;

static int16_t _x, _y;

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
    _x = x;
    _y = y;
}

void glcdSetY(uint16_t y)
{
    _y = y;
}

void glcdLoadFont(const uint8_t *font, uint16_t color, uint16_t bgColor)
{
    _font = font + FONT_HEADER_END;

    memcpy_P(&fp, font, FONT_HEADER_END);
    fp.color = color;
    fp.bgColor = bgColor;
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

#if GLCD_TYPE == 9341
    ili9341SetWindow(_x, _y, _x + fwd - 1, _y + fp.height * 8 - 1);
    ili9341WriteChar(_font + oft, fwd, swd);
#else
#error "Implement _WriteChar in display driver!"
#endif

    glcdSetXY(_x + fwd, _y);
}

void glcdWriteIcon(const uint8_t *icon, uint16_t color, uint16_t bgColor)
{
#if GLCD_TYPE == 9341
    ili9341SetWindow(_x, _y, _x + pgm_read_byte(&icon[0]) - 1, _y + pgm_read_byte(&icon[1]) - 1);
    ili9341WriteIcon(icon, color, bgColor);
#else
#error "Implement _WriteIcon in display driver!"
#endif
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

void glcdLoadLcdFont(const uint8_t *font, uint16_t color, uint16_t bgColor)
{
    _fontLcd = font + FONT_LCD_HEADER_END;

    memcpy_P(&flp, font, FONT_LCD_HEADER_END);
    flp.color = color;
    flp.bgColor = bgColor;
}

void glcdSkipLcdChar(uint8_t code)
{
    if ((code >= '0' && code <= '9') ||
            (code == ' ') || (code == '-') ||
            (code >= 'A' && code <= 'F') ||
            (code >= 'a' && code <= 'f')) {
        glcdSetXY(_x + flp.width + flp.thickness, _y);
    } else if (code == '.' || code == ':') {
        glcdSetXY(_x + 2 * flp.thickness, _y);
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
        const uint8_t *digStart = _fontLcd + 7 * (flp.thickness * 2 + 1);
        uint8_t startLine = pgm_read_byte(digStart++);
        uint8_t point1, point2;
        for (uint8_t line = 0; line < flp.thickness; line++) {
            point1 = pgm_read_byte(digStart++);
            point2 = pgm_read_byte(digStart++);
            if (code == '.') {
                glcdDrawHorizLine(_x + point1, _x + point2, _y + startLine + line, flp.color);
            } else {
                glcdDrawHorizLine(_x + point1, _x + point2, _y + startLine - 2 * flp.thickness + line, flp.color);
                glcdDrawHorizLine(_x + point1, _x + point2, _y + 2 * flp.thickness + line, flp.color);
            }
        }
        glcdSetXY(_x + 2 * flp.thickness, _y);
        return;
    } else if (code >= 'A' && code <= 'F') {
        code = pgm_read_byte(lcdChar + (code - 'A' + 10));
    } else if (code >= 'a' && code <= 'f') {
        code = pgm_read_byte(lcdChar + (code - 'a' + 10));
    } else {
        return;
    }

    for (uint8_t seg = 0; seg < 7; seg++) {
        segColor = code & (1 << seg) ? flp.color : flp.bgColor;
        const uint8_t *digStart = _fontLcd + seg * (flp.thickness * 2 + 1);
        uint8_t startLine = pgm_read_byte(digStart++);
        uint8_t point1, point2;
        for (uint8_t line = 0; line < flp.thickness; line++) {
            point1 = pgm_read_byte(digStart++);
            point2 = pgm_read_byte(digStart++);
            if (dirMask & (1 << seg)) {
                glcdDrawHorizLine(_x + point1, _x + point2, _y + startLine + line, segColor);
            } else {
                glcdDrawVertLine(_x + startLine + line, _y + point1, _y + point2, segColor);
            }
        }
    }
    glcdSetXY(_x + flp.width + flp.thickness, _y);
}

void glcdWriteLcdString(char *string)
{
    while (*string) {
        glcdWriteLcdChar(*string++);
    }
}
