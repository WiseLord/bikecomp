#include "font7seg.h"

#include "display/glcd.h"
#include "string.h"

#define BIT_A       (1<<0)
#define BIT_B       (1<<1)
#define BIT_C       (1<<2)
#define BIT_D       (1<<3)
#define BIT_E       (1<<4)
#define BIT_F       (1<<5)
#define BIT_G       (1<<6)
#define BIT_P       (1<<7)

#define CH_0        (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F)
#define CH_1        (BIT_B | BIT_C)
#define CH_2        (BIT_A | BIT_B | BIT_D | BIT_E | BIT_G)
#define CH_3        (BIT_A | BIT_B | BIT_C | BIT_D | BIT_G)
#define CH_4        (BIT_B | BIT_C | BIT_F | BIT_G)
#define CH_5        (BIT_A | BIT_C | BIT_D | BIT_F | BIT_G)
#define CH_6        (BIT_A | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define CH_7        (BIT_A | BIT_B | BIT_C)
#define CH_8        (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define CH_9        (BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G)
#define CH_A        (BIT_A | BIT_B | BIT_C | BIT_E | BIT_F | BIT_G)
#define CH_B        (BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define CH_C        (BIT_A | BIT_D | BIT_E | BIT_F)
#define CH_D        (BIT_B | BIT_C | BIT_D | BIT_E | BIT_G)
#define CH_E        (BIT_A | BIT_D | BIT_E | BIT_F | BIT_G)
#define CH_F        (BIT_A | BIT_E | BIT_F | BIT_G)
#define CH_EMPTY    (0)
#define CH_MINUS    (BIT_G)

static const __flash tFont7seg *font7seg;

static const __flash uint8_t lcdChar[] = {
    CH_0, CH_1, CH_2, CH_3, CH_4,
    CH_5, CH_6, CH_7, CH_8, CH_9,
    CH_A, CH_B, CH_C, CH_D, CH_E, CH_F,
    CH_MINUS, CH_EMPTY
};

void font7segLoad(const __flash tFont7seg *font)
{
    font7seg = font;
}

void font7segSkipChar(char code)
{
    Glcd *glcd = glcdGet();

    if ((code >= '0' && code <= '9') ||
        (code == ' ') || (code == '-') ||
        (code >= 'A' && code <= 'F') ||
        (code >= 'a' && code <= 'f')) {
        glcdSetXY(glcd->x + font7seg->width + font7seg->thickness, glcd->y);
    } else if (code == '.' || code == ':') {
        glcdSetXY(glcd->x + 2 * font7seg->thickness, glcd->y);
    }
}

void font7segWriteChar(char code)
{
    uint8_t dirMask = 0b01001001; // 1 - vertical, 0 - horisontal segment
    uint16_t segColor;

    Glcd *glcd = glcdGet();

    if (code >= '0' && code <= '9') {
        code = lcdChar[code - '0'];
    } else if (code == ' ') {
        code = lcdChar[17];
    } else if (code == '-') {
        code = lcdChar[16];
    } else if (code == '.' || code == ':') {
        const __flash uint8_t *digStart = &font7seg->data[7 * (font7seg->thickness * 2 + 1)];
        uint8_t startLine = *digStart++;
        uint8_t point1, point2;
        for (uint8_t line = 0; line < font7seg->thickness; line++) {
            point1 = *digStart++;
            point2 = *digStart++;
            int16_t y;
            if (code == '.') {
                y = glcd->y + startLine + line;
                glcdDrawLine(glcd->x + point1, y, glcd->x + point2, y, glcd->fontFg);
            } else {
                y = glcd->y + startLine - 2 * font7seg->thickness + line;
                glcdDrawLine(glcd->x + point1, y, glcd->x + point2, y, glcd->fontFg);
                y = glcd->y + 2 * font7seg->thickness + line;
                glcdDrawLine(glcd->x + point1, y, glcd->x + point2, y, glcd->fontFg);
            }
        }
        glcdSetXY(glcd->x + 2 * font7seg->thickness, glcd->y);
        return;
    } else if (code >= 'A' && code <= 'F') {
        code = lcdChar[code - 'A' + 10];
    } else if (code >= 'a' && code <= 'f') {
        code = lcdChar[code - 'a' + 10];
    } else {
        return;
    }

    for (uint8_t seg = 0; seg < 7; seg++) {
        segColor = code & (1 << seg) ? glcd->fontFg : glcd->fontBg;
        const __flash uint8_t *digStart = &font7seg->data[seg * (font7seg->thickness * 2 + 1)];
        uint8_t startLine = *digStart++;
        uint8_t point1, point2;
        for (uint8_t line = 0; line < font7seg->thickness; line++) {
            point1 = *digStart++;
            point2 = *digStart++;
            if (dirMask & (1 << seg)) {
                int16_t y = glcd->y + startLine + line;
                glcdDrawLine(glcd->x + point1, y, glcd->x + point2, y, segColor);
            } else {
                int16_t x = glcd->x + startLine + line;
                glcdDrawLine(x, glcd->y + point1, x, glcd->y + point2, segColor);
            }
        }
    }
    glcdSetXY(glcd->x + font7seg->width + font7seg->thickness, glcd->y);
}

