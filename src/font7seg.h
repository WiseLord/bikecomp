#ifndef FONT7SEG_H
#define FONT7SEG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "display/colors.h"

typedef struct {
    const __flash uint8_t *data;
    uint8_t width;
    uint8_t height;
    uint8_t thickness;
} tFont7seg;

extern const __flash tFont7seg font_7seg_13;
extern const __flash tFont7seg font_7seg_11;
extern const __flash tFont7seg font_7seg_10;
extern const __flash tFont7seg font_7seg_8;
extern const __flash tFont7seg font_7seg_7;
extern const __flash tFont7seg font_7seg_6;
extern const __flash tFont7seg font_7seg_5;
extern const __flash tFont7seg font_7seg_4;
extern const __flash tFont7seg font_7seg_3;

void font7segLoad(const __flash tFont7seg *font);
void font7segWriteChar(char code);
void font7segSkipChar(char code);
void font7segWriteString(const char *string);

#ifdef __cplusplus
}
#endif

#endif // FONT7SEG_H
