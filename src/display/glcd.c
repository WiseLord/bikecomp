#include "glcd.h"

#include <stdlib.h>
#include <string.h>

#include "dispdrv.h"

static Glcd glcd;

static void glcdUnRleImg(const __flash tImage *img, uint8_t *data)
{
    if (img->rle) {
        // Uncompress image to storage
        const __flash uint8_t *inPtr = img->data;
        uint8_t *outPtr = data;

        while (inPtr < img->data + img->size) {
            int8_t size = (int8_t)(*inPtr);
            inPtr++;
            if (size < 0) {
                for (uint8_t i = 0; i < -size; i++) {
                    *outPtr++ = *inPtr++;
                }
            } else if (size > 0) {
                uint8_t data = *inPtr;
                for (uint8_t i = 0; i < size; i++) {
                    *outPtr++ = data;
                }
                inPtr++;
            } else {
                return;
            }
        }
    } else {
#ifdef __AVR__
        memcpy_P(data, img->data, img->size);
#else
        data = (uint8_t *)img->data;
#endif
    }
}

static UChar findSymbolCode(const char **string)
{
    UChar code = 0;
    uint8_t curr = 0;

    const char *str = *string;

    while (*str) {
        uint8_t sym = *str++;

        if ((sym & 0xC0) == 0x80) {         // Not first byte
            code <<= 8;
            code |= sym;
            if (curr) {
                curr--;
            }
        } else {
            code = sym;
            if ((sym & 0x80) == 0x00) {         // one-byte symbol
                curr = 0;
            } else if ((sym & 0xE0) == 0xC0) {  // two-byte symbol
                curr = 1;
            } else if ((sym & 0xF0) == 0xE0) {  // three-byte symbol
                curr = 2;
            } else if ((sym & 0xF8) == 0xF0) {  // four-byte symbol
                curr = 3;
            } else {
                curr = 0;
            }
        }
        if (curr)
            continue;

        *string = str;
        return code;
    }

    *string = 0;
    return BLOCK_CHAR;
}

void glcdInit(GlcdOrientation value)
{
    dispdrvInit();

    glcd.drv = &dispdrv;

    glcdSetOrientation(value);
}

uint8_t glcdGetBus(void)
{
    return dispdrvGetBus();
}

void glcdSetBacklight(bool value)
{
#ifdef _DISP_BCKL_ENABLED
    if (value) {
        SET(DISP_BCKL);
    } else {
        CLR(DISP_BCKL);
    }
#else
    (void)value;
#endif
}

Glcd *glcdGet(void)
{
    return &glcd;
}

void glcdSetOrientation(GlcdOrientation value)
{
    glcd.orientation = value;

    if (glcd.drv->rotate) {
        glcd.drv->rotate(value & GLCD_LANDSCAPE_ROT);
    }

    glcdResetRect();
}

void glcdShift(int16_t pos)
{
    if (glcd.drv->shift) {
        glcd.drv->shift(pos);
    }
}

void glcdSleep(bool value)
{
    if (glcd.drv->sleep) {
        glcd.drv->sleep(value);
    }
}

void glcdSetIdle(bool value)
{
    if (glcd.drv->setIdle) {
        glcd.drv->setIdle(value);
    }
}

void glcdFbSync(void)
{
    if (glcd.drv->fbSync) {
        glcd.drv->fbSync();
    }
}


void glcdSetRect(const GlcdRect *rect)
{
    glcd.rect.x = rect->x;
    glcd.rect.y = rect->y;
    glcd.rect.w = rect->w;
    glcd.rect.h = rect->h;
}

void glcdSetRectValues(int16_t x, int16_t y, int16_t w, int16_t h)
{
    glcd.rect.x = x;
    glcd.rect.y = y;
    glcd.rect.w = w;
    glcd.rect.h = h;
}

void glcdResetRect(void)
{
    bool portrate = (glcd.orientation & GLCD_PORTRATE);

    glcd.rect.x = 0;
    glcd.rect.y = 0;
    glcd.rect.w = portrate ? dispdrv.height : dispdrv.width;
    glcd.rect.h = portrate ? dispdrv.width : dispdrv.height;
}

GlcdRect *glcdGetRect(void)
{
    return &glcd.rect;
}

void glcdSetFont(const __flash tFont *font)
{
    glcd.font = font;
}

void glcdSetFontColor(color_t color)
{
    glcd.fontFg = color;
}

void glcdSetFontBgColor(color_t color)
{
    glcd.fontBg = color;
}

void glcdSetFontAlign(GlcdAlign align)
{
    glcd.fontAlign = align;
}

int16_t glcdGetFontHeight(const __flash tFont *font)
{
    return font->chars->image->height;
}

void glcdSetXY(int16_t x, int16_t y)
{
    glcd.x = x;
    glcd.y = y;
}

