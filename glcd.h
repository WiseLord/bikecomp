#ifndef GLCD_H
#define GLCD_H

#ifndef GLCD_TYPE
#define GLCD_TYPE               ILI9341
#endif

#if GLCD_TYPE == ILI9341
#include "ili9341.h"
#endif

#if GLCD_TYPE == ILI9341
#define LCD_WIDTH               ILI9341_WIDTH
#define LCD_HEIGHT              ILI9341_HEIGHT
#define LCD_PIXELS              ILI9341_PIXELS
#endif

//Colors
#define LCD_COLOR_WHITE         0xFFFF
#define LCD_COLOR_BLACK         0x0000
#define LCD_COLOR_RED           0xF800
#define LCD_COLOR_GREEN         0x07E0
#define LCD_COLOR_BLUE          0x001F
#define LCD_COLOR_YELLOW        0xFFE0
#define LCD_COLOR_CYAN          0x07FF
#define LCD_COLOR_MAGENTA       0xF81F

void glcdInit(void);
void glcdDrawColorMap(void);



#endif // GLCD_H
