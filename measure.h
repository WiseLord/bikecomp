#ifndef MEASURE_H
#define MEASURE_H

#include <inttypes.h>

#define ANTI_BOUNCE     10

void measureInit(void);
void measureAntiBounce(void);

uint16_t getCurrentSpeed(void);
uint16_t getCurrentTrack(void);
uint32_t getTotalDistance(void);

#endif // MEASURE_H
