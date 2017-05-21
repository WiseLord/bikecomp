#include <avr/io.h>
#include <util/delay.h>

#include "pins.h"

int main(void)
{
    DDR(LED_GREEN) |= LED_GREE_LINE;

    while (1) {
        PORT(LED_GREEN) &= ~LED_GREE_LINE;
        _delay_ms(250);
        PORT(LED_GREEN) |= LED_GREE_LINE;
        _delay_ms(250);
    }

    return 0;
}
