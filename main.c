#include <avr/io.h>
#include <util/delay.h>

#include "ili9341.h"

void hwInit()
{
    ILI9341_Init();

    return;
}

void ColorMap(void)
{
    uint16_t r, g, b, color;
    uint16_t i, j;

    ILI9341_SetCursorPos(0, 0, 319, 239);
    ILI9341_SendCmd(ILI9341_RAMWR);
    for (j = 0; j < 240; j++) {
        r = 0;
        b = 0;
        g = j * 63 / 239;
        for (i = 0; i < 80; i++) {
            r = i * 31 / 79;
            color = (r << 11) | (g << 5) | (b << 0);
            ILI9341_SendData(color >> 8);
            ILI9341_SendData(color & 0xFF);
        }
        for (i = 0; i < 80; i++) {
            b = i * 31 / 79;
            color = (r << 11) | (g << 5) | (b << 0);
            ILI9341_SendData(color >> 8);
            ILI9341_SendData(color & 0xFF);
        }
        for (i = 0; i < 80; i++) {
            r = (79 - i) * 31 / 79;
            color = (r << 11) | (g << 5) | (b << 0);
            ILI9341_SendData(color >> 8);
            ILI9341_SendData(color & 0xFF);
        }
        for (i = 0; i < 80; i++) {
            b = (79 - i) * 31 / 79;
            color = (r << 11) | (g << 5) | (b << 0);
            ILI9341_SendData(color >> 8);
            ILI9341_SendData(color & 0xFF);
        }
    }

    return;
}

int main(void)
{
    hwInit();

    ILI9341_Rotate(LCD_Orientation_Landscape_1);

    ILI9341_Fill(LCD_COLOR_BLACK);

    ColorMap();

    ILI9341_DrawFilledCircle(160, 120, 59, LCD_COLOR_BLACK);
    ILI9341_DrawFilledCircle(160, 120, 56, LCD_COLOR_WHITE);
    ILI9341_DrawFilledCircle(160, 120, 53, LCD_COLOR_BLACK);

    while (1) {
        _delay_ms(500);
        ILI9341_DrawFilledCircle(160, 120, 50, LCD_COLOR_YELLOW);
        _delay_ms(500);
        ILI9341_DrawFilledCircle(160, 120, 50, LCD_COLOR_GREEN);
        _delay_ms(500);
        ILI9341_DrawFilledCircle(160, 120, 50, LCD_COLOR_CYAN);
        _delay_ms(500);
        ILI9341_DrawFilledCircle(160, 120, 50, LCD_COLOR_BLUE);
        _delay_ms(500);
        ILI9341_DrawFilledCircle(160, 120, 50, LCD_COLOR_MAGENTA);
        _delay_ms(500);
    }

    return 0;
}
