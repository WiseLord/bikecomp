#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adcInit();
int16_t adcGetVoltage();
void adcStart();

#endif // ADC_H
