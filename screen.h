#ifndef SCREEN_H
#define SCREEN_H

#include <inttypes.h>

#include "font7seg.h"

#define PARAM_STRBUF    7

typedef enum {
    SCREEN_MAIN,
    SCREEN_SETUP,

    SCREEN_END
} Screen;

typedef enum {
    SECTION_MAIN_TOP,
    SECTION_MAIN_MID,
    SECTION_MAIN_BTM,

    SECTION_SETUP_TOP,
    SECTION_SETUP_MID,
    SECTION_SETUP_BTM,

    SECTION_END
} Section;

typedef struct {
    // Rectangle parameters
    uint16_t left;
    uint16_t top;
    uint16_t right;
    uint16_t bottom;
    // Text label parameters
    uint16_t labX;
    uint16_t labY;
    // LCD fonts
    const __flash tFont7seg *fontMain;
    const __flash tFont7seg *fontDeci;
} ParamArea;

typedef struct {
    // LCD text parameters
    uint16_t x;
    uint16_t y;
    uint8_t len;
    uint8_t dot;
    char lead;
} LcdText;

typedef struct {
    LcdText hour;
    LcdText minSec;
} LcdTimeText;

typedef struct {
    uint16_t color;
    const char* label;
} ParamData;

typedef enum {
    CLEAR_NOTHING   = 0b00,
    CLEAR_LCDDATA   = 0b01,
    CLEAR_ICON      = 0b10,
    CLEAR_ALL       = 0b11,

    CLEAR_END
} ClearMode;

typedef enum {
    COLOR_MODE_FULL_COLOR,
    COLOR_MODE_WHITE_ON_BLACK,
    COLOR_MODE_BLACK_ON_WHITE,

    COLOR_MODE_END
} ColorMode;

void screenInit(void);
void switchParam(Section section);

void switchParamSetup(void);
void diffParamSetup(int8_t value);

void screenShowMain(ClearMode clear);
void screenShowSetup(ClearMode clear);

void screenUpdate(void);
Screen screenGet(void);

#endif // SCREEN_H
