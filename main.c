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

        uint8_t btnCmd = getBtnCmd();

        switch (btnCmd) {
        case BTN_0:
            screenShowMain();
            break;
        case BTN_1:
            switchParam(SECTION_MAIN_MID);
            break;
        case BTN_2:
            switchParam(SECTION_MAIN_BTM);
            break;
        case BTN_1_LONG | BTN_2_LONG:
            resetCurrent();
            break;
        default:
            break;
        }
        screenUpdate();
    }

    return 0;
}
