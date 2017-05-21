#ifndef PINS_H
#define PINS_H

#include <avr/io.h>
#include "arduinopromini.h"

#define CONCAT(x,y)         x ## y

#define DDR(x)              CONCAT(DDR,x)
#define PORT(x)             CONCAT(PORT,x)
#define PIN(x)              CONCAT(PIN,x)


#define LED_GREEN           ARDUINO_D13
#define LED_GREE_LINE       ARDUINO_D13_LINE

#endif // PINS_H
