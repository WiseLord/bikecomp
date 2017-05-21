#include "ili9341.h"

#include <util/delay.h>

static uint16_t ili9341_x;
static uint16_t ili9341_y;

LCD_Options ILI9341_Opts;

static void ILI9341_SPI_Send(uint8_t data)
{
    // Fill output buffer with data
    SPDR = data;

    // Wait for transmission to complete
    while (!(SPSR & (1 << SPIF)));

    return;
}

void ILI9341_Init(void)
{
    OUT(ILI9341_DC);
    OUT(ILI9341_RST);
    OUT(ILI9341_LED);

    CLR(ILI9341_CS);
    SET(ILI9341_LED);

    ILI9341_SPI_Init();
    ILI9341_InitLCD();

    ili9341_x = ili9341_y = 0;

    ILI9341_Opts.width = ILI9341_WIDTH;
    ILI9341_Opts.height = ILI9341_HEIGHT;
    ILI9341_Opts.orientation = LCD_Orientation_Portrait_1;

    return;
}

void ILI9341_SPI_Init(void)
{
    OUT(ILI9341_SDI);
    OUT(ILI9341_SCK);
    OUT(ILI9341_CS);

    // Configure Hardware SPI
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPI2X);

    return;
}


void ILI9341_SendCmd(uint8_t cmd)
{
    CLR(ILI9341_DC);

    ILI9341_SPI_Send(cmd);

    return;
}

void ILI9341_SendData(uint8_t data)
{
    SET(ILI9341_DC);

    ILI9341_SPI_Send(data);

    return;
}

void ILI9341_InitLCD(void)
{
    CLR(ILI9341_RST);
    _delay_ms(10);
    SET(ILI9341_RST);
    _delay_ms(10);

    ILI9341_SendCmd(ILI9341_SWRESET);
    _delay_ms(10);

    ILI9341_SendCmd(ILI9341_PWCTRLA);
    ILI9341_SendData(0x39);
    ILI9341_SendData(0x2C);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x34);
    ILI9341_SendData(0x02);

    ILI9341_SendCmd(ILI9341_PWCTRLB);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0xC1);
    ILI9341_SendData(0x30);

    ILI9341_SendCmd(ILI9341_DRVTIMCTLA1);
    ILI9341_SendData(0x85);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x78);

    ILI9341_SendCmd(ILI9341_DRVTIMB);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x00);

    ILI9341_SendCmd(ILI9341_PWSEQCTL);
    ILI9341_SendData(0x64);
    ILI9341_SendData(0x03);
    ILI9341_SendData(0x12);
    ILI9341_SendData(0x81);

    ILI9341_SendCmd(ILI9341_PUMPRTCTL);
    ILI9341_SendData(0x20);

    ILI9341_SendCmd(ILI9341_PWCTRL1);
    ILI9341_SendData(0x23);

    ILI9341_SendCmd(ILI9341_PWCTRL2);
    ILI9341_SendData(0x10);

    ILI9341_SendCmd(ILI9341_VMCTRL1);
    ILI9341_SendData(0x3E);
    ILI9341_SendData(0x28);

    ILI9341_SendCmd(ILI9341_VMCTRL2);
    ILI9341_SendData(0x86);

    ILI9341_SendCmd(ILI9341_MADCTL);
    ILI9341_SendData(0x48);

    ILI9341_SendCmd(ILI9341_PIXSET);
    ILI9341_SendData(0x55);

    ILI9341_SendCmd(ILI9341_FRMCTR1);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x18);

    ILI9341_SendCmd(ILI9341_DISCTRL);
    ILI9341_SendData(0x08);
    ILI9341_SendData(0x82);
    ILI9341_SendData(0x27);

    ILI9341_SendCmd(ILI9341_EN3G);
    ILI9341_SendData(0x00);

    ILI9341_SendCmd(ILI9341_CASET);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0xEF);
    ILI9341_SendCmd(ILI9341_PASET);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x01);
    ILI9341_SendData(0x3F);

    ILI9341_SendCmd(ILI9341_GAMSET);
    ILI9341_SendData(0x01);

    ILI9341_SendCmd(ILI9341_PGAMCTRL);
    ILI9341_SendData(0x0F);
    ILI9341_SendData(0x31);
    ILI9341_SendData(0x2B);
    ILI9341_SendData(0x0C);
    ILI9341_SendData(0x0E);
    ILI9341_SendData(0x08);
    ILI9341_SendData(0x4E);
    ILI9341_SendData(0xF1);
    ILI9341_SendData(0x37);
    ILI9341_SendData(0x07);
    ILI9341_SendData(0x10);
    ILI9341_SendData(0x03);
    ILI9341_SendData(0x0E);
    ILI9341_SendData(0x09);
    ILI9341_SendData(0x00);

    ILI9341_SendCmd(ILI9341_NGAMCTRL);
    ILI9341_SendData(0x00);
    ILI9341_SendData(0x0E);
    ILI9341_SendData(0x14);
    ILI9341_SendData(0x03);
    ILI9341_SendData(0x11);
    ILI9341_SendData(0x07);
    ILI9341_SendData(0x31);
    ILI9341_SendData(0xC1);
    ILI9341_SendData(0x48);
    ILI9341_SendData(0x08);
    ILI9341_SendData(0x0F);
    ILI9341_SendData(0x0C);
    ILI9341_SendData(0x31);
    ILI9341_SendData(0x36);
    ILI9341_SendData(0x0F);

    _delay_ms(150);
    ILI9341_SendCmd(ILI9341_SLPOUT);
    _delay_ms(10);

    ILI9341_SendCmd(ILI9341_DISPON);
    ILI9341_SendCmd(ILI9341_RAMWR);

    return;
}

