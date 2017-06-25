#include "input.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "measure.h"
#include "pins.h"

static volatile uint8_t btnCmd = 0;         // Command buffer

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

    TIMSK0 |= (1 << OCIE0A);                // Overflow interrupt
    TCCR0A = (1 << WGM01);                  // CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00);     // PSK = 1024
    OCR0A = 124;                            // 16M/(OCR0A - 1)/PSK = 125 Hz
}

ISR (TIMER0_COMPA_vect, ISR_NOBLOCK)        // TIME_STEP_FREQ = 125 Hz
{
    static uint8_t btnCnt = 0;              // Buttons press duration
    static uint8_t btnPrev = BTN_STATE_0;   // Previous buttons state

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
            if (btnCnt == LONG_PRESS) {
                btnCmd = (btnPrev << 4);
                btnCnt = LONG_PRESS - AUTOREPEAT;
            }
        } else {
            btnPrev = btnNow;
        }
    } else {
        if ((btnCnt > SHORT_PRESS) && (btnCnt < LONG_PRESS - AUTOREPEAT))
            btnCmd = btnPrev;
        btnCnt = 0;
    }

    measureIncTime();

    return;
    // TODO: Temporary set sensor as output for sw interrupts
    OUT(SENSOR_WHEEL);
    OUT(SENSOR_PEDAL);
    static uint16_t max = 5 * 256;
    if (++max > 60 * 256)
        max = 5 * 256;

    static uint8_t w = 0;
    if (++w >= max / 256) {
        CLR(SENSOR_WHEEL);
        w = 0;
    } else {
        SET(SENSOR_WHEEL);
    }
    static uint8_t p = 0;
    if (++p >= max / 128) {
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
