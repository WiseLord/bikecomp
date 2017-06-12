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
    PARAM_TOP_SPEED,

    PARAM_TOP_END
} ParamTop;

typedef enum {
    PARAM_MID_TRACK,

    PARAM_MID_END
} ParamMid;

typedef enum {
    PARAM_BTM_DISTANCE,
    PARAM_BTM_TRACKTIME,

    PARAM_BTM_END
} ParamBtm;

typedef struct {
    // Rectangle parameters
    uint16_t left;
    uint16_t top;
    uint16_t right;
    uint16_t bottom;
    // Text label parameters
    uint16_t labX;
    uint16_t labY;
    // LCD font parameters
    const uint8_t *fontMain;
    const uint8_t *fontDeci;
    uint16_t x;
    uint16_t y;
    uint8_t len;
    uint8_t dot;
    char lead;
} SectionPgm;

typedef struct {
    const SectionPgm *pgm;
    char str[PARAM_STRBUF];
} Section;

typedef struct {
    uint16_t color;
    const char* label;
} Param;

void screenShowMain(void);
void screenShowSetup(void);

void screenUpdate(void);

#endif // SCREEN_H