void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    ILI9341_SetCursorPos(x, y, x, y);

    ILI9341_SendCmd(ILI9341_RAMWR);
    ILI9341_SendData(color >> 8);
    ILI9341_SendData(color & 0xFF);

    return;
}

void ILI9341_SetCursorPos(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    ILI9341_SendCmd(ILI9341_CASET);
    ILI9341_SendData(x1 >> 8);
    ILI9341_SendData(x1 & 0xFF);
    ILI9341_SendData(x2 >> 8);
    ILI9341_SendData(x2 & 0xFF);

    ILI9341_SendCmd(ILI9341_PASET);
    ILI9341_SendData(y1 >> 8);
    ILI9341_SendData(y1 & 0xFF);
    ILI9341_SendData(y2 >> 8);
    ILI9341_SendData(y2 & 0xFF);

    return;
}

void ILI9341_Fill(uint16_t color)
{
    uint32_t i;

    ILI9341_SetCursorPos(0, 0, ILI9341_Opts.width - 1, ILI9341_Opts.height - 1);
    ILI9341_SendCmd(ILI9341_RAMWR);

    for (i = 0; i < ILI9341_PIXEL; i++) {
        ILI9341_SendData(color >> 8);
        ILI9341_SendData(color & 0xFF);
    }

    return;
}

void ILI9341_Rotate(LCD_Orientation orientation)
{
    ILI9341_SendCmd(ILI9341_MADCTL);

    switch (orientation) {
    case LCD_Orientation_Portrait_1:
        ILI9341_SendData(0x58);
        ILI9341_Opts.width = ILI9341_WIDTH;
        ILI9341_Opts.height = ILI9341_HEIGHT;
        ILI9341_Opts.orientation = orientation;
        break;
    case LCD_Orientation_Portrait_2:
        ILI9341_SendData(0x88);
        ILI9341_Opts.width = ILI9341_WIDTH;
        ILI9341_Opts.height = ILI9341_HEIGHT;
        ILI9341_Opts.orientation = orientation;
        break;
    case LCD_Orientation_Landscape_1:
        ILI9341_SendData(0x28);
        ILI9341_Opts.width = ILI9341_HEIGHT;
        ILI9341_Opts.orientation = orientation;
        break;
    case LCD_Orientation_Landscape_2:
        ILI9341_SendData(0xE8);
        ILI9341_Opts.width = ILI9341_HEIGHT;
        ILI9341_Opts.orientation = orientation;
        break;
    default:
        break;
    }

    return;
}