void font7segWriteString(const char *string)
{
    while (*string) {
        font7segWriteChar(*string++);
    }
}

// Font format:
// width, height, thickness,
// Ya - Y position of top horisontal line of segment A
// pairs of Xa1..XaN to draw "thickness" horisontal lines starting from Ya
// Xb - X position of left vertical line of segment B
// pairs of Yb1..YbN to draw "thickness" vertical lines starting from Xb
// Xc - X position of left vertical line of segment C
// pairs of Yc1..YcN to draw "thickness" vertical lines starting from Xc
// Yd - Y position of top horisontal line of segment D
// pairs of Xd1..XdN to draw "thickness" horisontal lines starting from Yd
// Xe - X position of left vertical line of segment E
// pairs of Ye1..YeN to draw "thickness" vertical lines starting from Xe
// Xf - X position of left vertical line of segment F
// pairs of Yf1..YfN to draw "thickness" vertical lines starting from Xf
// Yg - Y position of top horisontal line of segment G
// pairs of Xg1..XgN to draw "thickness" horisontal lines starting from Yg
// Yp - Y position of top horisontal line of dot point
// pairs of Xp1..XpN to draw "thickness" horisontal lines starting from Yp

const __flash uint8_t font_7seg_13_data[] = {
    0,      // A
    4, 55, 3, 55, 2, 54, 1, 54, 0, 53, 0, 53, 0, 52, 2, 52, 4, 51, 6, 51, 8, 50, 10, 50, 12, 49,
    52,     // B
    12, 49, 10, 50, 8, 51, 6, 52, 4, 53, 2, 54, 0, 55, 0, 56, 0, 57, 1, 57, 2, 57, 3, 56, 4, 55,
    52,     // C
    67, 104, 66, 106, 65, 108, 64, 110, 63, 112, 62, 114, 61, 116, 60, 116, 59, 116, 59, 115, 59, 114, 60, 113, 61, 112,
    104,    // D
    12, 49, 10, 50, 8, 50, 6, 51, 4, 51, 2, 52, 0, 52, 0, 53, 0, 53, 1, 54, 2, 54, 3, 55, 4, 55,
    0,      // E
    61, 107, 60, 107, 59, 106, 59, 106, 59, 105, 60, 105, 61, 104, 62, 104, 63, 103, 64, 103, 65, 102, 66, 102, 67, 101,
    0,      // F
    9, 55, 9, 56, 10, 57, 10, 57, 11, 57, 11, 56, 12, 55, 12, 54, 13, 53, 13, 52, 14, 51, 14, 50, 15, 49,
    52,     // G
    13, 51, 12, 52, 11, 53, 10, 54, 9, 55, 8, 56, 8, 56, 8, 56, 9, 55, 10, 54, 11, 53, 12, 52, 13, 51,
    104,    // dot
    3, 9, 2, 10, 1, 11, 0, 12, 0, 12, 0, 12, 0, 12, 0, 12, 0, 12, 0, 12, 1, 11, 2, 10, 3, 9,
};
const __flash tFont7seg font_7seg_13 = {font_7seg_13_data, 65, 117, 13};

