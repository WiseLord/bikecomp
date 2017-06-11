#include "input.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "measure.h"
#include "pins.h"

static volatile uint8_t btnCmd = BTN_0;         // Command buffer

void inputInit(void)
{
    // Buttons as inputs
    IN(BUTTON_1);
    IN(BUTTON_2);
    IN(BUTTON_3);
    // Enable pull-up resistors
    SET(BUTTON_1);
    SET(BUTTON_2);
    SET(BUTTON_3);

    TIMSK0 |= (1 << TOIE0);                     // Overflow interrupt
    TCCR0B = (1 << CS02) | (0 << CS01) | (1 << CS00); // PSK = 1024
}

ISR (TIMER0_OVF_vect, ISR_NOBLOCK)              // 16M/256/PSK = ~61 polls/sec
{
    static uint8_t btnCnt = 0;                  // Buttons press duration
    static uint8_t btnPrev = BTN_STATE_0;       // Previous buttons state

    uint8_t btnNow = BTN_STATE_0;

    if (~PIN(BUTTON_1) & BUTTON_1_LINE)
        btnNow |= BTN_0;
    if (~PIN(BUTTON_2) & BUTTON_2_LINE)
        btnNow |= BTN_1;
    if (~PIN(BUTTON_3) & BUTTON_3_LINE)
        btnNow |= BTN_2;

    // If button event has happened, place it to command buffer
    if (btnNow) {
        if (btnNow == btnPrev) {
            btnCnt++;
            if (btnCnt == LONG_PRESS)
                btnCmd = (btnPrev << 4);
        } else {
            btnPrev = btnNow;
        }
    } else {
        if ((btnCnt > SHORT_PRESS) && (btnCnt < LONG_PRESS))
            btnCmd = btnPrev;
        btnCnt = 0;
    }

    measureAntiBounce();

    // TODO: Temporary generate software interrupts
    static uint8_t w = 0;
    if (++w > 5) {
        CLR(SENSOR_WHEEL);
        w = 0;
    } else {
        SET(SENSOR_WHEEL);
    }
    static uint8_t p = 0;
    if (++p > 7) {
        CLR(SENSOR_PEDAL);
        p = 0;
    } else {
        SET(SENSOR_PEDAL);
    }
}

uint8_t getBtnCmd()
{
    uint8_t ret = btnCmd;
    btnCmd = BTN_STATE_0;

    return ret;
}
