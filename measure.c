#include "measure.h"
#include <avr/interrupt.h>
#include "pins.h"

static volatile uint16_t wheelCnt = 0;
static volatile uint16_t pedalCnt = 0;
static volatile uint8_t wheelAntiBounce = ANTI_BOUNCE;
static volatile uint8_t pedalAntiBounce = ANTI_BOUNCE;

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
}

void measureAntiBounce()
{
    if (wheelAntiBounce)
        wheelAntiBounce--;
    if (pedalAntiBounce)
        pedalAntiBounce--;
}

ISR(INT0_vect)
{
    if (!wheelAntiBounce) {
        wheelCnt++;
        wheelAntiBounce = ANTI_BOUNCE;
    }
}

ISR(INT1_vect)
{
    if (!pedalAntiBounce) {
        pedalCnt++;
        pedalAntiBounce = ANTI_BOUNCE;
    }
}

uint16_t getCurrentSpeed(void)
{
    return 234;
}

uint16_t getCurrentTrack(void)
{
    return pedalCnt;
}

uint32_t getTotalDistance(void)
{
    return wheelCnt;
}


