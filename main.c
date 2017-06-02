#include <avr/io.h>
#include <util/delay.h>

#include "ili9341.h"
#include "glcd.h"
#include "fonts.h"
#include "screen.h"
#include "measure.h"

void hwInit()
{
    glcdInit();

    return;
}

int main(void)
{
    hwInit();

    while (1) {
        showScreenMain();
        testUpdate();
    }

    glcdDrawColorMap();
    while(1) {
        glcdFill(LCD_COLOR_BLACK);
        glcdDrawColorMap();
    }

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
