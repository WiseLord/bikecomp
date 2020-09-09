#ifndef GLCD_H
#define GLCD_H

#include <stdbool.h>
#include <stdint.h>

#include "ili9341.h"

#include "fonts.h"
#include "icons.h"

typedef uint16_t color_t;

//Colors
#define RGB_TO_565(x)                   (((x >> 8) & 0xF800) | ((x >> 5) & 0x7E0) | ((x >> 3) & 0x1F))
#define COLOR_BLACK                     RGB_TO_565(0x000000)
#define COLOR_NAVI                      RGB_TO_565(0x000080)
#define COLOR_BLUE                      RGB_TO_565(0x0000FF)
#define COLOR_GREEN                     RGB_TO_565(0x008000)
#define COLOR_TEAL                      RGB_TO_565(0x008080)
#define COLOR_DODGER_BLUE               RGB_TO_565(0x0080FF)
#define COLOR_LIME                      RGB_TO_565(0x00FF00)
#define COLOR_SPRING_GREEN              RGB_TO_565(0x00FF80)
#define COLOR_AQUA                      RGB_TO_565(0x00FFFF)
#define COLOR_NERO                      RGB_TO_565(0x252525)
#define COLOR_ECLIPSE                   RGB_TO_565(0x3F3939)
#define COLOR_MAROON                    RGB_TO_565(0x800000)
#define COLOR_PURPLE                    RGB_TO_565(0x800080)
#define COLOR_ELECTRIC_INDIGO           RGB_TO_565(0x8000FF)
#define COLOR_OLIVE                     RGB_TO_565(0x808000)
#define COLOR_GRAY                      RGB_TO_565(0x808080)
#define COLOR_LIGHT_SLATE_BLUE          RGB_TO_565(0x8080FF)
#define COLOR_CHARTREUSE                RGB_TO_565(0x80FF00)
#define COLOR_LIGHT_GREEN               RGB_TO_565(0x80FF80)
#define COLOR_ELECTRIC_BLUE             RGB_TO_565(0x80FFFF)
#define COLOR_RED                       RGB_TO_565(0xFF0000)
#define COLOR_DEEP_PINK                 RGB_TO_565(0xFF0080)
#define COLOR_MAGENTA                   RGB_TO_565(0xFF00FF)
#define COLOR_DARK_ORANGE               RGB_TO_565(0xFF8000)
#define COLOR_LIGHT_CORAL               RGB_TO_565(0xFF8080)
#define COLOR_FUCHSIA_PINK              RGB_TO_565(0xFF80FF)
#define COLOR_YELLOW                    RGB_TO_565(0xFFFF00)
#define COLOR_WITCH_HAZE                RGB_TO_565(0xFFFF80)
#define COLOR_WHITE                     RGB_TO_565(0xFFFFFF)

typedef enum {
    GLCD_LANDSCAPE,
    GLCD_LANDSCAPE_ROT,
    GLCD_PORTRATE,
    GLCD_PORTRATE_ROT,
} GlcdOrientation;

typedef struct {
    uint16_t width;
    uint16_t height;
    GlcdOrientation orientation;
    int16_t x;
    int16_t y;
    const uint8_t *font;
    FontParam fp;
} GlcdOpts;

extern GlcdOpts glcdOpts;

void glcdInit(GlcdOrientation value);

void glcdSetBacklight(bool value);

void glcdSetFont(const __flash tFont *font);
void glcdSetFontColor(color_t color);
void glcdSetFontBgColor(color_t color);



void glcdSleep(bool value);

void glcdDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

void glcdSetXY(uint16_t x, uint16_t y);
void glcdSetY(uint16_t y);

void glcdWriteString(char *string);

void glcdWriteIcon(const uint8_t *icon, uint16_t color, uint16_t bgColor);

void glcdLoadLcdFont(const uint8_t *font, uint16_t color, uint16_t bgColor);
void glcdSkipLcdChar(uint8_t code);
void glcdWriteLcdChar(uint8_t code);

void glcdDrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

#endif // GLCD_H
