#ifndef MEASURE_H
#define MEASURE_H

#include <inttypes.h>

#define ANTI_BOUNCE     5

void measureInit(void);
void measureInc8ms(void);

int32_t getCurrentSpeed(void);      // in m/s
int32_t getCurrentTrack(void);      // in m
int32_t getTotalDistance(void);     // in m
int32_t getTrackTime(void);

#endif // MEASURE_H
