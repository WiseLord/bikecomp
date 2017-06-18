#include "screen.h"

#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "eeprom.h"
#include "glcd.h"
#include "measure.h"

#define STR_BUFSIZE                     20

static Screen screen = SCREEN_END;

static Param paramTop = PARAM_SPEED;
static Param paramMid = PARAM_TRACK;
static Param paramBtm = PARAM_SPEED_AVG;

static char strTop[PARAM_STRBUF];
static char strMid[PARAM_STRBUF];
static char strBtm[PARAM_STRBUF];

static const ParamArea areaMainTop PROGMEM = {
    0, 0, 239, 119,
    2, 0,
    font_lcd_90, font_lcd_63,
};
static const ParamArea areaMainMid PROGMEM = {
    0, 120, 239, 219,
    2, 7,
    font_lcd_63, font_lcd_36,
};
static const ParamArea areaMainBtm PROGMEM = {
    0, 220, 239, 319,
    2, 7,
    font_lcd_63, font_lcd_36,
};

static const ParamArea areaSetupTop PROGMEM = {
    0, 0, 239, 101,
    2, 0,
    font_lcd_72, font_lcd_45,
};
static const ParamArea areaSetupMid PROGMEM = {
    0, 102, 239, 210,
    2, 7,
    font_lcd_72, font_lcd_45,
};
static const ParamArea areaSetupBtm PROGMEM = {
    0, 211, 239, 319,
    2, 7,
    font_lcd_72, font_lcd_45,
};

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

static const LcdText textParamTop_5 PROGMEM = {
    7, 29, 5, 0, ' ',
};
static const LcdText textParam_5 PROGMEM = {
    7, 36, 5, 0, ' ',
};

const char speedLabel[] PROGMEM = "Current speed";
static const ParamData speedParam PROGMEM = {
    LCD_COLOR_AQUA, speedLabel,
};

const char trackLabel[] PROGMEM = "Track length";
static const ParamData trackParam PROGMEM = {
    LCD_COLOR_YELLOW, trackLabel,
};

const char trackTimeLabel[] PROGMEM = "Track time";
static const ParamData trackTimeParam PROGMEM = {
    LCD_COLOR_GREEN, trackTimeLabel,
};

const char speedAvgLabel[] PROGMEM = "Average speed";
static const ParamData speedAvgParam PROGMEM = {
    LCD_COLOR_OLIVE, speedAvgLabel,
};

const char distanceLabel[] PROGMEM = "Total distance";
static const ParamData distanceParam PROGMEM = {
    LCD_COLOR_LIGHT_CORAL, distanceLabel,
};

const char wheelLabel[] PROGMEM = "Wheel length";
static const ParamData wheelParam PROGMEM = {
    LCD_COLOR_WHITE, wheelLabel,
};

