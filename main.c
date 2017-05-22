#include <avr/io.h>
#include <util/delay.h>

#include "ili9341.h"
#include "glcd.h"

void hwInit()
{
    glcdInit();

    return;
}

int main(void)
{
    hwInit();

    glcdFill(LCD_COLOR_BLACK);
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
