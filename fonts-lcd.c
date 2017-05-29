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
    65, 117, 13,
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

const uint8_t font_lcd_72[] PROGMEM = {
    40, 72, 8,  // width, height and thickness
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

const uint8_t font_lcd_45[] PROGMEM = {
    25, 45, 5,
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

const uint8_t font_lcd_27[] PROGMEM = {
    15, 27, 3,
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
