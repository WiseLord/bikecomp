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

    PARAM_END
} Param;

void measureInit(void);
void measureInc8ms(void);

int32_t getParam(Param param);
void resetCurrent(void);

#endif // MEASURE_H
