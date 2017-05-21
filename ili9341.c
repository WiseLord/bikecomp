#include "ili9341.h"

#include <util/delay.h>

LCD_Options ILI9341_Opts;

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
    _delay_ms(10);
    SET(ILI9341_RST);
    _delay_ms(10);

    ili9341SendCmd(ILI9341_SWRESET);
    _delay_ms(10);

    ili9341SendCmd(ILI9341_PWCTRLA);
    ili9341WriteData(0x39);
    ili9341WriteData(0x2C);
    ili9341WriteData(0x00);
    ili9341WriteData(0x34);
    ili9341WriteData(0x02);

    ili9341SendCmd(ILI9341_PWCTRLB);
    ili9341WriteData(0x00);
    ili9341WriteData(0xC1);
    ili9341WriteData(0x30);

    ili9341SendCmd(ILI9341_DRVTIMCTLA1);
    ili9341WriteData(0x85);
    ili9341WriteData(0x00);
    ili9341WriteData(0x78);

    ili9341SendCmd(ILI9341_DRVTIMB);
    ili9341WriteData(0x00);
    ili9341WriteData(0x00);

    ili9341SendCmd(ILI9341_PWSEQCTL);
    ili9341WriteData(0x64);
    ili9341WriteData(0x03);
    ili9341WriteData(0x12);
    ili9341WriteData(0x81);

    ili9341SendCmd(ILI9341_PUMPRTCTL);
    ili9341WriteData(0x20);

    ili9341SendCmd(ILI9341_PWCTRL1);
    ili9341WriteData(0x23);

    ili9341SendCmd(ILI9341_PWCTRL2);
    ili9341WriteData(0x10);

    ili9341SendCmd(ILI9341_VMCTRL1);
    ili9341WriteData(0x3E);
    ili9341WriteData(0x28);

    ili9341SendCmd(ILI9341_VMCTRL2);
    ili9341WriteData(0x86);

    ili9341SendCmd(ILI9341_MADCTL);
    ili9341WriteData(0x48);

    ili9341SendCmd(ILI9341_PIXSET);
    ili9341WriteData(0x55);

    ili9341SendCmd(ILI9341_FRMCTR1);
    ili9341WriteData(0x00);
    ili9341WriteData(0x18);

    ili9341SendCmd(ILI9341_DISCTRL);
    ili9341WriteData(0x08);
    ili9341WriteData(0x82);
    ili9341WriteData(0x27);

    ili9341SendCmd(ILI9341_EN3G);
    ili9341WriteData(0x00);

    ili9341SendCmd(ILI9341_GAMSET);
    ili9341WriteData(0x01);

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

    _delay_ms(150);
    ili9341SendCmd(ILI9341_SLPOUT);
    _delay_ms(10);

    ili9341SendCmd(ILI9341_DISPON);
}

static void ili9341SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ili9341SendCmd(ILI9341_CASET);
    ili9341SendData(x0);
    ili9341SendData(x1);

    ili9341SendCmd(ILI9341_PASET);
    ili9341SendData(y0);
    ili9341SendData(y1);

    ili9341SendCmd(ILI9341_RAMWR);
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
    ili9341Rotate(LCD_Orientation_Landscape_1);
}

void ili9341Rotate(LCD_Orientation orientation)
{
    ili9341SendCmd(ILI9341_MADCTL);

    switch (orientation) {
    case LCD_Orientation_Portrait_1:
        ili9341WriteData(0x58);
        ILI9341_Opts.width = ILI9341_WIDTH;
        ILI9341_Opts.height = ILI9341_HEIGHT;
        ILI9341_Opts.orientation = orientation;
        break;
    case LCD_Orientation_Portrait_2:
        ili9341WriteData(0x88);
        ILI9341_Opts.width = ILI9341_WIDTH;
        ILI9341_Opts.height = ILI9341_HEIGHT;
        ILI9341_Opts.orientation = orientation;
        break;
    case LCD_Orientation_Landscape_1:
        ili9341WriteData(0x28);
        ILI9341_Opts.width = ILI9341_HEIGHT;
        ILI9341_Opts.height = ILI9341_WIDTH;
        ILI9341_Opts.orientation = orientation;
        break;
    default:
        ili9341WriteData(0xE8);
        ILI9341_Opts.width = ILI9341_HEIGHT;
        ILI9341_Opts.height = ILI9341_WIDTH;
        ILI9341_Opts.orientation = orientation;
        break;
    }
}

