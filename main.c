#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "ili9341.h"
#include "glcd.h"
#include "fonts.h"
#include "screen.h"
#include "measure.h"
#include "input.h"
#include "action.h"

void hwInit()
{
    glcdInit();
    inputInit();
    measureInit();
    sei();

    return;
}

int main(void)
{
    hwInit();

    while (1) {
        actionGetInput();
        actionHandle();
        screenUpdate();
    }

    return 0;
}
