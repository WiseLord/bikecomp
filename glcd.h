#ifndef GLCD_H
#define GLCD_H

#ifndef GLCD_TYPE
#define GLCD_TYPE               ILI9341
#endif

#if GLCD_TYPE == ILI9341
#include "ili9341.h"
#endif

#include "fonts.h"

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

typedef enum {
    LCD_Orientation_Portrait_1,
    LCD_Orientation_Portrait_2,
    LCD_Orientation_Landscape_1,
    LCD_Orientation_Landscape_2
} GlcdOrientation;

typedef struct {
    uint16_t width;
    uint16_t height;
    GlcdOrientation orientation;
} GlcdOpts;

extern GlcdOpts glcdOpts;
extern FontParam fp;
extern FontLcdParam flp;

// Some function remap
#if GLCD_TYPE == ILI9341
#define glcdInit                ili9341Init
#define glcdDrawPixel           ili9431DrawPixel
#define glcdDrawRectangle       ili9341DrawRectangle
#define glcdDrawColorMap        ili9341DrawColorMap
#endif
#define glcdFill(c)                     glcdDrawRectangle(0, 0, glcdOpts.width, glcdOpts.height, c)
#define glcdDrawHorizLine(x0, x1, y, c) glcdDrawRectangle(x0, y, x1, y, c);
#define glcdDrawVertLine(x, y0, y1, c)  glcdDrawRectangle(x, y0, x, y1, c);

void glcdDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void glcdDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void glcdDrawRing(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void glcdDrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void glcdSetXY(uint16_t x, uint16_t y);

void glcdLoadFont(const uint8_t *font, uint16_t color, uint16_t bgColor);
void glcdWriteChar(uint8_t code);
void glcdWriteString(char *string);

void glcdLoadLcdFont(const uint8_t *font, uint16_t color, uint16_t bgColor);
void glcdWriteLcdDig(uint8_t code);

#endif // GLCD_H
