#include "redraw.h"
#include "matrix.h"
#include "palettes.h"
#include "config.h"
#include "settings.h"
#include <Looper.h>
#include <FastLED.h>

void drawBack()
{
    uint32_t count = (millis() / 64) * db[kk::back_speed].toInt();
    uint8_t pal = db[kk::back_pal];
    uint8_t scale = db[kk::back_scale];
    uint8_t bright = db[kk::back_bright];

    matrix.setModeXY();
    matrix.setBright(bright);

    for (int y = 0; y < matrix.height(); y++)
    {
        for (int x = 0; x < matrix.width(); x++)
        {
            if (matrix.ledXY(x, y) == NM)
                continue;
            uint32_t col = getPaletteColor(pal, inoise16(x * scale * 64, y * scale * 64, count * 32), bright);
            matrix.setLED(x, y, col);
        }
    }
    matrix.update();
}

// void drawChoosenPallette(uint16_t pal)
// {
//     for (int i = 0; i < pal + 1; i++)
//     {
//         matrix.setLED(i, 0xffff00);
//     }
//     matrix.update();
// }

LP_TIMER_("redraw", 50, []()
          { drawBack(); });