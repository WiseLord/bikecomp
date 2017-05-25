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

    while(1) {
        uint8_t i;
        glcdLoadFont(font_digits_32, LCD_COLOR_WHITE, LCD_COLOR_BLACK, FONT_DIR_0);
        glcdSetXY(0, 0);
        for (i = 0; i < 10; i++)
            glcdWriteChar('0' + i);
        for (i = 0; i < 10; i++)
            glcdWriteChar('0' + i);
        glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_GREEN, LCD_COLOR_BLACK, FONT_DIR_0);
        glcdSetXY(0, 40);
        glcdWriteString("Quite a long test string");
        _delay_ms(500);

        glcdLoadFont(font_digits_32, LCD_COLOR_BLACK, LCD_COLOR_BLACK, FONT_DIR_0);
        glcdSetXY(0, 0);
        for (i = 0; i < 10; i++)
            glcdWriteChar('0' + i);
        for (i = 0; i < 10; i++)
            glcdWriteChar('0' + i);
        glcdLoadFont(font_ks0066_ru_24, LCD_COLOR_BLACK, LCD_COLOR_BLACK, FONT_DIR_0);
        glcdSetXY(0, 40);
        glcdWriteString("Quite a long test string");
        _delay_ms(500);
    }

    glcdDrawColorMap();

    glcdDrawCircle(160, 120, 59, LCD_COLOR_BLACK);
    glcdDrawCircle(160, 120, 56, LCD_COLOR_WHITE);
    glcdDrawCircle(160, 120, 53, LCD_COLOR_BLACK);

    while (1) {
        _delay_ms(500);
        glcdDrawCircle(160, 120, 50, LCD_COLOR_YELLOW);
        _delay_ms(500);
        glcdDrawCircle(160, 120, 50, LCD_COLOR_GREEN);
        _delay_ms(500);
        glcdDrawCircle(160, 120, 50, LCD_COLOR_CYAN);
        _delay_ms(500);
        glcdDrawCircle(160, 120, 50, LCD_COLOR_BLUE);
        _delay_ms(500);
        glcdDrawCircle(160, 120, 50, LCD_COLOR_MAGENTA);
        _delay_ms(500);
    }

    return 0;
}
