#include "screen.h"

#include "glcd.h"
#include "measure.h"

#define STR_BUFSIZE                     20

static char strbuf[STR_BUFSIZE + 1];           // String buffer

static char *mkNumString(int32_t number, uint8_t width, uint8_t lead, uint8_t dot)
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

static void updateSpeed(uint8_t x, uint8_t y)
{
    static char speedStrPrev[] = "    ";
    char *speedStr = mkNumString(getSpeed(), 4, ' ', 1);

    glcdLoadLcdFont(font_lcd_117, LCD_COLOR_AQUA, LCD_COLOR_BLACK);
    glcdSetXY(x, y);

    for (uint8_t i = 0; i < 4; i++) {
        if (i == 2) {
            glcdLoadLcdFont(font_lcd_72, LCD_COLOR_AQUA, LCD_COLOR_BLACK);
            glcdSetY(y + 117 - 72);
        }
        if (speedStr[i] != speedStrPrev[i]) {
            glcdWriteLcdChar(speedStr[i]);
            speedStrPrev[i] = speedStr[i];
        } else {
            glcdSkipLcdChar(speedStr[i]);
        }
    }
}

static void updateDistance(uint8_t x, uint8_t y)
{
    static char distanceStrPrev[] = "     ";
    char *distanceStr = mkNumString(getDistance(), 5, ' ', 1);

    glcdLoadLcdFont(font_lcd_72, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
    glcdSetXY(x, y);

    for (uint8_t i = 0; i < 5; i++) {
        if (i == 3) {
            glcdLoadLcdFont(font_lcd_45, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
            glcdSetY(y + 72 - 45);
        }
        if (distanceStr[i] != distanceStrPrev[i]) {
            glcdWriteLcdChar(distanceStr[i]);
            distanceStrPrev[i] = distanceStr[i];
        } else {
            glcdSkipLcdChar(distanceStr[i]);
        }
    }
}

void updateMainScreen(void)
{
    updateSpeed(0, 0);
    updateDistance(0, 140);
}

void showScreenMain(void)
{
    glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_LIME, LCD_COLOR_BLACK);
    glcdSetXY(156, 0);
    glcdWriteString("km/h");
    glcdSetXY(156, 140);
    glcdWriteString("km");
}
