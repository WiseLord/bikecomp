#include "screen.h"

#include <avr/pgmspace.h>
#include "glcd.h"
#include "measure.h"

#define STR_BUFSIZE                     20

static Screen screen = SCREEN_END;
static ParamType paramMid = PARAM_TRACK;
static ParamType paramBtm = PARAM_DISTANCE;

static const ParamArea areaMainTop PROGMEM = {
    0, 0, 239, 119,
    2, 0,
    font_lcd_90, font_lcd_63,
};
static char strMainTop[PARAM_STRBUF];

static const ParamArea areaMainMid PROGMEM = {
    0, 120, 239, 219,
    2, 7,
    font_lcd_63, font_lcd_36,
};
static char strMainMid[PARAM_STRBUF];

static const ParamArea areaMainBtm PROGMEM = {
    0, 220, 239, 319,
    2, 7,
    font_lcd_63, font_lcd_36,
};
static char strMainBtm[PARAM_STRBUF];

static const LcdText textMainTop_5_1 PROGMEM = {
    10, 29, 5, 1, ' ',
};
static const LcdText textParam_7_2 PROGMEM = {
    19, 36, 7, 2, ' ',
};
static const LcdText textParam_7_1 PROGMEM = {
    1, 36, 7, 1, ' ',
};
static const LcdText textTimeHour PROGMEM = {
    5, 36, 2, 0, ' ',
};
static const LcdText textTimeMinSec PROGMEM = {
    103, 36, 5, 2, '0',
};

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

static char strbuf[STR_BUFSIZE + 1];

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

static void updateParam(const Param *paramPgm, const LcdText *lcdTextPgm, int32_t val,
                        Section section, ClearMode clear)
{
    // Read progmem values to RAM structures
    Param param;
    memcpy_P(&param, paramPgm, sizeof(Param));

    LcdText text;
    memcpy_P(&text, lcdTextPgm, sizeof(LcdText));

    ParamArea area;
    char *posStr;
    switch (section) {
    case SECTION_MAIN_MID:
        memcpy_P(&area, &areaMainMid, sizeof(ParamArea));
        posStr = strMainMid;
        break;
    case SECTION_MAIN_BTM:
        memcpy_P(&area, &areaMainBtm, sizeof(ParamArea));
        posStr = strMainBtm;
        break;
    default:
        memcpy_P(&area, &areaMainTop, sizeof(ParamArea));
        posStr = strMainTop;
        break;
    }

    // Clear section area if required and draw constant text labels
    if (clear == CLEAR_ALL) {
        glcdDrawRectangle(area.left, area.top, area.right, area.bottom, LCD_COLOR_BLACK);
        if (area.top)
            glcdDrawHorizLine(area.left + 2, area.right - 2, area.top + 3, LCD_COLOR_GRAY);
        glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_GRAY, LCD_COLOR_BLACK);
        glcdSetXY(area.labX, area.top + area.labY);
        glcdWriteString("> ");
        strcpy_P(strbuf, param.label);
        glcdWriteString(strbuf);
    }

    // Redraw param value with selected LCD font
    char *valStr = mkNumString(val, text.len, text.dot, text.lead);

    glcdLoadLcdFont(area.fontMain, param.color, LCD_COLOR_BLACK);
    glcdSetXY(text.x, area.top + text.y);

    for (uint8_t i = 0; i < text.len; i++) {
        if (text.dot && i == text.len - text.dot - 1) {
            glcdLoadLcdFont(area.fontDeci, param.color, LCD_COLOR_BLACK);
            glcdSetY(area.top + text.y + pgm_read_byte(&area.fontMain[1]) - pgm_read_byte(
                         &area.fontDeci[1]));
        }
        if (clear || (valStr[i] != posStr[i])) {
            posStr[i] = valStr[i];
            glcdWriteLcdChar(posStr[i]);
        } else {
            glcdSkipLcdChar(posStr[i]);
        }
    }
}

static void updateTime(int32_t time, Section pos, ClearMode clear)
{
    updateParam(&currentTrackTimeParam, &textTimeHour, time / 3600, pos, clear);
    glcdWriteLcdChar(':');
    time %= 3600;
    time = time / 60 * 100 + time % 60;
    updateParam(&currentTrackTimeParam, &textTimeMinSec, time, pos, CLEAR_LCDDATA);
}

static ParamType getParameter(Section section)
{
    switch (section) {
    case SECTION_MAIN_MID:
        return paramMid;
        break;
    case SECTION_MAIN_BTM:
        return paramBtm;
        break;
    default:
        break;
    }

    return PARAM_SPEED;
}

static void updateSection(Section section, ClearMode clear)
{
    int32_t value;

    switch (getParameter(section)) {
    case PARAM_SPEED:
        value = getCurrentSpeed() * 36 / 10 / 100;  // mm/s => 0.1km/h
        updateParam(&currentSpeedParam, &textMainTop_5_1, value, section, clear);
        break;
    case PARAM_TRACK:
        value = getCurrentTrack() / 10;             // m => 0.01km
        updateParam(&currentTrackParam, &textParam_7_2, value, section, clear);
        break;
    case PARAM_DISTANCE:
        value = getTotalDistance() / 100;           // m => 0.1km
        updateParam(&totalDistanceParam, &textParam_7_1, value, section, clear);
        break;
    case PARAM_TRACKTIME:
        value = getTrackTime();
        updateTime(value, section, clear);
        break;
    default:
        break;
    }
}

void switchParam(Section section)
{
    ParamType *ppar;

    switch (section) {
    case SECTION_MAIN_MID:
        ppar = &paramMid;
        break;
    case SECTION_MAIN_BTM:
        ppar = &paramBtm;
        break;
    default:
        return;
    }

    if (++(*ppar) >= PARAM_END)
        *ppar = PARAM_TRACK;
    updateSection(section, CLEAR_ALL);
}

void screenShowMain(void)
{
    ClearMode clear = (SCREEN_MAIN != screen);

    updateSection(SECTION_MAIN_TOP, clear);
    updateSection(SECTION_MAIN_MID, clear);
    updateSection(SECTION_MAIN_BTM, clear);
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