void glcdSetX(int16_t x)
{
    glcd.x = x;
}
void glcdSetY(int16_t y)
{
    glcd.y = y;
}

int16_t glcdFontSymbolPos(UChar code)
{
    int16_t sPos = -1;

    const __flash tFont *font = glcd.font;
    for (int16_t i = 0; i < font->length; i++) {
        if (font->chars[i].code == code) {
            return i;
        }
        if (font->chars[i].code == BLOCK_CHAR) {
            sPos = i;
        }
    }

    return sPos;
}

UChar glcdFontSymbolCode(int16_t pos)
{
    const __flash tFont *font = glcd.font;

    if (pos >= 0 && pos < font->length) {
        return font->chars[pos].code;
    }

    return BLOCK_CHAR;
}

void glcdDrawImage(const __flash tImage *img, color_t color, color_t bgColor)
{
    if (img == NULL) {
        return;
    }

    uint8_t *unRleData = malloc((size_t)(img->width * ((img->height + 7) / 8)));
    glcdUnRleImg(img, unRleData);

    GlcdRect *rect = &glcd.rect;

    int16_t x = glcd.x;
    int16_t y = glcd.y;

    int16_t w = img->width;
    int16_t h = img->height;

    glcdSetX(x + w);

    int16_t xOft = x > 0 ? 0 : -x;
    int16_t yOft = y > 0 ? 0 : -y;

    x += xOft;
    w -= xOft;

    y += yOft;
    h -= yOft;

    if (x + w > rect->w) {
        w = rect->w - x;
    }
    if (y + h > rect->h) {
        h = rect->h - y;
    }

    if (w <= 0 || h <= 0) {
        free(unRleData);
        return;
    }

    x += rect->x;
    y += rect->y;

    bool portrate = (glcd.orientation & GLCD_PORTRATE);

    dispdrvDrawImage(unRleData, img->width,
                     portrate, x, y,
                     color, bgColor,
                     xOft, yOft, w, h);

    free(unRleData);
}

uint16_t glcdStrToUStr(const char *str, UChar *ustr)
{
    uint16_t len = 0;

    while (*str) {
        *ustr++ = findSymbolCode(&str);
        len++;
    }

    *ustr = 0;

    return len;
}

void glcdUStrToStr(const UChar *ustr, char *str)
{
    while (*ustr) {
        uint32_t uCode = (uint32_t)(*ustr);
        if ((uCode & 0x00000080) == 0x00000000) { // One-byte symbol
            *str++ = (char)((uCode & 0x000000FF) >> 0);
        } else if ((uCode & 0x0000E0C0) == 0x0000C080) { // Two-byte symbol
            *str++ = (char)((uCode & 0x0000FF00) >> 8);
            *str++ = (char)((uCode & 0x000000FF) >> 0);
        } else if ((uCode & 0x00F0C0C0) == 0x00E08080) { // Three-byte symbol
            *str++ = (char)((uCode & 0x00FF0000) >> 16);
            *str++ = (char)((uCode & 0x0000FF00) >> 8);
            *str++ = (char)((uCode & 0x000000FF) >> 0);
        } else if ((uCode & 0xF8C0C0C0) == 0xF0808080) { // Four-byte symbol
            *str++ = (char)((uCode & 0xFF000000) >> 24);
            *str++ = (char)((uCode & 0x00FF0000) >> 16);
            *str++ = (char)((uCode & 0x0000FF00) >> 8);
            *str++ = (char)((uCode & 0x000000FF) >> 0);
        }
        ustr++;
    }
    *str = 0;
}

int16_t glcdWriteUChar(UChar code)
{
    const tImage *img = NULL;

    int16_t pos = glcdFontSymbolPos(code);

    if (pos < 0) {
        return 0;
    }

    img = glcd.font->chars[pos].image;

    glcdDrawImage(img, glcd.fontFg, glcd.fontBg);

    return img->width;
}

int16_t glcdCalcUCharLen(UChar code)
{
    int16_t pos = glcdFontSymbolPos(code);

    if (pos < 0) {
        return 0;
    }

    return glcd.font->chars[pos].image->width;
}

void glcdSetStringFramed(bool framed)
{
    glcd.strFramed = framed;
}

