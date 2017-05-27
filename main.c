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
    glcdSetXY(50, 0);
    glcdWriteLcdDig('1');
    glcdSetXY(100, 0);
    glcdWriteLcdDig('8');
    glcdSetXY(150, 0);
    glcdWriteLcdDig('3');
    glcdSetXY(200, 0);
    glcdWriteLcdDig('2');

    glcdSetXY(0, 0);
    while (1) {
        uint8_t i;
        for (i = '0'; i <= '9'; i++) {
            glcdWriteLcdDig(i);
            _delay_ms(200);
        }
    }

    glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_GREEN, LCD_COLOR_BLACK);
    glcdSetXY(0, 0);
    glcdWriteString("Speed");
    glcdSetXY(0, 296);
    glcdWriteString("Bottom test string");

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
