#ifndef SCREEN_H
#define SCREEN_H

#include <inttypes.h>

#define PARAM_STRBUF    6

typedef enum {
    SCREEN_MAIN,
    SCREEN_STAT,
    SCREEN_SETUP,

    SCREEN_END
} Screen;

typedef struct {
    uint16_t x;
    uint16_t y;
    const uint8_t *fontMain;
    const uint8_t *fontDeci;
    uint16_t color;
    uint16_t bgColor;
    uint8_t len;
    uint8_t dot;
    char lead;
} ParamPgm;

typedef struct {
    const ParamPgm * pgm;
    char str[PARAM_STRBUF];
} Param;

void screenShowMain(void);
void screenShowSetup(void);

void screenUpdate(void);

#endif // SCREEN_H
