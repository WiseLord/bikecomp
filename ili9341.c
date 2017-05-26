#include "ili9341.h"

#include <avr/pgmspace.h>
#include <util/delay.h>
#include "glcd.h"

static void inline __attribute__((always_inline)) ili9341SendSPI(uint8_t data)
{
    // Fill output buffer with data
    SPDR = data;

    // Wait for transmission to complete
    while (!(SPSR & (1 << SPIF)));
}

static void ili9341WriteData(uint8_t data)
{
    SET(ILI9341_DC);
    CLR(ILI9341_CS);
    ili9341SendSPI(data);
    SET(ILI9341_CS);
}

static void ili9341SendCmd(uint8_t cmd)
{
    CLR(ILI9341_DC);
    CLR(ILI9341_CS);
    ili9341SendSPI(cmd);
    SET(ILI9341_CS);
}

static void ili9341SendData(uint16_t data)
{
    uint8_t dataH = data >> 8;
    uint8_t dataL = data & 0xFF;

    SET(ILI9341_DC);
    CLR(ILI9341_CS);
    ili9341SendSPI(dataH);
    ili9341SendSPI(dataL);
    SET(ILI9341_CS);
}

static void ili9341InitSeq(void)
{
    CLR(ILI9341_RST);
    _delay_ms(100);
    SET(ILI9341_RST);
    _delay_ms(100);

    ili9341SendCmd(ILI9341_SWRESET);
    _delay_ms(10);
    ili9341SendCmd(ILI9341_DISPOFF);

    ili9341SendCmd(ILI9341_PWCTRLB);
    ili9341WriteData(0x00);
    ili9341WriteData(0xC1);
    ili9341WriteData(0x30);

    ili9341SendCmd(ILI9341_PWSEQCTL);
    ili9341WriteData(0x64);
    ili9341WriteData(0x03);
    ili9341WriteData(0x12);
    ili9341WriteData(0x81);

    ili9341SendCmd(ILI9341_DRVTIMCTLA1);
    ili9341WriteData(0x85);
    ili9341WriteData(0x01);
    ili9341WriteData(0x79);

    ili9341SendCmd(ILI9341_PWCTRLA);
    ili9341WriteData(0x39);
    ili9341WriteData(0x2C);
    ili9341WriteData(0x00);
    ili9341WriteData(0x34);
    ili9341WriteData(0x02);

    ili9341SendCmd(ILI9341_PUMPRTCTL);
    ili9341WriteData(0x20);

    ili9341SendCmd(ILI9341_DRVTIMB);
    ili9341WriteData(0x00);
    ili9341WriteData(0x00);

    ili9341SendCmd(ILI9341_PWCTRL1);
    ili9341WriteData(0x26);

    ili9341SendCmd(ILI9341_PWCTRL2);
    ili9341WriteData(0x11);

    ili9341SendCmd(ILI9341_VMCTRL1);
    ili9341WriteData(0x35);
    ili9341WriteData(0x3E);

    ili9341SendCmd(ILI9341_VMCTRL2);
    ili9341WriteData(0xBE);

    ili9341SendCmd(ILI9341_PIXSET);
    ili9341WriteData(0x55);

    ili9341SendCmd(ILI9341_FRMCTR1);
    ili9341WriteData(0x00);
    ili9341WriteData(0x1B);

    ili9341SendCmd(ILI9341_EN3G);
    ili9341WriteData(0x08);

    ili9341SendCmd(ILI9341_GAMSET);
    ili9341WriteData(0x01);

    ili9341SendCmd(ILI9341_ETMOD);
    ili9341WriteData(0x07);

    ili9341SendCmd(ILI9341_DISCTRL);
    ili9341WriteData(0x08);
    ili9341WriteData(0x82);
    ili9341WriteData(0x27);

    ili9341SendCmd(ILI9341_PGAMCTRL);
    ili9341WriteData(0x0F);
    ili9341WriteData(0x31);
    ili9341WriteData(0x2B);
    ili9341WriteData(0x0C);
    ili9341WriteData(0x0E);
    ili9341WriteData(0x08);
    ili9341WriteData(0x4E);
    ili9341WriteData(0xF1);
    ili9341WriteData(0x37);
    ili9341WriteData(0x07);
    ili9341WriteData(0x10);
    ili9341WriteData(0x03);
    ili9341WriteData(0x0E);
    ili9341WriteData(0x09);
    ili9341WriteData(0x00);

    ili9341SendCmd(ILI9341_NGAMCTRL);
    ili9341WriteData(0x00);
    ili9341WriteData(0x0E);
    ili9341WriteData(0x14);
    ili9341WriteData(0x03);
    ili9341WriteData(0x11);
    ili9341WriteData(0x07);
    ili9341WriteData(0x31);
    ili9341WriteData(0xC1);
    ili9341WriteData(0x48);
    ili9341WriteData(0x08);
    ili9341WriteData(0x0F);
    ili9341WriteData(0x0C);
    ili9341WriteData(0x31);
    ili9341WriteData(0x36);
    ili9341WriteData(0x0F);

    ili9341SendCmd(ILI9341_SLPOUT);
    _delay_ms(100);

    ili9341SendCmd(ILI9341_DISPON);
    _delay_ms(100);
}

