#ifndef FONTS_H
#define FONTS_H

extern const uint8_t font_ks0066_ru_08[];
extern const uint8_t font_ks0066_ru_24[];
extern const uint8_t font_digits_32[];

extern const uint8_t lcdChar[];
extern const uint8_t font_lcd_72[];

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

#define BIT_A       (1<<0)
#define BIT_B       (1<<1)
#define BIT_C       (1<<2)
#define BIT_D       (1<<3)
#define BIT_E       (1<<4)
#define BIT_F       (1<<5)
#define BIT_G       (1<<6)
#define BIT_P       (1<<7)

#define CH_0        (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F)
#define CH_1        (BIT_B | BIT_C)
#define CH_2        (BIT_A | BIT_B | BIT_D | BIT_E | BIT_G)
#define CH_3        (BIT_A | BIT_B | BIT_C | BIT_D | BIT_G)
#define CH_4        (BIT_B | BIT_C | BIT_F | BIT_G)
#define CH_5        (BIT_A | BIT_C | BIT_D | BIT_F | BIT_G)
#define CH_6        (BIT_A | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define CH_7        (BIT_A | BIT_B | BIT_C)
#define CH_8        (BIT_A | BIT_B | BIT_C | BIT_D | BIT_E | BIT_F | BIT_G)
#define CH_9        (BIT_A | BIT_B | BIT_C | BIT_D | BIT_F | BIT_G)

#endif /* FONTS_H */
