#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "ili9341.h"
#include "glcd.h"
#include "fonts.h"
#include "screen.h"
#include "measure.h"
#include "input.h"

void hwInit()
{
    glcdInit();
    inputInit();
    measureInit();
    screenInit();

    // Setup sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    PCMSK2 |= (BUTTON_1_LINE | BUTTON_2_LINE | BUTTON_3_LINE);

    // Interrupts
    TIMSK0 |= (1 << OCIE0A);    // Input timer compare
    TIMSK1 |= (1 << TOIE1);     // Measure timer overflow
    EIMSK |= (1 << INT0);       // Wheel sensor
    EIMSK |= (1 << INT1);       // Pedal sensor
    sei();

    return;
}

void sleep(void)
{
    // Prepare sleep
    glcdSleep();
    TIMSK0 &= ~(1 << OCIE0A);   // Input timer compare disable
    TIMSK1 &= ~(1 << TOIE1);    // Measure timer overflow disable
    PCICR |= (1<<PCIE2);        // Buttons interrupt enable

    // Sleep
    sleep_mode();

    // Wakeup
    PCICR &= ~(1<<PCIE2);       // Buttons interrupt disable
    TIMSK0 |= (1 << OCIE0A);    // Input timer compare enable
    TIMSK1 |= (1 << TOIE1);     // Measure timer overflow enable
    glcdWakeup();
}

int main(void)
{
    hwInit();
    screenShowMain();

    while (1) {
        Screen screen = screenGet();
        uint8_t btnCmd = getBtnCmd();

        if (measureGetSleepTimer() == 0) {
            btnCmd = BTN_STATE_0;
            sleep();
        }

        if (btnCmd)
            measureResetSleepTimer();

        switch (btnCmd) {
        case BTN_0:
            switch (screen) {
            case SCREEN_SETUP:
                switchParamSetup();
                break;
            default:
                break;
            }
            break;
        case BTN_1:
            switch (screen) {
            case SCREEN_MAIN:
                switchParam(SECTION_MAIN_MID);
                break;
            case SCREEN_SETUP:
                diffParamSetup(-1);
                break;
            default:
                break;
            }
            break;
        case BTN_2:
            switch (screen) {
            case SCREEN_MAIN:
                switchParam(SECTION_MAIN_BTM);
                break;
            case SCREEN_SETUP:
                diffParamSetup(+1);
                break;
            default:
                break;
            }
            break;
        case BTN_0_LONG:
            switch (screen) {
            case SCREEN_MAIN:
                screenShowSetup();
                break;
            case SCREEN_SETUP:
                screenShowMain();
                break;
            default:
                break;
            }
            break;
        case BTN_1_LONG:
            switch (screen) {
            case SCREEN_MAIN:
                measurePauseCurrent();
                break;
            case SCREEN_SETUP:
                diffParamSetup(-10);
                break;
            default:
                break;
            }
            break;
        case BTN_2_LONG:
            switch (screen) {
            case SCREEN_MAIN:
                measureResetCurrent();
                break;
            case SCREEN_SETUP:
                diffParamSetup(+10);
                break;
            default:
                break;
            }
            break;
        case BTN_1_LONG | BTN_2_LONG:
            switch (screen) {
            default:
                break;
            }
            break;
        default:
            break;
        }
        screenUpdate();
    }

    return 0;
}
