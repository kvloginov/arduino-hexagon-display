#include "redraw.h"
#include "matrix.h"
#include "palettes.h"
#include "config.h"
#include "settings.h"
#include "brezline.h"
#include <Looper.h>
#include <FastLED.h>

struct GradData {
    GradData(int x0, int y0, int w, int h, int angle) {
        uint16_t hypot = sqrt(w * w + h * h) / 2;
        cx = x0 + w / 2;
        cy = y0 + h / 2;
        sx = cos(radians(angle)) * hypot;
        sy = sin(radians(angle)) * hypot;
        len = sqrt(sx * sx + sy * sy) * 2;
    }

    int16_t cx, cy;
    int16_t sx, sy;
    uint16_t i = 0, len;
    uint8_t pal;
    uint8_t bright;
    uint8_t scale;
    uint32_t count;
};

void drawBack()
{
    uint32_t count = (millis() / 64) * db[kk::back_speed].toInt();
    uint8_t pal = db[kk::back_pal];
    uint8_t scale = db[kk::back_scale];
    uint8_t bright = db[kk::back_bright];

    matrix.setModeXY();
    matrix.setBright(bright);

    switch (db[kk::back_mode].toInt())
    {
    // none
    case 0:
        matrix.clear();
        break;

    // grad
    case 1:
    {
        GradData data(0, 0, matrix.width(), matrix.height(), db[kk::back_angle]);
        data.pal = pal;
        data.bright = bright;
        data.count = count;
        data.scale = scale;

        brezLine(data.cx - data.sx, data.cy + data.sy, data.cx + data.sx, data.cy - data.sy, false, &data, [](int x, int y, void *vdata)
                 {
                GradData& data = *((GradData*)vdata);
                uint32_t color = getPaletteColor(data.pal, (uint32_t)data.i * (0xffff / 64) * data.scale / data.len + data.count * 2, data.bright);
                data.i++;

                brezLine(x - data.sy, y - data.sx, x + data.sy, y + data.sx, true, &color, [](int x, int y, void* color) {
                    matrix.setLED(x, y, *((uint32_t*)color));
                }); });
    }
    break;

    // perlin
    case 2:
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
        break;
    }
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
          { 
            drawBack(); 
          matrix.update(); });