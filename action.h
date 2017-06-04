#ifndef ACTION_H
#define ACTION_H

#include <inttypes.h>

typedef enum {
    ACTION_SCR0,
    ACTION_SCR1,

    ACTION_END
} Action;

void actionGetInput(void);
void actionHandle(void);

#endif // ACTION_H
