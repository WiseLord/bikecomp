#include <avr/pgmspace.h>
#include "fonts.h"

const uint8_t lcdChar[] PROGMEM = {
    CH_0, CH_1, CH_2, CH_3, CH_4,
    CH_5, CH_6, CH_7, CH_8, CH_9
};

const uint8_t font_lcd_72[] PROGMEM = {
    0, // SEG_A start X line
    3, 33, 2, 32, 1, 32, 0, 31, 1, 31, 3, 30, 5, 30, 7, 29,
    32, // SEG_B start Y line
    7, 29, 5, 30, 3, 31, 1, 32, 0, 33, 1, 34, 2, 34, 3, 33,
    32, // SEG_C start Y line
    42, 64, 41, 66, 40, 68, 39, 70, 38, 71, 37, 70, 37, 69, 38, 68,
    64, // SEG_D start X line
    7, 29, 5, 30, 3, 30, 1, 31, 0, 31, 1, 32, 2, 32, 3, 33,
    0, // SEG_E start Y line
    38, 65, 37, 64, 37, 64, 38, 63, 39, 63, 40, 62, 41, 62, 42, 61,
    0, // SEG_F start Y line
    6, 33, 7, 34, 7, 34, 8, 33, 8, 32, 9, 31, 9, 30, 10, 29,
    32, // SEG_G start X line
    8, 31, 7, 32, 6, 33, 5, 34, 5, 34, 6, 33, 7, 32, 8, 31,
};
