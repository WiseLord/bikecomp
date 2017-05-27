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

    glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_GREEN, LCD_COLOR_BLACK);
    glcdSetXY(0, 0);
    glcdWriteString("Speed");
    glcdSetXY(0, 296);
    glcdWriteString("Bottom test string");

    glcdLoadLcdFont(font_lcd_72, LCD_COLOR_CYAN, LCD_COLOR_BLACK);
    glcdSetXY(8, 30);
    glcdWriteLcdDig('5');
    glcdSetXY(56, 30);
    glcdWriteLcdDig('7');
    glcdSetXY(104, 30);
    glcdWriteLcdDig('8');
    glcdSetXY(152, 30);
    glcdWriteLcdDig('3');
    glcdSetXY(200, 30);
    glcdWriteLcdDig('2');

    glcdLoadLcdFont(font_lcd_45, LCD_COLOR_YELLOW, LCD_COLOR_BLACK);
    glcdSetXY(5, 110);
    glcdWriteLcdDig('2');
    glcdSetXY(35, 110);
    glcdWriteLcdDig('6');
    glcdSetXY(65, 110);
    glcdWriteLcdDig('4');
    glcdSetXY(95, 110);
    glcdWriteLcdDig('9');
    glcdSetXY(125, 110);
    glcdWriteLcdDig('7');
    glcdSetXY(155, 110);
    glcdWriteLcdDig('8');
    glcdSetXY(185, 110);
    glcdWriteLcdDig('1');
    glcdSetXY(215, 110);
    glcdWriteLcdDig('6');

    glcdLoadLcdFont(font_lcd_27, LCD_COLOR_WHITE, LCD_COLOR_BLACK);
    for (uint8_t i = '0'; i <= '9'; i++) {
        glcdSetXY(225 - (i - '0') * (flp.width + flp.thickness), 160);
        glcdWriteLcdDig(i);
    }

    glcdLoadLcdFont(font_lcd_72, LCD_COLOR_RED, LCD_COLOR_BLACK);
    glcdSetXY(0, 200);
    glcdWriteLcdDig('5');
    glcdSetXY(48, 200);
    glcdWriteLcdDig('4');
    glcdSetXY(96, 200);
    glcdWriteString("km/h");

    glcdSetXY(96, 200);
    glcdWriteLcdDig('.');
    glcdLoadLcdFont(font_lcd_45, LCD_COLOR_RED, LCD_COLOR_BLACK);
    glcdSetXY(114, 227);
    glcdWriteLcdDig('2');


    while(1);

    glcdDrawColorMap();

    glcdDrawCircle(120, 160, 59, LCD_COLOR_BLACK);
    glcdDrawCircle(120, 160, 56, LCD_COLOR_WHITE);
    glcdDrawCircle(120, 160, 53, LCD_COLOR_BLACK);

    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_YELLOW);
    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_GREEN);
    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_CYAN);
    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_BLUE);
    _delay_ms(500);
    glcdDrawCircle(120, 160, 50, LCD_COLOR_MAGENTA);
    _delay_ms(500);

    while (1);

    return 0;
}
