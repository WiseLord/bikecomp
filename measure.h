#ifndef MEASURE_H
#define MEASURE_H

#include <inttypes.h>

#define ANTI_BOUNCE         10
#define TIME_STEP_FREQ      125
#define AVG_MIN_TIME        20

#define SLEEP_TIMER         7   // ~4.2sec intervals => ~30 sec

typedef enum {
    PARAM_SPEED,

    PARAM_TRACK,
    PARAM_TRACK_TIME,
    PARAM_TRACK_TIME_MOVE,
    PARAM_SPEED_AVG,
    PARAM_SPEED_AVG_MOVE,
    PARAM_CADENCE,
    PARAM_DISTANCE,

    PARAM_END,

    PARAM_SETUP_AUTO_OFF,
    PARAM_SETUP_WHEEL,
    PARAM_SETUP_COLOR_MODE,

    PARAM_SETUP_END
} Param;

void measureInit(void);
void measureIncTime(void);
void measureDiffWheel(int8_t diff);
void measurePauseCurrent(void);
void measureResetCurrent(void);
int32_t measureGetValue(Param param);
uint8_t measureGetSleepTimer(void);
void measureResetSleepTimer(void);

#endif // MEASURE_H
