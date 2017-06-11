#ifndef MEASURE_H
#define MEASURE_H

#include <inttypes.h>

#define ANTI_BOUNCE     10

void measureInit(void);
void measureAntiBounce(void);

uint16_t getCurrentSpeed(void);
int32_t getCurrentTrack(void);
int32_t getTotalDistance(void);

#endif // MEASURE_H
