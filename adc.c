#include "adc.h"

#include <avr/io.h>

void adcInit()
{
    // Set prescaler to 16
    ADCSRA = (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);
    // Set internal VREF as input
    ADMUX = (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0);
    // Set AVcc as voltage reference
    ADMUX |= ((0 << REFS1) | (1 << REFS0));

    // Enable ADC
    ADCSRA |= (1 << ADEN);
}

int16_t adcGetVoltage()
{
    int16_t ret = ADCW;

    // Vref = 1100mV => 1023 ADC value
    if (ret)
        ret = (int16_t)(1023UL * 1100 / ret);

    return ret;
}

void adcStart()
{
    ADCSRA |= 1 << ADSC;            // Start new measurement
}
