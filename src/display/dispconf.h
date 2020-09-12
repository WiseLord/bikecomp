#ifndef DISPCONF_H
#define DISPCONF_H

#ifdef __cplusplus
extern "C" {
#endif

// SPI library implementation
#ifdef _DISP_SPI
#define DISP_WAIT_BUSY()        while (!(SPSR & (1 << SPIF)))
#define DISP_SPI_INIT() \
    do { \
        OUT(DISP_SDI); \
        OUT(DISP_SCK); \
        SPCR = (1 << SPE) | (1 << MSTR); \
        SPSR = (1 << SPI2X); \
        SPDR = 0; \
    } while (0);
#define DISP_SPI_SEND_BYTE(x) \
    do { \
        DISP_WAIT_BUSY(); \
        (SPDR = x); \
    } while (0);
#else
#define DISP_WAIT_BUSY()        (void)0
#endif

// Third-party milliseconds delay function implementation
#include <util/delay.h>
#define DISP_MDELAY(x)          _delay_ms(x)

// TFT LCD pins description
#ifdef _DISP_16BIT
#define DISP_DATA_HI_Port       B
#define DISP_DATA_HI_Pin        0xFF
#define DISP_DATA_LO_Port       C
#define DISP_DATA_LO_Pin        0xFF
#else
#define DISP_DATA_Port          D
#define DISP_DATA_Pin           0xFF
#endif

#define DISP_CS_Port            B
#define DISP_CS_Pin             2
#define DISP_RS_Port            C
#define DISP_RS_Pin             1

#ifdef _DISP_SPI
#define DISP_SDI_Port           B
#define DISP_SDI_Pin            3
#define DISP_SCK_Port           B
#define DISP_SCK_Pin            5
#else
#define DISP_WR_Port            B
#define DISP_WR_Pin             3
#endif

#ifdef _DISP_READ_ENABLED
#define DISP_RD_Port            B
#define DISP_RD_Pin             4
#endif

#ifdef _DISP_RST_ENABLED
#define DISP_RST_Port           C
#define DISP_RST_Pin            2
#endif

#ifdef _DISP_BCKL_ENABLED
#define DISP_BCKL_Port          C
#define DISP_BCKL_Pin           0
#endif

#ifdef __cplusplus
}
#endif

#endif // DISPCONF_H
