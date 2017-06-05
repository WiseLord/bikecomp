#include "screen.h"

#include <avr/pgmspace.h>
#include "glcd.h"
#include "measure.h"

#define STR_BUFSIZE                     20

static Screen screen = SCREEN_END;

static char strbuf[STR_BUFSIZE + 1];           // String buffer

static const ParamPgm speedPgm PROGMEM = {
    48, 4,
    font_lcd_99, font_lcd_72,
    LCD_COLOR_AQUA, LCD_COLOR_BLACK,
    4, 1, ' ',
};
static Param speed = { &speedPgm };

static const ParamPgm trackPgm PROGMEM = {
    57, 148,
    font_lcd_63, font_lcd_45,
    LCD_COLOR_YELLOW, LCD_COLOR_BLACK,
    5, 1, ' ',
};
static Param track = { &trackPgm };

static const ParamPgm cadencePgm PROGMEM = {
    9, 244,
    font_lcd_63, font_lcd_45,
    LCD_COLOR_LIGHT_CORAL, LCD_COLOR_BLACK,
    6, 1, ' ',
};
static Param cadence = { &cadencePgm };


static const ParamPgm distancePgm PROGMEM = {
    48, 4,
    font_lcd_99, font_lcd_72,
    LCD_COLOR_AQUA, LCD_COLOR_BLACK,
    4, 1, ' ',
};
static Param distance = { &distancePgm };

static const ParamPgm avgSpeedPgm PROGMEM = {
    57, 148,
    font_lcd_63, font_lcd_45,
    LCD_COLOR_YELLOW, LCD_COLOR_BLACK,
    5, 1, ' ',
};
static Param avgSpeed = { &avgSpeedPgm };

static const ParamPgm durationPgm PROGMEM = {
    9, 244,
    font_lcd_63, font_lcd_45,
    LCD_COLOR_LIGHT_CORAL, LCD_COLOR_BLACK,
    6, 1, ' ',
};
static Param duration = { &durationPgm };

static char *mkNumString(int32_t number, uint8_t width, uint8_t dot, uint8_t lead)
{
    uint8_t numdiv;
    int8_t i;

    for (i = 0; i < width; i++)
        strbuf[i] = lead;
    strbuf[width] = '\0';
    i = 0;

    while (number > 0 || i < dot + 1 + !!dot) {
        numdiv = number % 10;
        strbuf[width - 1 - i] = numdiv + 0x30;
        i++;
        if (dot == i) {
            strbuf[width - 1 - i] = '.';
            i++;
        }
        number /= 10;
    }
    return strbuf;
}

static void updateParam(Param *param, int32_t val, uint8_t skip)
{
    ParamPgm parPgm;
    memcpy_P(&parPgm, param->pgm, sizeof(ParamPgm));

    char *valStr = mkNumString(val, parPgm.len, parPgm.dot, parPgm.lead);

    glcdLoadLcdFont(parPgm.fontMain, parPgm.color, parPgm.bgColor);
    glcdSetXY(parPgm.x, parPgm.y);

    for (uint8_t i = 0; i < parPgm.len; i++) {
        if (parPgm.dot && i == parPgm.len - parPgm.dot - 1) {
            glcdLoadLcdFont(parPgm.fontDeci, parPgm.color, parPgm.bgColor);
            glcdSetY(parPgm.y + pgm_read_byte(&parPgm.fontMain[1]) - pgm_read_byte(&parPgm.fontDeci[1]));
        }
        if (skip && (valStr[i] == param->str[i])) {
            glcdSkipLcdChar(param->str[i]);
        } else {
            param->str[i] = valStr[i];
            glcdWriteLcdChar(param->str[i]);
        }
    }
}

void screenShowMain(void)
{
    uint8_t skip = 1;
    if (SCREEN_MAIN != screen) {
        screen = SCREEN_MAIN;
        glcdFill(LCD_COLOR_BLACK);
        skip = 0;

        glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_LIME, LCD_COLOR_BLACK);
        glcdSetXY(190, 4);
        glcdWriteString("km/h");
        glcdDrawHorizLine(4, 235, 135, LCD_COLOR_AQUA);
        glcdSetXY(213, 144);
        glcdWriteString("km");
        glcdDrawHorizLine(4, 235, 232, LCD_COLOR_AQUA);
        glcdSetXY(213, 244);
        glcdWriteString("km");
    }
    updateParam(&speed, getSpeed(), skip);
    updateParam(&track, getDistance(), skip);
    updateParam(&cadence, getDistance() * 8, skip);
}

void screenShowSetup(void)
{
    uint8_t skip = 1;
    if (SCREEN_SETUP != screen) {
        screen = SCREEN_SETUP;
        glcdFill(LCD_COLOR_BLACK);
        skip = 0;

        glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_LIME, LCD_COLOR_BLACK);
        glcdSetXY(190, 4);
        glcdWriteString("km/h");
        glcdDrawHorizLine(4, 235, 135, LCD_COLOR_AQUA);
        glcdSetXY(213, 144);
        glcdWriteString("km");
        glcdDrawHorizLine(4, 235, 232, LCD_COLOR_AQUA);
        glcdSetXY(213, 244);
        glcdWriteString("km");
    }
    updateParam(&distance, getSpeed(), skip);
    updateParam(&avgSpeed, getDistance(), skip);
    updateParam(&duration, getDistance() * 8, skip);
}

void screenUpdate()
{
    switch (screen) {
    case SCREEN_MAIN:
        screenShowMain();
        break;
    case SCREEN_SETUP:
        screenShowSetup();
        break;
    default:
        break;
    }
}
