#ifndef INPUT_H
#define INPUT_H

#include <inttypes.h>

#define BTN1_Port               D
#define BTN1_Pin                4
#define BTN2_Port               D
#define BTN2_Pin                5
#define BTN3_Port               D
#define BTN3_Pin                6

#define BTN_STATE_0             0x00
#define BTN_0                   (1 << 0)
#define BTN_1                   (1 << 1)
#define BTN_2                   (1 << 2)
#define BTN_3                   (1 << 3)
#define BTN_0_LONG              (BTN_0 << 4)
#define BTN_1_LONG              (BTN_1 << 4)
#define BTN_2_LONG              (BTN_2 << 4)
#define BTN_4_LONG              (BTN_3 << 4)

// Handling long press actions
#define SHORT_PRESS             12
#define LONG_PRESS              96
#define AUTOREPEAT              60

void inputInit(void);

uint8_t getBtnCmd(void);

#endif // INPUT_H
