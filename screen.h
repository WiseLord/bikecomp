#ifndef SCREEN_H
#define SCREEN_H

#include <inttypes.h>

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

    SECTION_END
} Section;

typedef enum {
    PARAM_SPEED,

    PARAM_TRACK,
    PARAM_DISTANCE,
    PARAM_TRACKTIME,

    PARAM_END
} ParamType;

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
    const uint8_t *fontMain;
    const uint8_t *fontDeci;
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
    uint16_t color;
    const char* label;
} Param;

typedef enum {
    CLEAR_NOTHING,
    CLEAR_ALL,
    CLEAR_LCDDATA,

    CLEAR_END
} ClearMode;

void switchParam(Section section);

void screenShowMain(void);
void screenShowSetup(void);

void screenUpdate(void);

#endif // SCREEN_H
