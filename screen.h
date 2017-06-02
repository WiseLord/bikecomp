#ifndef SCREEN_H
#define SCREEN_H

#include <inttypes.h>

typedef enum {
    SCREEN_MAIN,
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
    char str[6];
} Param;

void showScreenMain(void);
void showScreenSetup(void);

#endif // SCREEN_H
