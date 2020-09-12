#include "input.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "display/glcd.h"
#include "display/hw/avr.h"
#include "measure.h"

static volatile uint8_t btnCmd = 0;         // Command buffer

void inputInit(void)
{
    // Buttons as inputs
    IN(BTN1);
    IN(BTN2);
    IN(BTN3);
    // Enable pull-up resistors
    SET(BTN1);
    SET(BTN2);
    SET(BTN3);

    TCCR0A = (1 << WGM01);                  // CTC mode
    TCCR0B = (1 << CS02) | (1 << CS00);     // PSK = 1024
    OCR0A = 124;                            // 16M/(OCR0A - 1)/PSK = 125 Hz
}

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)        // TIME_STEP_FREQ = 125 Hz
{
    static uint8_t btnCnt = 0;              // Buttons press duration
    static uint8_t btnPrev = BTN_STATE_0;   // Previous buttons state

    uint8_t btnNow = BTN_STATE_0;

    uint8_t busData = ~glcdGetBus();

    if (busData & (1<<BTN1_Pin))
        btnNow |= BTN_0;
    if (busData & (1<<BTN2_Pin))
        btnNow |= BTN_1;
    if (busData & (1<<BTN3_Pin))
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
    static uint8_t batteryTimer = 0;
    if (batteryTimer++ == 0) {
        adcStart(); // Measure batter level every ~2 seconds
    }
}

uint8_t getBtnCmd()
{
    uint8_t ret = btnCmd;
    btnCmd = BTN_STATE_0;

    return ret;
}
