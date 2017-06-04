#include "action.h"

#include "input.h"
#include "screen.h"
#include "glcd.h"

static Action action = ACTION_END;

void actionGetInput(void)
{
    action = ACTION_END;

    uint8_t btnCmd = getBtnCmd();

    switch (btnCmd) {
    case BTN_0:
        action = ACTION_SCR0;
        break;
    case BTN_1:
        action = ACTION_SCR1;
        break;
    default:
        break;
    }
}

void actionHandle()
{
    switch (action) {
    case ACTION_SCR0:
        screenShowMain();
        break;
    case ACTION_SCR1:
        screenShowSetup();
        break;
    default:
        break;
    }
}
