#ifndef SCREEN_H
#define SCREEN_H

#include <inttypes.h>

typedef enum {
    SCREEN_MAIN,

    SCREEN_END
} Screen;

void showScreenMain(void);
void updateMainScreen(void);

#endif // SCREEN_H