int16_t glcdWriteString(const char *string)
{
    if (string == NULL || glcd.font == 0) {
        return 0;
    }

    if (glcd.fontAlign != GLCD_ALIGN_LEFT) {
        uint16_t sLen = glcdCalcStringLen(string);

        if (glcd.fontAlign == GLCD_ALIGN_CENTER) {
            glcdSetX(glcd.x - sLen / 2);
        } else if (glcd.fontAlign == GLCD_ALIGN_RIGHT) {
            glcdSetX(glcd.x - sLen);
        }

        // Reset align after string finished
        glcd.fontAlign = GLCD_ALIGN_LEFT;
    }

    int16_t sLen = 0;
    int16_t lsp = glcdCalcUCharLen(LETTER_SPACE_CHAR);

    if (glcd.strFramed) {
        glcdDrawRect(glcd.x, glcd.y, lsp, glcd.font->chars[0].image->height, glcd.fontBg);
        sLen += lsp;
        glcd.x += lsp;
    }
    while (*string) {
        sLen += glcdWriteUChar(findSymbolCode(&string));
        if (*string) {
            glcdDrawRect(glcd.x, glcd.y, lsp, glcd.font->chars[0].image->height, glcd.fontBg);
            sLen += lsp;
            glcd.x += lsp;
        }
    }
    if (glcd.strFramed) {
        glcdDrawRect(glcd.x, glcd.y, lsp, glcd.font->chars[0].image->height, glcd.fontBg);
        sLen += lsp;
        glcd.x += lsp;
    }

    return sLen;
}

int16_t glcdCalcStringLen(const char *string)
{
    if (string == NULL || glcd.font == 0) {
        return 0;
    }

    int16_t sLen = 0;
    int16_t lsp = glcdCalcUCharLen(LETTER_SPACE_CHAR);

    if (glcd.strFramed) {
        sLen += 2 * lsp;
    }

    while (*string) {
        sLen += glcdCalcUCharLen(findSymbolCode(&string));
        if (*string) {
            sLen += lsp;
        }
    }

    return sLen;
}

void glcdDrawPixel(int16_t x, int16_t y, color_t color)
{
    GlcdRect *rect = &glcd.rect;

    if (x < 0 || y < 0) {
        return;
    }
    if (x >= rect->w || y >= rect->h) {
        return;
    }

    x += rect->x;
    y += rect->y;

    bool portrate = (glcd.orientation & GLCD_PORTRATE);

    if (portrate) {
        dispdrvDrawPixel(y, dispdrv.height - 1 - x, color);
    } else {
        dispdrvDrawPixel(x, y, color);
    }
}

void glcdDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, color_t color)
{
    GlcdRect *rect = &glcd.rect;

    if (w <= 0 || h <= 0) {
        return;
    }

    int16_t xOft = x > 0 ? 0 : -x;
    int16_t yOft = y > 0 ? 0 : -y;

    x += xOft;
    w -= xOft;

    y += yOft;
    h -= yOft;

    if (x + w > rect->w) {
        w = rect->w - x;
    }
    if (y + h > rect->h) {
        h = rect->h - y;
    }

    if (w <= 0 || h <= 0) {
        return;
    }

    x += rect->x;
    y += rect->y;

    bool portrate = (glcd.orientation & GLCD_PORTRATE);

    if (portrate) {
        dispdrvDrawRect(y, dispdrv.height - w - x, h, w, color);
    } else {
        dispdrvDrawRect(x, y, w, h, color);
    }
}

void glcdDrawVertGrad(int16_t x, int16_t y, int16_t w, int16_t h, color_t *gr)
{
    GlcdRect *rect = &glcd.rect;

    if (w <= 0 || h <= 0) {
        return;
    }

    int16_t xOft = x > 0 ? 0 : -x;
    int16_t yOft = y > 0 ? 0 : -y;

    x += xOft;
    w -= xOft;

    y += yOft;
    h -= yOft;

    if (x + w > rect->w) {
        w = rect->w - x;
    }
    if (y + h > rect->h) {
        h = rect->h - y;
    }

    if (w <= 0 || h <= 0) {
        return;
    }

    x += rect->x;
    y += rect->y;

    bool portrate = (glcd.orientation & GLCD_PORTRATE);

    if (portrate) {
        dispdrvDrawVertGrad(y, dispdrv.height - w - x, h, w, gr);
    } else {
        dispdrvDrawVertGrad(x, y, w, h, gr);
    }
}

void glcdDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, color_t color)
{
    if (x0 == x1) {                 // Vertical
        if (y0 > y1) {              // Swap
            y0 = y0 + y1;
            y1 = y0 - y1;
            y0 = y0 - y1;
        }
        glcdDrawRect(x0, y0, 1, y1 - y0 + 1, color);
    } else if (y0 == y1) {          // Horisontal
        if (x0 > x1) {              // Swap
            x0 = x0 + x1;
            x1 = x0 - x1;
            x0 = x0 - x1;
        }
        glcdDrawRect(x0, y0, x1 - x0 + 1, 1, color);
    } else {
        int16_t sX, sY, dX, dY, err;

        sX = x0 < x1 ? 1 : -1;
        sY = y0 < y1 ? 1 : -1;
        dX = sX > 0 ? x1 - x0 : x0 - x1;
        dY = sY > 0 ? y1 - y0 : y0 - y1;
        err = dX - dY;

        while (x0 != x1 || y0 != y1) {
            glcdDrawPixel(x0, y0, color);
            int16_t err2 = err * 2;
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
}

void glcdDrawFrame(int16_t x, int16_t y, int16_t w, int16_t h, int16_t t, color_t color)
{
    glcdDrawRect(x, y, w - t, t, color);
    glcdDrawRect(x, y + t, t, h - t, color);
    glcdDrawRect(x + t, y + h - t, w - t, t, color);
    glcdDrawRect(x + w - t, y, t, h - t, color);
}

void glcdDrawRFrame(int16_t x, int16_t y, int16_t w, int16_t h, int16_t t, int16_t r, color_t color)
{
    int16_t xc = x + r;
    int16_t yc = y + r;

    int16_t xo = r;
    int16_t xi = xo - t + 1;
    int16_t yo = 0;
    int16_t erro = 1 - xo;
    int16_t erri = 1 - xi;

    while (xo >= yo) {
        glcdDrawLine(xc + xi + w - 2 * r - 1, yc + yo + h - 2 * r - 1, xc + xo + w - 2 * r - 1,
                     yc + yo + h - 2 * r - 1, color);
        glcdDrawLine(xc + yo + w - 2 * r - 1, yc + xi + h - 2 * r - 1, xc + yo + w - 2 * r - 1,
                     yc + xo + h - 2 * r - 1, color);
        glcdDrawLine(xc - xo, yc + yo + h - 2 * r - 1, xc - xi, yc + yo + h - 2 * r - 1, color);
        glcdDrawLine(xc - yo, yc + xi + h - 2 * r - 1, xc - yo, yc + xo + h - 2 * r - 1, color);
        glcdDrawLine(xc - xo, yc - yo, xc - xi, yc - yo, color);
        glcdDrawLine(xc - yo, yc - xo, xc - yo, yc - xi, color);
        glcdDrawLine(xc + xi + w - 2 * r - 1, yc - yo, xc + xo + w - 2 * r - 1, yc - yo, color);
        glcdDrawLine(xc + yo + w - 2 * r - 1, yc - xo, xc + yo + w - 2 * r - 1, yc - xi, color);

        yo++;

        if (erro < 0) {
            erro += 2 * yo + 1;
        } else {
            xo--;
            erro += 2 * (yo - xo + 1);
        }

        if (yo > xo - t + 1) {
            xi = yo;
        } else {
            if (erri < 0) {
                erri += 2 * yo + 1;
            } else {
                xi--;
                erri += 2 * (yo - xi + 1);
            }
        }
    }

    glcdDrawRect(x + r + 1, y, w - 2 * r - 2, t,  color);
    glcdDrawRect(x, y + r + 1, t, h - 2 * r - 2, color);
    glcdDrawRect(x + r + 1, y + h - t, w - 2 * r - 2, t, color);
    glcdDrawRect(x + w - t, y + r + 1, t, h - 2 * r - 2, color);
}

void glcdDrawCircle(int16_t xc, int16_t yc, int16_t r,
                    color_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        glcdDrawLine(xc - x, yc + y, xc + x, yc + y, color);
        glcdDrawLine(xc - x, yc - y, xc + x, yc - y, color);
        glcdDrawLine(xc - y, yc - x, xc - y, yc + x, color);
        glcdDrawLine(xc + y, yc - x, xc + y, yc + x, color);
    }

    glcdDrawRect(xc - x, yc - y, 2 * x, 2 * y, color);
}

void glcdDrawRing(int16_t xc, int16_t yc, int16_t r, int16_t t, color_t color)
{
    int16_t xo = r;
    int16_t xi = xo - t + 1;
    int16_t y = 0;
    int16_t erro = 1 - xo;
    int16_t erri = 1 - xi;

    while (xo >= y) {
        glcdDrawLine(xc + xi, yc + y, xc + xo, yc + y, color);
        glcdDrawLine(xc + y, yc + xi, xc + y, yc + xo, color);
        glcdDrawLine(xc - xo, yc + y, xc - xi, yc + y, color);
        glcdDrawLine(xc - y, yc + xi, xc - y, yc + xo, color);
        glcdDrawLine(xc - xo, yc - y, xc - xi, yc - y, color);
        glcdDrawLine(xc - y, yc - xo, xc - y, yc - xi, color);
        glcdDrawLine(xc + xi, yc - y, xc + xo, yc - y, color);
        glcdDrawLine(xc + y, yc - xo, xc + y, yc - xi, color);

        y++;

        if (erro < 0) {
            erro += 2 * y + 1;
        } else {
            xo--;
            erro += 2 * (y - xo + 1);
        }

        if (y > xo - t + 1) {
            xi = y;
        } else {
            if (erri < 0) {
                erri += 2 * y + 1;
            } else {
                xi--;
                erri += 2 * (y - xi + 1);
            }
        }
    }
}
