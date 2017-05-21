#include "glcd.h"

void glcdInit()
{
#if GLCD_TYPE == ILI9341
    ili9341Init();
#endif
}

void glcdDrawColorMap(void)
{
#if GLCD_TYPE == ILI9341
    ili9341DrawColorMap();
#endif
}
