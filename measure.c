#include "measure.h"

#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "eeprom.h"
#include "pins.h"

static volatile int32_t wheelTurns = 0;
static volatile int32_t wheelCnt = 0;
static volatile int32_t wheelCntBuf = 0;

static volatile int32_t pedalTurns = 0;
static volatile int32_t pedalCnt = 0;
static volatile int32_t pedalCntBuf = 0;

static volatile int32_t trackTime = 0;
static volatile int32_t trackTimeMove = 0;
static volatile uint8_t inMove = 0;

static volatile uint8_t wheelAntiBounce = ANTI_BOUNCE;
static volatile uint8_t pedalAntiBounce = ANTI_BOUNCE;

// Data saved in eeprom
static int32_t totalDistance;
static uint16_t wheelLength;

static int32_t getCurrentTrack(void)
{
    int32_t ret = wheelTurns;

    ret *= wheelLength;

    return ret;
}

ISR(INT0_vect)
{
    if (!wheelAntiBounce) {
        wheelAntiBounce = ANTI_BOUNCE;
        wheelTurns++;
        if (inMove)
            wheelCntBuf = wheelCnt + TCNT1;
        pedalCnt += TCNT1;
        TCNT1 = 0;
        wheelCnt = 0;
        inMove = 1;
    }
}

ISR(INT1_vect)
{
    if (!pedalAntiBounce) {
        pedalAntiBounce = ANTI_BOUNCE;
        pedalTurns++;
        if (inMove)
            pedalCntBuf = pedalCnt + TCNT1;
        wheelCnt += TCNT1;
        TCNT1 = 0;
        pedalCnt = 0;
        inMove = 1;
    }
}

ISR (TIMER1_OVF_vect)
{
    wheelCnt = 0;
    pedalCnt = 0;
    wheelCntBuf = 0;
    pedalCntBuf = 0;
    inMove = 0;
}

void measureInit(void)
{
    // Load data from EEPROM
    totalDistance = eeprom_read_dword((uint32_t*)EEPROM_DISTANCE);
    wheelLength = eeprom_read_word((uint16_t*)EEPROM_WHEEL);

    // Sensor lines as inputs
    IN(SENSOR_WHEEL);
    IN(SENSOR_PEDAL);
    // Enable pull-up resistors
    SET(SENSOR_WHEEL);
    SET(SENSOR_PEDAL);
    // INT0 on falling edge (wheel sensor)
    EICRA |= (1 << ISC01) | (0 << ISC00);
    EIMSK |= (1 << INT0);
    // INT1 on falling edge (pedal sensor)
    EICRA |= (1 << ISC11) | (0 << ISC10);
    EIMSK |= (1 << INT1);

    // 16bit Timer1 for measuring intervals
    TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10);   // PSK = 1024
    // Enable overflow interrupts
    TIMSK1 |= (1 << TOIE1);
}

void measureIncTime(void)
{
    if (wheelAntiBounce)
        wheelAntiBounce--;
    if (pedalAntiBounce)
        pedalAntiBounce--;
    trackTime++;
    if (inMove)
        trackTimeMove++;
}

void measureSetWheel(int8_t diff)
{
    wheelLength += diff;
    if (wheelLength < 500)
        wheelLength = 500;
    if (wheelLength > 3000)
        wheelLength = 3000;

    eeprom_update_word((uint16_t*)EEPROM_WHEEL, wheelLength);
}

void measureResetCurrent(void)
{
    totalDistance += getCurrentTrack() / 1000;
    wheelTurns = 0;
    pedalTurns = 0;
    trackTime = 0;
    trackTimeMove = 0;

    eeprom_update_dword((uint32_t*)EEPROM_DISTANCE, totalDistance);
}

int32_t measureGetValue(Param param)
{
    int32_t ret = 0;

    switch (param) {
    case PARAM_SPEED:
        if (inMove) {
            ret = wheelCntBuf;
            if (ret)
                ret = 15625L * wheelLength / ret;
        } else {
            ret = 0;
        }
        break;
    case PARAM_TRACK:
        ret = getCurrentTrack();
        break;
    case PARAM_TRACK_TIME:
        ret = trackTime / TIME_STEP_FREQ;
        break;
    case PARAM_TRACK_TIME_MOVE:
        ret = trackTimeMove / TIME_STEP_FREQ;
        break;
    case PARAM_SPEED_AVG:
        ret = trackTime;
        if (ret > AVG_MIN_TIME * TIME_STEP_FREQ)
            ret = getCurrentTrack() / ret * TIME_STEP_FREQ;
        else
            ret = -TIME_STEP_FREQ;
        break;
    case PARAM_SPEED_AVG_MOVE:
        ret = trackTimeMove;
        if (ret > AVG_MIN_TIME * TIME_STEP_FREQ)
            ret = getCurrentTrack() / ret * TIME_STEP_FREQ;
        else
            ret = -TIME_STEP_FREQ;
        break;
    case PARAM_DISTANCE:
        ret = getCurrentTrack() / 1000 + totalDistance;
        break;
    case PARAM_SETUP_WHEEL:
        ret = wheelLength;
        break;
    default:
        break;
    }

    return ret;
}