const char emptyLabel[] PROGMEM = "";
static const ParamData emptyParam PROGMEM = {
    LCD_COLOR_BLACK, emptyLabel,
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

static void updateParam(const ParamData *paramPgm, const LcdText *lcdTextPgm, int32_t val,
                        Section section, ClearMode clear)
{
    // Read progmem values to RAM structures
    ParamData param;
    memcpy_P(&param, paramPgm, sizeof(ParamData));

    LcdText text;
    memcpy_P(&text, lcdTextPgm, sizeof(LcdText));

    ParamArea area;
    char *posStr;
    switch (section) {
    case SECTION_MAIN_TOP:
        memcpy_P(&area, &areaMainTop, sizeof(ParamArea));
        posStr = strTop;
        break;
    case SECTION_MAIN_MID:
        memcpy_P(&area, &areaMainMid, sizeof(ParamArea));
        posStr = strMid;
        break;
    case SECTION_MAIN_BTM:
        memcpy_P(&area, &areaMainBtm, sizeof(ParamArea));
        posStr = strBtm;
        break;
    case SECTION_SETUP_TOP:
        memcpy_P(&area, &areaSetupTop, sizeof(ParamArea));
        posStr = strTop;
        break;
    case SECTION_SETUP_MID:
        memcpy_P(&area, &areaSetupMid, sizeof(ParamArea));
        posStr = strMid;
        break;
    case SECTION_SETUP_BTM:
        memcpy_P(&area, &areaSetupBtm, sizeof(ParamArea));
        posStr = strBtm;
        break;
    default:
        return;
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
    updateParam(&trackTimeParam, &textTimeHour, time / 3600, pos, clear);
    glcdWriteLcdChar(':');
    time %= 3600;
    time = time / 60 * 100 + time % 60;
    updateParam(&trackTimeParam, &textTimeMinSec, time, pos, CLEAR_LCDDATA);
}

static Param getParamType(Section section)
{
    switch (section) {
    case SECTION_MAIN_TOP:
        return paramTop;
    case SECTION_MAIN_MID:
        return paramMid;
    case SECTION_MAIN_BTM:
        return paramBtm;
    case SECTION_SETUP_TOP:
        return PARAM_SETUP_WHEEL;
    case SECTION_SETUP_MID:
        return PARAM_SETUP_END;
    case SECTION_SETUP_BTM:
        return PARAM_SETUP_END;
    default:
        return PARAM_END;
    }
}

static void updateSection(Section section, ClearMode clear)
{
    Param param = getParamType(section);
    int32_t value = measureGetValue(param);

    switch (param) {
    case PARAM_SPEED:
        value = value * 36 / 10 / 100;      // mm/s => 0.1km/h
        updateParam(&speedParam, &textMainTop_5_1, value, section, clear);
        break;
    case PARAM_TRACK:
        value = value / 1000 / 10;          // mm => 0.01km
        updateParam(&trackParam, &textParam_7_2, value, section, clear);
        break;
    case PARAM_TRACKTIME:                   // sec
        updateTime(value, section, clear);
        break;
    case PARAM_SPEED_AVG:
        value = value * 36 / 10 / 100;      // mm/s => 0.1km/h
        updateParam(&speedAvgParam, &textParam_7_1, value, section, clear);
        break;
    case PARAM_DISTANCE:
        value = value / 100;                // m => 0.1km
        updateParam(&distanceParam, &textParam_7_1, value, section, clear);
        break;
    case PARAM_SETUP_WHEEL:
        updateParam(&wheelParam, &textParamTop_5, value, section, clear);
        break;
    case PARAM_SETUP_END:
        updateParam(&emptyParam, &textParam_5, value, section, clear);
        break;
    default:
        break;
    }
}

void screenInit(void)
{
    paramMid = eeprom_read_byte((uint8_t*)EEPROM_PARAM_MID);
    paramBtm = eeprom_read_byte((uint8_t*)EEPROM_PARAM_BTM);
}

void switchParam(Section section)
{
    switch (section) {
    case SECTION_MAIN_MID:
        if (++paramMid >= PARAM_END)
            paramMid = PARAM_TRACK;
        if (paramMid == paramBtm)
            if (++paramMid >= PARAM_END)
                paramMid = PARAM_TRACK;
        eeprom_update_byte((uint8_t*)EEPROM_PARAM_MID, paramMid);
        break;
    case SECTION_MAIN_BTM:
        if (++paramBtm >= PARAM_END)
            paramBtm = PARAM_TRACK;
        if (paramBtm == paramMid)
            if (++paramBtm >= PARAM_END)
                paramBtm = PARAM_TRACK;
        eeprom_update_byte((uint8_t*)EEPROM_PARAM_BTM, paramBtm);
        break;
    default:
        return;
    }

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
    ClearMode clear = (SCREEN_SETUP != screen);

    updateSection(SECTION_SETUP_TOP, clear);
    updateSection(SECTION_SETUP_MID, clear);
    updateSection(SECTION_SETUP_BTM, clear);

    screen = SCREEN_SETUP;
}

void screenUpdate(void)
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

Screen screenGet(void)
{
    return screen;
}
