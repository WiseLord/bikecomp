#ifndef GLCD_H
#define GLCD_H

#include <inttypes.h>

#ifndef GLCD_TYPE
#define GLCD_TYPE                       9341 // ILI9341
#endif

#if GLCD_TYPE == 9341
#include "ili9341.h"
#else
#error "Unknown GLCD_TYPE"
#endif

#include "fonts.h"

#if GLCD_TYPE == 9341
#define LCD_WIDTH                       ILI9341_WIDTH
#define LCD_HEIGHT                      ILI9341_HEIGHT
#define LCD_PIXELS                      ILI9341_PIXELS
#endif

//Colors
#define RGB_TO_565(x)                   (((x >> 8) & 0xF800) | ((x >> 5) & 0x7E0) | ((x >> 3) & 0x1F))
#define LCD_COLOR_BLACK                 RGB_TO_565(0x000000)
#define LCD_COLOR_NAVI                  RGB_TO_565(0x000080)
#define LCD_COLOR_BLUE                  RGB_TO_565(0x0000FF)
#define LCD_COLOR_GREEN                 RGB_TO_565(0x008000)
#define LCD_COLOR_TEAL                  RGB_TO_565(0x008080)
#define LCD_COLOR_DODGER_BLUE           RGB_TO_565(0x0080FF)
#define LCD_COLOR_LIME                  RGB_TO_565(0x00FF00)
#define LCD_COLOR_SPRING_GREEN          RGB_TO_565(0x00FF80)
#define LCD_COLOR_AQUA                  RGB_TO_565(0x00FFFF)
#define LCD_COLOR_NERO                  RGB_TO_565(0x252525)
#define LCD_COLOR_ECLIPSE               RGB_TO_565(0x3F3939)
#define LCD_COLOR_MAROON                RGB_TO_565(0x800000)
#define LCD_COLOR_PURPLE                RGB_TO_565(0x800080)
#define LCD_COLOR_ELECTRIC_INDIGO       RGB_TO_565(0x8000FF)
#define LCD_COLOR_OLIVE                 RGB_TO_565(0x808000)
#define LCD_COLOR_GRAY                  RGB_TO_565(0x808080)
#define LCD_COLOR_LIGHT_SLATE_BLUE      RGB_TO_565(0x8080FF)
#define LCD_COLOR_CHARTREUSE            RGB_TO_565(0x80FF00)
#define LCD_COLOR_LIGHT_GREEN           RGB_TO_565(0x80FF80)
#define LCD_COLOR_ELECTRIC_BLUE         RGB_TO_565(0x80FFFF)
#define LCD_COLOR_RED                   RGB_TO_565(0xFF0000)
#define LCD_COLOR_DEEP_PINK             RGB_TO_565(0xFF0080)
#define LCD_COLOR_MAGENTA               RGB_TO_565(0xFF00FF)
#define LCD_COLOR_DARK_ORANGE           RGB_TO_565(0xFF8000)
#define LCD_COLOR_LIGHT_CORAL           RGB_TO_565(0xFF8080)
#define LCD_COLOR_FUCHSIA_PINK          RGB_TO_565(0xFF80FF)
#define LCD_COLOR_YELLOW                RGB_TO_565(0xFFFF00)
#define LCD_COLOR_WITCH_HAZE            RGB_TO_565(0xFFFF80)
#define LCD_COLOR_WHITE                 RGB_TO_565(0xFFFFFF)

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
#if GLCD_TYPE == 9341
#define glcdInit()                              ili9341Init()
#define glcdDrawPixel(x, y, clr)                ili9431DrawPixel(x, y, clr)
#define glcdDrawRectangle(x0, y0, x1, y1, clr)  ili9341DrawRectangle(x0, y0, x1, y1, clr)
#define glcdDrawColorMap()                      ili9341DrawColorMap()
#else
#define glcdInit()
#define glcdDrawPixel(x, y, color)
#define glcdDrawRectangle(x0, y0, x1, y1, clr)
#define glcdDrawColorMap()
#endif
#define glcdFill(c)                         glcdDrawRectangle(0, 0, glcdOpts.width, glcdOpts.height, c)
#define glcdDrawHorizLine(x0, x1, y, c)     glcdDrawRectangle(x0, y, x1, y, c);
#define glcdDrawVertLine(x, y0, y1, c)      glcdDrawRectangle(x, y0, x, y1, c);

void glcdDrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void glcdDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void glcdDrawRing(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void glcdDrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

void glcdSetXY(uint16_t x, uint16_t y);

void glcdLoadFont(const uint8_t *font, uint16_t color, uint16_t bgColor);
void glcdWriteChar(uint8_t code);
void glcdWriteString(char *string);

void glcdLoadLcdFont(const uint8_t *font, uint16_t color, uint16_t bgColor);
void glcdSkipLcdChar(uint8_t code);
void glcdWriteLcdChar(uint8_t code);
void glcdWriteLcdString(char *string);

#endif // GLCD_H