void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    int16_t sX, sY, dX, dY, err, err2;

    sX = x0 < x1 ? 1 : -1;
    sY = y0 < y1 ? 1 : -1;
    dX = sX > 0 ? x1 - x0 : x0 - x1;
    dY = sY > 0 ? y1 - y0 : y0 - y1;
    err = dX - dY;

    while (x0 != x1 || y0 != y1) {
        ILI9341_DrawPixel(x0, y0, color);
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
    ILI9341_DrawPixel(x1, y1, color);

    return;
}

void ILI9341_DrawHorizLine(uint16_t x0, uint16_t x1, uint16_t y, uint16_t color)
{
    uint16_t i, len;

    len = x1 - x0 + 1;

    ILI9341_SetCursorPos(x0, y, x1, y);
    ILI9341_SendCmd(ILI9341_RAMWR);

    for (i = 0; i < len; i++) {
        ILI9341_SendData(color >> 8);
        ILI9341_SendData(color & 0xFF);
    }

    return;
}

void ILI9341_DrawVertLine(uint16_t x, uint16_t y0, uint16_t y1, uint16_t color)
{
    uint16_t i, len;

    len = y1 - y0 + 1;

    ILI9341_SetCursorPos(x, y0, x, y1);
    ILI9341_SendCmd(ILI9341_RAMWR);

    for (i = 0; i < len; i++) {
        ILI9341_SendData(color >> 8);
        ILI9341_SendData(color & 0xFF);
    }

    return;
}

void ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    ILI9341_DrawHorizLine(x0, x1, y0, color);
    ILI9341_DrawVertLine(x0, y0, y1, color);
    ILI9341_DrawVertLine(x1, y0, y1, color);
    ILI9341_DrawHorizLine(x0, x1, y1, color);

    return;
}

void ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    uint16_t i, w, h;

    w = x1 - x0 + 1;
    h = y1 - y0 + 1;

    ILI9341_SetCursorPos(x0, y0, x1, y1);
    ILI9341_SendCmd(ILI9341_RAMWR);

    for (i = 0; i < w * h; i++) {
        ILI9341_SendData(color >> 8);
        ILI9341_SendData(color & 0xFF);
    }

    return;
}

void ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ILI9341_DrawPixel(x0, y0 + r, color);
    ILI9341_DrawPixel(x0, y0 - r, color);
    ILI9341_DrawPixel(x0 + r, y0, color);
    ILI9341_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawPixel(x0 + x, y0 + y, color);
        ILI9341_DrawPixel(x0 - x, y0 + y, color);
        ILI9341_DrawPixel(x0 + x, y0 - y, color);
        ILI9341_DrawPixel(x0 - x, y0 - y, color);

        ILI9341_DrawPixel(x0 + y, y0 + x, color);
        ILI9341_DrawPixel(x0 - y, y0 + x, color);
        ILI9341_DrawPixel(x0 + y, y0 - x, color);
        ILI9341_DrawPixel(x0 - y, y0 - x, color);
    }

    return;
}

void ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    ILI9341_DrawHorizLine(x0 - r, x0 + r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawHorizLine(x0 - x, x0 + x, y0 + y, color);
        ILI9341_DrawHorizLine(x0 - x, x0 + x, y0 - y, color);
        ILI9341_DrawHorizLine(x0 - y, x0 + y, y0 + x, color);
        ILI9341_DrawHorizLine(x0 - y, x0 + y, y0 - x, color);
    }

    return;
}
