#include "adc.h"

#include <avr/io.h>

#define LOW_BAT_ADC     350
#define FULL_BAT_ADC    280

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

int16_t adcGetVoltage(void)
{
    int16_t ret = ADCW;

    // Vref = 1100mV => 1023 ADC value
    if (ret)
        ret = (int16_t)(1023UL * 1100 / ret);

    return ret;
}

void adcStart(void)
{
    ADCSRA |= 1 << ADSC;            // Start new measurement
}

uint8_t adcGetPercent(void)
{
    uint16_t adc = ADCW;

    if (adc > LOW_BAT_ADC) {
        return 0;
    } else if (adc < FULL_BAT_ADC) {
        return 100;
    } else {
        return 100 * (LOW_BAT_ADC - adc) / (LOW_BAT_ADC - FULL_BAT_ADC);
    }
}