void ili9341SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ili9341SendCmd(ILI9341_PASET);
    ili9341SendData(x0);
    ili9341SendData(x1);

    ili9341SendCmd(ILI9341_CASET);
    ili9341SendData(y0);
    ili9341SendData(y1);

    ili9341SendCmd(ILI9341_RAMWR);
}

static void ili9341Rotate(GlcdOrientation orientation)
{
    ili9341SendCmd(ILI9341_MADCTL);

    switch (orientation) {
    case LCD_Orientation_Portrait_1:
        ili9341WriteData(0x78);
        glcdOpts.width = ILI9341_WIDTH;
        glcdOpts.height = ILI9341_HEIGHT;
        glcdOpts.orientation = orientation;
        break;
    case LCD_Orientation_Portrait_2:
        ili9341WriteData(0xA8);
        glcdOpts.width = ILI9341_WIDTH;
        glcdOpts.height = ILI9341_HEIGHT;
        glcdOpts.orientation = orientation;
        break;
    case LCD_Orientation_Landscape_1:
        ili9341WriteData(0x08);
        glcdOpts.width = ILI9341_HEIGHT;
        glcdOpts.height = ILI9341_WIDTH;
        glcdOpts.orientation = orientation;
        break;
    default:
        ili9341WriteData(0xC8);
        glcdOpts.width = ILI9341_HEIGHT;
        glcdOpts.height = ILI9341_WIDTH;
        glcdOpts.orientation = orientation;
        break;
    }
}

void ili9341Init(void)
{
    // Non-SPI pins
    OUT(ILI9341_DC);
    OUT(ILI9341_RST);
    OUT(ILI9341_LED);

    CLR(ILI9341_CS);
    SET(ILI9341_LED);

    // SPI pins
    OUT(ILI9341_SDI);
    OUT(ILI9341_SCK);
    OUT(ILI9341_CS);

    // Configure Hardware SPI
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPI2X);

    // Init magic
    ili9341InitSeq();

    // Set display orientation and size
    ili9341Rotate(LCD_Orientation_Portrait_2);
}

void ili9431DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    ili9341SetWindow(x, y, x, y);
    ili9341SendData(color);
}

void ili9341DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint32_t i, j;

    uint16_t w = x1 - x0 + 1;
    uint16_t h = y1 - y0 + 1;

    ili9341SetWindow(x0, y0, x1, y1);

    uint8_t colorH = color >> 8;
    uint8_t colorL = color & 0xFF;

    SET(ILI9341_DC);
    CLR(ILI9341_CS);
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            ili9341SendSPI(colorH);
            ili9341SendSPI(colorL);
        }
    }
    SET(ILI9341_CS);
}

void ili9341WriteChar(const uint8_t *chOft, uint8_t fwd, uint8_t swd)
{
    uint8_t i;
    uint8_t j;
    uint8_t k;

    uint8_t pgmData;

    SET(ILI9341_DC);
    CLR(ILI9341_CS);
    for (i = 0; i < fwd; i++) {
        for (j = 0; j < fp.height; j++) {
            if (i >= swd)
                pgmData = 0x00;
            else
                pgmData = pgm_read_byte(chOft + (swd * j) + i);
            for (k = 0; k < 8; k++) {
                if (pgmData & 0x01) {
                    ili9341SendSPI(fp.colorH);
                    ili9341SendSPI(fp.colorL);
                } else {
                    ili9341SendSPI(fp.bgColorH);
                    ili9341SendSPI(fp.bgColorL);
                }
                pgmData >>= 1;
            }
        }
    }
    SET(ILI9341_CS);
}

void ili9341DrawColorMap(void)
{
    uint16_t r, g, b, color;
    uint16_t i, j;

    ili9341SetWindow(0, 0, glcdOpts.width - 1, glcdOpts.height - 1);

    uint8_t quarter = glcdOpts.height / 4;

    SET(ILI9341_DC);
    CLR(ILI9341_CS);
    for (j = 0; j < glcdOpts.width; j++) {
        r = 0;
        b = 0;
        g = j * 63 / (glcdOpts.width - 1);
        for (i = 0; i < quarter; i++) {
            r = i * 31 / (quarter - 1);
            color = (r << 11) | (g << 5) | (b << 0);
            ili9341SendSPI(color >> 8);
            ili9341SendSPI(color & 0xFF);
        }
        for (i = 0; i < quarter; i++) {
            b = i * 31 / (quarter - 1);
            color = (r << 11) | (g << 5) | (b << 0);
            ili9341SendSPI(color >> 8);
            ili9341SendSPI(color & 0xFF);
        }
        for (i = 0; i < quarter; i++) {
            r = ((quarter - 1) - i) * 31 / (quarter - 1);
            color = (r << 11) | (g << 5) | (b << 0);
            ili9341SendSPI(color >> 8);
            ili9341SendSPI(color & 0xFF);
        }
        for (i = 0; i < quarter; i++) {
            b = ((quarter - 1) - i) * 31 / (quarter - 1);
            color = (r << 11) | (g << 5) | (b << 0);
            ili9341SendSPI(color >> 8);
            ili9341SendSPI(color & 0xFF);
        }
    }
    SET(ILI9341_CS);
}
