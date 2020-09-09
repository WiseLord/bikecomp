#ifndef ARDUINO_H
#define ARDUINO_H

#define ARDUINO_A0              C
#define ARDUINO_A0_LINE         (1<<0)
#define ARDUINO_A1              C
#define ARDUINO_A1_LINE         (1<<1)
#define ARDUINO_A2              C
#define ARDUINO_A2_LINE         (1<<2)
#define ARDUINO_A3              C
#define ARDUINO_A3_LINE         (1<<3)
#define ARDUINO_A4              C
#define ARDUINO_A4_LINE         (1<<4)
#define ARDUINO_A5              C
#define ARDUINO_A5_LINE         (1<<5)

#define ARDUINO_D0              D
#define ARDUINO_D0_LINE         (1<<0)
#define ARDUINO_D1              D
#define ARDUINO_D1_LINE         (1<<1)
#define ARDUINO_D2              D
#define ARDUINO_D2_LINE         (1<<2)
#define ARDUINO_D3              D
#define ARDUINO_D3_LINE         (1<<3)
#define ARDUINO_D4              D
#define ARDUINO_D4_LINE         (1<<4)
#define ARDUINO_D5              D
#define ARDUINO_D5_LINE         (1<<5)
#define ARDUINO_D6              D
#define ARDUINO_D6_LINE         (1<<6)
#define ARDUINO_D7              D
#define ARDUINO_D7_LINE         (1<<7)

#define ARDUINO_D8              B
#define ARDUINO_D8_LINE         (1<<0)
#define ARDUINO_D9              B
#define ARDUINO_D9_LINE         (1<<1)
#define ARDUINO_D10             B
#define ARDUINO_D10_LINE        (1<<2)
#define ARDUINO_D11             B
#define ARDUINO_D11_LINE        (1<<3)
#define ARDUINO_D12             B
#define ARDUINO_D12_LINE        (1<<4)
#define ARDUINO_D13             B
#define ARDUINO_D13_LINE        (1<<5)

#define ARDUINO_MOSI            ARDUINO_D11
#define ARDUINO_MOSI_LINE       ARDUINO_D11_LINE
#define ARDUINO_MISO            ARDUINO_D12
#define ARDUINO_MISO_LINE       ARDUINO_D12_LINE
#define ARDUINO_SCLK            ARDUINO_D13
#define ARDUINO_SCLK_LINE       ARDUINO_D13_LINE
#define ARDUINO_SS              ARDUINO_D10
#define ARDUINO_SS_LINE         ARDUINO_D10_LINE

#define ARDUINO_SDA             ARDUINO_A4
#define ARDUINO_SDA_LINE        ARDUINO_A4_LINE
#define ARDUINO_SCL             ARDUINO_A5
#define ARDUINO_SCL_LINE        ARDUINO_A5_LINE

#endif /* ARDUINO_H */