const __flash uint8_t font_7seg_11_data[] = {
    0,      // A
    3, 46, 2, 46, 1, 45, 0, 45, 0, 44, 0, 44, 2, 43, 4, 43, 6, 42, 8, 42, 10, 41,
    44,     // B
    10, 42, 8, 43, 6, 44, 4, 45, 2, 46, 0, 47, 0, 48, 0, 48, 1, 48, 2, 47, 3, 46,
    44,     // C
    56, 88, 55, 90, 54, 92, 53, 94, 52, 96, 51, 98, 50, 98, 50, 98, 50, 97, 51, 96, 52, 95,
    88,     // D
    10, 41, 8, 42, 6, 42, 4, 43, 2, 43, 0, 44, 0, 44, 0, 45, 1, 45, 2, 46, 3, 46,
    0,      // E
    52, 90, 51, 90, 50, 89, 50, 89, 50, 88, 51, 88, 52, 87, 53, 87, 54, 86, 55, 86, 56, 85,
    0,      // F
    8, 46, 8, 47, 9, 48, 9, 48, 10, 48, 10, 47, 11, 46, 11, 45, 12, 44, 12, 43, 13, 42,
    44,     // G
    11, 43, 10, 44, 9, 45, 8, 46, 7, 47, 6, 48, 7, 47, 8, 46, 9, 45, 10, 44, 11, 43,
    88,     // dot
    2, 8, 1, 9, 0, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 10, 1, 9, 2, 8,
};
const __flash tFont7seg font_7seg_11 = {font_7seg_11_data, 55, 99, 11};

const __flash uint8_t font_7seg_10_data[] = {
    0,      // A
    3, 42, 2, 41, 1, 41, 0, 40, 0, 40, 1, 39, 3, 39, 5, 38, 7, 38, 9, 37,
    40,     // B
    9, 38, 7, 39, 5, 40, 3, 41, 1, 42, 0, 43, 0, 43, 1, 43, 2, 42, 3, 41,
    40,     // C
    51, 80, 50, 82, 49, 84, 48, 86, 47, 88, 46, 89, 46, 89, 46, 88, 47, 87, 48, 86,
    80,     // D
    9, 37, 7, 38, 5, 38, 3, 39, 1, 39, 0, 40, 0, 40, 1, 41, 2, 41, 3, 42,
    0,      // E
    48, 82, 47, 81, 46, 81, 46, 80, 46, 80, 47, 79, 48, 79, 49, 78, 50, 78, 51, 77,
    0,      // F
    7, 41, 8, 42, 8, 43, 9, 43, 9, 43, 10, 42, 10, 41, 11, 40, 11, 39, 12, 38,
    40,     // G
    10, 39, 9, 40, 8, 41, 7, 42, 6, 43, 6, 43, 7, 42, 8, 41, 9, 40, 10, 39,
    80,     // dot
    2, 7, 1, 8, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 1, 8, 2, 7,
};
const __flash tFont7seg font_7seg_10 = {font_7seg_10_data,  50, 90, 10};

const __flash uint8_t font_7seg_8_data[] = {
    0,      // A
    3, 33, 2, 32, 1, 32, 0, 31, 1, 31, 3, 30, 5, 30, 7, 29,
    32,     // B
    7, 29, 5, 30, 3, 31, 1, 32, 0, 33, 1, 34, 2, 34, 3, 33,
    32,     // C
    42, 64, 41, 66, 40, 68, 39, 70, 38, 71, 37, 70, 37, 69, 38, 68,
    64,     // D
    7, 29, 5, 30, 3, 30, 1, 31, 0, 31, 1, 32, 2, 32, 3, 33,
    0,      // E
    38, 65, 37, 64, 37, 64, 38, 63, 39, 63, 40, 62, 41, 62, 42, 61,
    0,      // F
    6, 33, 7, 34, 7, 34, 8, 33, 8, 32, 9, 31, 9, 30, 10, 29,
    32,     // G
    8, 31, 7, 32, 6, 33, 5, 34, 5, 34, 6, 33, 7, 32, 8, 31,
    64,     // dot
    2, 5, 1, 6, 0, 7, 0, 7, 0, 7, 0, 7, 1, 6, 2, 5,
};
const __flash tFont7seg font_7seg_8 = {font_7seg_8_data, 40, 72, 8};

