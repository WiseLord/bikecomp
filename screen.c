#include "screen.h"

#include <avr/pgmspace.h>
#include "glcd.h"
#include "measure.h"

#define STR_BUFSIZE                     20

static Screen screen = SCREEN_END;

static char strbuf[STR_BUFSIZE + 1];           // String buffer

static const SectionPgm mainTopPgm PROGMEM = {
    0, 0, 239, 107,
    0, 0,
    font_lcd_99, font_lcd_72,
    52, 0,
    4, 1, ' ',
};
static Section mainTop = { &mainTopPgm };

static const SectionPgm mainMidPgm PROGMEM = {
    0, 109, 239, 226,
    2, 111,
    font_lcd_72, font_lcd_45,
    13, 143,
    6, 1, ' ',
};
static Section mainMid = { &mainMidPgm };

static const SectionPgm mainBtmPgm PROGMEM = {
    0, 225, 239, 319,
    2, 227,
    font_lcd_63, font_lcd_36,
    1, 257,
    7, 1, ' ',
};
static Section mainBtm = { &mainBtmPgm };

const char currentSpeedLabel[] PROGMEM = "Current speed";
static const Param currentSpeedParam PROGMEM = {
    LCD_COLOR_AQUA, currentSpeedLabel,
};

const char currentTrackLabel[] PROGMEM = "Track length";
static const Param currentTrackParam PROGMEM = {
    LCD_COLOR_YELLOW, currentTrackLabel,
};

const char totalDistanceLabel[] PROGMEM = "Total distance";
static const Param totalDistanceParam PROGMEM = {
    LCD_COLOR_LIGHT_CORAL, totalDistanceLabel,
};

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

static void updateParam(const Param *param, Section *section, int32_t val, uint8_t clear)
{
    // Read progmem values to RAM structures
    Param parPgm;
    memcpy_P(&parPgm, param, sizeof(Param));

    SectionPgm secPgm;
    memcpy_P(&secPgm, section->pgm, sizeof(SectionPgm));

    // Clear section area if required and draw constant text labels
    if (clear) {
        glcdDrawRectangle(secPgm.left, secPgm.top, secPgm.right, secPgm.bottom, LCD_COLOR_BLACK);
        if (secPgm.top) {
            glcdDrawHorizLine(secPgm.left + 2, secPgm.right - 2, secPgm.top - 1, LCD_COLOR_GRAY);
            glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_GRAY, LCD_COLOR_BLACK);
            glcdSetXY(secPgm.labX, secPgm.labY);
            glcdWriteString("> ");
            strcpy_P(strbuf, parPgm.label);
            glcdWriteString(strbuf);
        }
    }

    // Redraw param value with selected LCD font
    char *valStr = mkNumString(val, secPgm.len, secPgm.dot, secPgm.lead);

    glcdLoadLcdFont(secPgm.fontMain, parPgm.color, LCD_COLOR_BLACK);
    glcdSetXY(secPgm.x, secPgm.y);

    for (uint8_t i = 0; i < secPgm.len; i++) {
        if (secPgm.dot && i == secPgm.len - secPgm.dot - 1) {
            glcdLoadLcdFont(secPgm.fontDeci, parPgm.color, LCD_COLOR_BLACK);
            glcdSetY(secPgm.y + pgm_read_byte(&secPgm.fontMain[1]) - pgm_read_byte(&secPgm.fontDeci[1]));
        }
        if (clear || (valStr[i] != section->str[i])) {
            section->str[i] = valStr[i];
            glcdWriteLcdChar(section->str[i]);
        } else {
            glcdSkipLcdChar(section->str[i]);
        }
    }
}

void updateSpeed(int32_t value)
{
    updateParam(&currentSpeedParam, &mainTop, value, SCREEN_MAIN != screen);
}

void updateTrack(int32_t value)
{
    updateParam(&currentTrackParam, &mainMid, value, SCREEN_MAIN != screen);
}

void updateDistance(int32_t value)
{
    updateParam(&totalDistanceParam, &mainBtm, value, SCREEN_MAIN != screen);
}

void screenShowMain(void)
{
    updateSpeed(getCurrentSpeed());
    updateTrack(getCurrentTrack());
    updateDistance(getTotalDistance());

    screen = SCREEN_MAIN;
}

void screenShowSetup(void)
{

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
