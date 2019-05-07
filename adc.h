#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void adcInit(void);
int16_t adcGetVoltage(void);
uint8_t adcGetPercent(void);
void adcStart(void);

#endif // ADC_H
