#include <avr/pgmspace.h>
#include "fonts.h"

const uint8_t lcdChar[] PROGMEM = {
    CH_0, CH_1, CH_2, CH_3, CH_4,
    CH_5, CH_6, CH_7, CH_8, CH_9,
    CH_A, CH_B, CH_C, CH_D, CH_E, CH_F,
    CH_MINUS, CH_EMPTY
};

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

const uint8_t font_lcd_117[] PROGMEM = {
    117, 65, 13,
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

const uint8_t font_lcd_99[] PROGMEM = {
    99, 55, 11,  // width, height and thickness
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

const uint8_t font_lcd_90[] PROGMEM = {
    90, 50, 10, // width, height and thickness
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

const uint8_t font_lcd_72[] PROGMEM = {
    72, 40, 8,  // width, height and thickness
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

const uint8_t font_lcd_63[] PROGMEM = {
    63, 35, 7,
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

const uint8_t font_lcd_45[] PROGMEM = {
    45, 25, 5,
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

const uint8_t font_lcd_36[] PROGMEM = {
    36, 20, 4,
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

const uint8_t font_lcd_27[] PROGMEM = {
    27, 15, 3,
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
