#include "measure.h"
#include <avr/interrupt.h>
#include "pins.h"

static volatile int32_t wheelTurns = 0;
static volatile int32_t pedalTurns = 0;
static volatile uint8_t wheelAntiBounce = ANTI_BOUNCE;
static volatile uint8_t pedalAntiBounce = ANTI_BOUNCE;
static volatile uint16_t wheelCnt = 0;
static volatile uint16_t pedalCnt = 0;
static volatile uint16_t wheelCntBuf = 0;
static volatile uint16_t pedalCntBuf = 0;
static volatile int32_t trackTime = 0;

// Data saved in eeprom
int32_t totalDistance = 12356;
uint16_t wheelLength = 2075;

void measureInit()
{
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
}

void measureInc8ms()
{
    if (wheelAntiBounce)
        wheelAntiBounce--;
    if (pedalAntiBounce)
        pedalAntiBounce--;
    trackTime++;
}

ISR(INT0_vect)
{
    if (!wheelAntiBounce) {
        wheelTurns++;
        wheelAntiBounce = ANTI_BOUNCE;
        wheelCntBuf = wheelCnt + TCNT1;
        pedalCnt += TCNT1;
        TCNT1 = 0;
        wheelCnt = 0;
    }
}

ISR(INT1_vect)
{
    if (!pedalAntiBounce) {
        pedalTurns++;
        pedalAntiBounce = ANTI_BOUNCE;
        pedalCntBuf = pedalCnt + TCNT1;
        wheelCnt += TCNT1;
        TCNT1 = 0;
        pedalCnt = 0;
    }
}

int32_t getCurrentSpeed(void)
{
    // Timer1: 16MHz / PSK = 15625 clocks/sec

    if (wheelCntBuf)
        return 15625L * wheelLength / wheelCntBuf;
    else
        return 0;
}

int32_t getCurrentTrack(void)
{
    int32_t ret = wheelTurns;

    ret *= wheelLength;
    ret /= 1000;

    return ret;
}

int32_t getTotalDistance(void)
{
    return getCurrentTrack() + totalDistance;
}

int32_t getTrackTime()
{
    return trackTime / 125;
}