const __flash uint8_t font_7seg_7_data[] = {
    0,      // A
    3, 28, 2, 28, 1, 27, 0, 27, 1, 26, 3, 26, 5, 26,
    28,     // B
    5, 26, 3, 27, 1, 28, 0, 29, 1, 30, 2, 30, 3, 29,
    28,     // C
    36, 57, 35, 59, 34, 61, 33, 62, 32, 61, 32, 60, 33, 59,
    56,     // D
    5, 26, 3, 26, 1, 26, 0, 27, 1, 27, 2, 28, 3, 28,
    0,      // E
    33, 56, 32, 56, 32, 55, 33, 55, 34, 54, 35, 54, 36, 54,
    0,      // F
    6, 29, 6, 30, 7, 30, 7, 29, 8, 28, 8, 27, 8, 26,
    28,      // G
    7, 27, 6, 28, 5, 29, 4, 30, 5, 29, 6, 28, 7, 27,
    56,      // dot
    1, 5, 0, 6, 0, 6, 0, 6, 0, 6, 0, 6, 1, 5,
};
const __flash tFont7seg font_7seg_7 = {font_7seg_7_data, 35, 63, 7};

const __flash uint8_t font_7seg_6_data[] = {
    0,      // A
    2, 25, 1, 24, 0, 24, 2, 23, 4, 23, 6, 22,
    24,     // B
    6, 23, 4, 24, 2, 25, 0, 26, 1, 25, 2, 24,
    24,     // C
    31, 47, 30, 49, 29, 51, 28, 53, 28, 52, 29, 51,
    48,     // D
    6, 22, 4, 23, 2, 23, 0, 24, 1, 24, 2, 25,
    0,      // E
    29, 49, 28, 48, 27, 48, 28, 47, 29, 47, 30, 46,
    0,      // F
    4, 24, 5, 25, 5, 25, 6, 24, 6, 23, 7, 22,
    24,      // G
    6, 22, 5, 23, 4, 24, 5, 25, 6, 24, 7, 23,
    48,      // dot
    1, 4, 0, 5, 0, 5, 0, 5, 0, 5, 1, 4,
};
const __flash tFont7seg font_7seg_6 = {font_7seg_6_data, 30, 54, 6};

const __flash uint8_t font_7seg_5_data[] = {
    0,      // A
    2, 20, 1, 20, 0, 19, 2, 19, 4, 18,
    20,     // B
    4, 18, 2, 19, 0, 20, 1, 21, 2, 20,
    20,     // C
    26, 40, 25, 42, 24, 44, 23, 43, 24, 42,
    40,     // D
    4, 18, 2, 19, 0, 19, 1, 20, 2, 20,
    0,      // E
    24, 40, 23, 40, 24, 39, 25, 39, 26, 38,
    0,      // F
    4, 20, 4, 21, 5, 20, 5, 19, 6, 18,
    20,     // G
    5, 19, 4, 20, 3, 21, 4, 20, 5, 19,
    40,     // dot
    1, 3, 0, 4, 0, 4, 0, 4, 1, 3,
};
const __flash tFont7seg font_7seg_5 = {font_7seg_5_data, 25, 45, 5};

const __flash uint8_t font_7seg_4_data[] = {
    0,      // A
    2, 15, 1, 15, 0, 14, 2, 14,
    16,     // B
    2, 15, 0, 16, 1, 17, 2, 16,
    16,     // C
    21, 33, 20, 35, 19, 34, 20, 33,
    32,     // D
    2, 14, 0, 14, 1, 15, 2, 15,
    0,      // E
    19, 31, 18, 31, 19, 30, 20, 30,
    0,      // F
    4, 15, 4, 16, 5, 15, 5, 14,
    16,     // G
    3, 15, 2, 16, 3, 17, 4, 16,
    32,     // dot
    1, 2, 0, 3, 0, 3, 1, 2
};
const __flash tFont7seg font_7seg_4 = {font_7seg_4_data, 20, 36, 4};

const __flash uint8_t font_7seg_3_data[] = {
    0,      // A
    1, 13, 2, 12, 3, 11,
    12,     // B
    3, 11, 2, 12, 1, 11,
    12,     // C
    15, 23, 14, 24, 15, 25,
    24,     // D
    3, 11, 2, 12, 1, 13,
    0,      // E
    15, 25, 14, 24, 15, 23,
    0,      // F
    1, 11, 2, 12, 3, 11,
    12,     // G
    3, 11, 2, 12, 3, 11,
    24,     // dot
    0, 3, 0, 3, 0, 3,
};
const __flash tFont7seg font_7seg_3 = {font_7seg_3_data, 15, 27, 3};
