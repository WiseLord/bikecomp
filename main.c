#include <avr/io.h>
#include <avr/interrupt.h>
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
    sei();

    return;
}

int main(void)
{
    hwInit();
    screenShowMain();

    while (1) {
        Screen screen = screenGet();
        uint8_t btnCmd = getBtnCmd();

        switch (btnCmd) {
        case BTN_1:
            switch (screen) {
            case SCREEN_MAIN:
                switchParam(SECTION_MAIN_MID);
                break;
            case SCREEN_SETUP:
                measureSetWheel(+1);
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
                measureSetWheel(-1);
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
        case BTN_1_LONG | BTN_2_LONG:
            switch (screen) {
            case SCREEN_MAIN:
                measureResetCurrent();
                break;
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
