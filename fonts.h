#ifndef FONTS_H
#define FONTS_H

extern const uint8_t font_ks0066_ru_08[];
extern const uint8_t font_ks0066_ru_24[];
extern const uint8_t font_digits_32[];

typedef enum {
    FONT_HEIGHT = 0,
    FONT_LTSPPOS,
    FONT_CCNT,
    FONT_OFTA,
    FONT_OFTNA,

    FONT_HEADER_END
} FontHeader;

typedef struct {
    uint8_t height;
    uint8_t ltspPos;
    uint8_t ccnt;
    uint8_t ofta;
    uint8_t oftna;

    uint8_t fwd;
    uint8_t direction;
    union {
        uint16_t color;
        struct {
            uint8_t colorL;
            uint8_t colorH;
        };
    };
    union {
        uint16_t bgColor;
        struct {
            uint8_t bgColorL;
            uint8_t bgColorH;
        };
    };
} FontParam;

typedef enum {
	FONT_DIR_0,
	FONT_DIR_90,
	FONT_DIR_180,
	FONT_DIR_270
} FontDirection;

#endif /* FONTS_H */
