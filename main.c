#include <avr/io.h>
#include <util/delay.h>

#include "ili9341.h"
#include "glcd.h"
#include "fonts.h"

void hwInit()
{
    glcdInit();

    return;
}

int main(void)
{
    hwInit();
    glcdFill(LCD_COLOR_BLACK);

    glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_LIME, LCD_COLOR_BLACK);
    glcdSetXY(156, 0);
    glcdWriteString("Speed");

    glcdLoadLcdFont(font_lcd_117, LCD_COLOR_AQUA, LCD_COLOR_BLACK);
    glcdSetXY(0, 0);
    glcdWriteLcdString("57.");
    glcdLoadLcdFont(font_lcd_72, LCD_COLOR_AQUA, LCD_COLOR_BLACK);
    glcdSetXY(182, 45);
    glcdWriteLcdString("2");

    glcdLoadLcdFont(font_lcd_72, LCD_COLOR_RED, LCD_COLOR_BLACK);
    glcdSetXY(0, 150);
    glcdWriteLcdString("254.3");

    glcdLoadLcdFont(font_lcd_45, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
    glcdSetXY(0, 230);
    glcdWriteLcdString("26497816");

    while (1);

    glcdDrawColorMap();

    glcdDrawCircle(120, 160, 59, LCD_COLOR_BLACK);
    glcdDrawCircle(120, 160, 56, LCD_COLOR_WHITE);
    glcdDrawCircle(120, 160, 53, LCD_COLOR_BLACK);

    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_YELLOW);
    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_LIME);
    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_AQUA);
    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_BLUE);
    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_MAGENTA);
    _delay_ms(500);

    while (1);

    return 0;
}