void ili9431DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    ili9341SetWindow(x, y, x, y);
    ili9341SendData(color);
}

void ili9341DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    int16_t sX, sY, dX, dY, err, err2;

    sX = x0 < x1 ? 1 : -1;
    sY = y0 < y1 ? 1 : -1;
    dX = sX > 0 ? x1 - x0 : x0 - x1;
    dY = sY > 0 ? y1 - y0 : y0 - y1;
    err = dX - dY;

    while (x0 != x1 || y0 != y1) {
        ili9431DrawPixel(x0, y0, color);
        err2 = err * 2;
        if (err2 > -dY / 2) {
            err -= dY;
            x0 += sX;
        }
        if (err2 < dX) {
            err += dX;
            y0 += sY;
        }
    }
    ili9431DrawPixel(x1, y1, color);
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

void ili9341Fill(uint16_t color)
{
    ili9341DrawRectangle(0, 0, ILI9341_Opts.width, ILI9341_Opts.height, LCD_COLOR_BLACK);
}

void ili9341DrawHorizLine(uint16_t x0, uint16_t x1, uint16_t y, uint16_t color)
{
    ili9341DrawRectangle(x0, y, x1, y, color);
}

void ili9341DrawVertLine(uint16_t x, uint16_t y0, uint16_t y1, uint16_t color)
{
    ili9341DrawRectangle(x, y0, x, y1, color);
}

void ili9341DrawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    ili9341DrawHorizLine(x0, x1, y0, color);
    ili9341DrawVertLine(x0, y0, y1, color);
    ili9341DrawVertLine(x1, y0, y1, color);
    ili9341DrawHorizLine(x0, x1, y1, color);
}

void ili9341DrawRing(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ili9431DrawPixel(x0, y0 + r, color);
    ili9431DrawPixel(x0, y0 - r, color);
    ili9431DrawPixel(x0 + r, y0, color);
    ili9431DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ili9431DrawPixel(x0 + x, y0 + y, color);
        ili9431DrawPixel(x0 - x, y0 + y, color);
        ili9431DrawPixel(x0 + x, y0 - y, color);
        ili9431DrawPixel(x0 - x, y0 - y, color);

        ili9431DrawPixel(x0 + y, y0 + x, color);
        ili9431DrawPixel(x0 - y, y0 + x, color);
        ili9431DrawPixel(x0 + y, y0 - x, color);
        ili9431DrawPixel(x0 - y, y0 - x, color);
    }
}

void ili9341DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ili9341DrawHorizLine(x0 - r, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ili9341DrawHorizLine(x0 - x, x0 + x, y0 + y, color);
        ili9341DrawHorizLine(x0 - x, x0 + x, y0 - y, color);
        ili9341DrawHorizLine(x0 - y, x0 + y, y0 + x, color);
        ili9341DrawHorizLine(x0 - y, x0 + y, y0 - x, color);
    }
}

void ili9341DrawColorMap(void)
{
    uint16_t r, g, b, color;
    uint16_t i, j;

    ili9341SetWindow(0, 0, ILI9341_Opts.width - 1, ILI9341_Opts.height - 1);

    uint8_t quarter = ILI9341_Opts.width / 4;

    SET(ILI9341_DC);
    CLR(ILI9341_CS);
    for (j = 0; j < ILI9341_Opts.height; j++) {
        r = 0;
        b = 0;
        g = j * 63 / (ILI9341_Opts.height - 1);
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
