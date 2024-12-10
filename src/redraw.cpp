#include "redraw.h"
#include "matrix.h"
#include "palettes.h"
#include "config.h"

#include <FastLED.h>

void drawBack()
{
    uint32_t count = (millis() / 64) * 50;
    uint8_t pal = CHOOSEN_PALLETTE;
    uint8_t scale = 50;
    uint8_t bright = 60;

    matrix.setModeXY();
    matrix.setBright(bright);

    for (int y = 0; y < matrix.height(); y++)
    {
        for (int x = 0; x < matrix.width(); x++)
        {
            if (matrix.ledXY(x, y) < 0)
                continue;
            uint32_t col = getPaletteColor(pal, inoise16(x * scale * 64, y * scale * 64, count * 32), bright);
            matrix.setLED(x, y, col);
        }
    }
    // matrix.update();
}

void drawChoosenPallette(uint16_t pal)
{
    for (int i = 0; i < pal + 1; i++)
    {
        matrix.setLED(i, 0xffff00);
    }
    matrix.update();
}
