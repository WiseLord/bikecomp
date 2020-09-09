#ifndef MEASURE_H
#define MEASURE_H

#include <inttypes.h>

#define WHEEL_MIN_LENGTH    500
#define WHEEL_AVG_LENGTH    2055
#define WHEEL_MAX_LENGTH    3000

#define ANTI_BOUNCE         10
#define TIME_STEP_FREQ      125
#define AVG_MIN_TIME        20

#define SLEEP_TIMER         7  // ~4.2sec intervals => ~30 sec

typedef enum {
    PARAM_SPEED,

    PARAM_TRACK,
    PARAM_TRACK_TIME,
    PARAM_SPEED_AVG,
    PARAM_CADENCE,
    PARAM_DISTANCE,

    PARAM_END,

    PARAM_SETUP_AUTO_OFF,
    PARAM_SETUP_WHEEL,
    PARAM_SETUP_COLOR_MODE,

    PARAM_SETUP_END
} Param;

typedef enum {
    AUTO_OFF_30,
    AUTO_OFF_60,
    AUTO_OFF_90,
    AUTO_OFF_120,
    AUTO_OFF_150,
    AUTO_OFF_180,

    AUTO_OFF_END
} AutoOff;

void measureInit(void);
void measureIncTime(void);
void measureDiffAutoMode(int8_t diff);
void measureDiffWheel(int8_t diff);
void measurePauseCurrent(void);
void measureResetCurrent(void);
int32_t measureGetValue(Param param);
uint8_t measureGetSleepTimer(void);
void measureResetSleepTimer(void);
AutoOff measureGetAutoOff(void);

#endif // MEASURE_H
