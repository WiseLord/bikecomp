#include "screen.h"

#include <avr/pgmspace.h>
#include "glcd.h"
#include "measure.h"

#define STR_BUFSIZE                     20

static Screen screen = SCREEN_END;
static ParamMid paramMid = PARAM_MID_TRACK;
static ParamBtm paramBtm = PARAM_BTM_DISTANCE;

static char strbuf[STR_BUFSIZE + 1];           // String buffer

static const SectionPgm mainTopPgm PROGMEM = {
    0, 0, 239, 124,
    2, 0,
    font_lcd_90, font_lcd_63,
    10, 29,
    5, 1, ' ',
};
static Section mainTop = { &mainTopPgm };

static const SectionPgm mainMidPgm PROGMEM = {
    0, 125, 239, 224,
    2, 127,
    font_lcd_63, font_lcd_36,
    19, 156,
    7, 2, ' ',
};
static Section mainMid = { &mainMidPgm };

static const SectionPgm mainBtmPgm PROGMEM = {
    0, 225, 239, 319,
    2, 227,
    font_lcd_63, font_lcd_36,
    1, 256,
    7, 1, ' ',
};
static Section mainBtm = { &mainBtmPgm };

static const SectionPgm mainBtmTimeHrPgm PROGMEM = {
    0, 225, 239, 319,
    2, 227,
    font_lcd_63, font_lcd_36,
    5, 256,
    2, 0, ' ',
};
static Section mainTimeHrBtm = { &mainBtmTimeHrPgm };

static const SectionPgm mainBtmTimeMsPgm PROGMEM = {
    0, 225, 239, 319,
    2, 227,
    font_lcd_63, font_lcd_36,
    103, 256,
    5, 2, '0',
};
static Section mainTimeMsBtm = { &mainBtmTimeMsPgm };

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

const char currentTrackTimeLabel[] PROGMEM = "Track time";
static const Param currentTrackTimeParam PROGMEM = {
    LCD_COLOR_GREEN, currentTrackTimeLabel,
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

static void updateParam(const Param *param, Section *section, int32_t val, ClearMode clear)
{
    // Read progmem values to RAM structures
    Param parPgm;
    memcpy_P(&parPgm, param, sizeof(Param));

    SectionPgm secPgm;
    memcpy_P(&secPgm, section->pgm, sizeof(SectionPgm));

    // Clear section area if required and draw constant text labels
    if (clear == CLEAR_ALL) {
        glcdDrawRectangle(secPgm.left, secPgm.top, secPgm.right, secPgm.bottom, LCD_COLOR_BLACK);
        if (secPgm.top)
            glcdDrawHorizLine(secPgm.left + 2, secPgm.right - 2, secPgm.top, LCD_COLOR_GRAY);
        glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_GRAY, LCD_COLOR_BLACK);
        glcdSetXY(secPgm.labX, secPgm.labY);
        glcdWriteString("> ");
        strcpy_P(strbuf, parPgm.label);
        glcdWriteString(strbuf);
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

static void updateTime(int32_t time, ClearMode clear)
{
    updateParam(&currentTrackTimeParam, &mainTimeHrBtm, time / 3600, clear);
    glcdWriteLcdChar(':');
    time %= 3600;
    time = time / 60 * 100 + time % 60;
    updateParam(&currentTrackTimeParam, &mainTimeMsBtm, time, CLEAR_LCDDATA);
}

static void updateSpeed(void)
{
    int32_t value;

    value = getCurrentSpeed() / 100 * 36 / 10;  // mm/s => 0.1km/h
    updateParam(&currentSpeedParam, &mainTop, value, SCREEN_MAIN != screen);
}

static void updateMid(ClearMode clear)
{
    int32_t value;

    switch (paramMid) {
    case PARAM_MID_TRACK:
        value = getCurrentTrack() / 10;             // m => 0.01km
        updateParam(&currentTrackParam, &mainMid, value, clear);
        break;
    default:
        break;
    }
}

static void updateBtm(ClearMode clear)
{
    int32_t value;

    switch (paramBtm) {
    case PARAM_BTM_DISTANCE:
        value = getTotalDistance() / 100;           // m => 0.1km
        updateParam(&totalDistanceParam, &mainBtm, value, clear);
        break;
    case PARAM_BTM_TRACKTIME:
        value = getTrackTime();
        updateTime(value, clear);
        break;
    default:
        break;
    }
}

Screen getScreen(void)
{
    return screen;
}


void switchParamMid(void)
{
    if (++paramMid >= PARAM_MID_END)
        paramMid = PARAM_MID_TRACK;
    updateMid(CLEAR_ALL);
}

void switchParamBtm(void)
{
    if (++paramBtm >= PARAM_BTM_END)
        paramBtm = PARAM_BTM_DISTANCE;
    updateBtm(CLEAR_ALL);
}

void screenShowMain(void)
{
    updateSpeed();
    updateMid(SCREEN_MAIN != screen);
    updateBtm(SCREEN_MAIN != screen);
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
