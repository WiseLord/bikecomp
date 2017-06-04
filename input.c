#include "input.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "measure.h"
#include "pins.h"

static volatile uint8_t btnCmd;                 // Command buffer

void inputInit(void)
{
    DDR(BUTTON_1) &= ~BUTTON_1_LINE;
    DDR(BUTTON_2) &= ~BUTTON_2_LINE;
    DDR(BUTTON_3) &= ~BUTTON_3_LINE;

    PORT(BUTTON_1) |= BUTTON_1_LINE;
    PORT(BUTTON_2) |= BUTTON_2_LINE;
    PORT(BUTTON_3) |= BUTTON_3_LINE;

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

    testUpdate();
}

uint8_t getBtnCmd()
{
    uint8_t ret = btnCmd;
    btnCmd = BTN_STATE_0;

    return ret;
}
