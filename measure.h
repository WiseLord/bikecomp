#ifndef MEASURE_H
#define MEASURE_H

#include <inttypes.h>

#define ANTI_BOUNCE     10

typedef enum {
    PARAM_SPEED,

    PARAM_TRACK,
    PARAM_TRACKTIME,
    PARAM_SPEED_AVG,
    PARAM_DISTANCE,

    PARAM_END,

    PARAM_SETUP_WHEEL,

    PARAM_SETUP_END
} Param;

void measureInit(void);
void measureInc8ms(void);
void measureSetWheel(int8_t diff);
void measureResetCurrent(void);
int32_t measureGetValue(Param param);

#endif // MEASURE_H
