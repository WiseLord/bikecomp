#include "screen.h"

#include "glcd.h"

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


static void updateSpeed()
{
    static char *speedStrPrev = "    ";
    char *speedStr = mkNumString(237, 4, ' ', 1);

    glcdLoadLcdFont(font_lcd_117, LCD_COLOR_AQUA, LCD_COLOR_BLACK);
    glcdSetXY(0, 0);

    for (uint8_t i = 0; i < 4; i++) {
        if (i == 3) {
            glcdLoadLcdFont(font_lcd_72, LCD_COLOR_AQUA, LCD_COLOR_BLACK);
            glcdSetXY(182, 45);
        }
        if (speedStr[i] != speedStrPrev[i]) {
            glcdWriteLcdChar(speedStr[i]);
            speedStrPrev[i] = speedStr[i];
        } else {
            glcdSkipLcdChar(speedStr[i]);
        }
    }
}


void showScreenMain()
{
    updateSpeed();
}
