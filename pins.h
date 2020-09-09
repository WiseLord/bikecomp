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
