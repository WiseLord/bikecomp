#include "measure.h"

static volatile uint16_t rawSpeed = 0;
static volatile uint16_t rawDistance = 0;

uint16_t getSpeed(void)
{
    return rawSpeed;
}

uint16_t getDistance(void)
{
    return rawDistance;
}

void testUpdate()
{
    static uint16_t x;
    if (++x >= 61) {
        x = 0;
        rawDistance++;
    }
//    if (rawDistance >= 10000)
//        rawDistance = 0;
//    rawSpeed = rawDistance / 10;
}
