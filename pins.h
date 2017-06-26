#ifndef PINS_H
#define PINS_H

#include <avr/io.h>
#include "arduinopromini.h"

#define CONCAT(x,y)         x ## y

#define DDR(x)              CONCAT(DDR,x)
#define PORT(x)             CONCAT(PORT,x)
#define PIN(x)              CONCAT(PIN,x)

#define OUT(x)              DDR(x) |= x ## _LINE
#define IN(x)               DDR(x) &= ~x ## _LINE
#define SET(x)              PORT(x) |= x ## _LINE
#define CLR(x)              PORT(x) &= ~x ## _LINE

// LCD non-SPI pins
#define ILI9341_RST         ARDUINO_A2
#define ILI9341_RST_LINE    ARDUINO_A2_LINE
#define ILI9341_DC          ARDUINO_A1
#define ILI9341_DC_LINE     ARDUINO_A1_LINE
#define ILI9341_LED         ARDUINO_A0
#define ILI9341_LED_LINE    ARDUINO_A0_LINE

// LCD SPI pins
#define ILI9341_SDI         ARDUINO_MOSI
#define ILI9341_SDI_LINE    ARDUINO_MOSI_LINE
#define ILI9341_SCK         ARDUINO_SCLK
#define ILI9341_SCK_LINE    ARDUINO_SCLK_LINE
#define ILI9341_SDO         ARDUINO_MISO
#define ILI9341_SDO_LINE    ARDUINO_MISO_LINE
#define ILI9341_CS          ARDUINO_SS
#define ILI9341_CS_LINE     ARDUINO_SS_LINE

// Buttons
#define BUTTON_1            ARDUINO_D4
#define BUTTON_1_LINE       ARDUINO_D4_LINE
#define BUTTON_2            ARDUINO_D5
#define BUTTON_2_LINE       ARDUINO_D5_LINE
#define BUTTON_3            ARDUINO_D6
#define BUTTON_3_LINE       ARDUINO_D6_LINE
#define PCINT_BUTTON1       PCINT20
#define PCINT_BUTTON2       PCINT21
#define PCINT_BUTTON3       PCINT22

// Sensors
#define SENSOR_WHEEL        ARDUINO_D2
#define SENSOR_WHEEL_LINE   ARDUINO_D2_LINE
#define SENSOR_PEDAL        ARDUINO_D3
#define SENSOR_PEDAL_LINE   ARDUINO_D3_LINE

#endif // PINS_H
