// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)
#include <Adafruit_NeoPixel.h>
#include "main.h"
#include "config.h"
#include "redraw.h"
#include "matrix.h"
#include <RunningGFX.h>
#include "palettes.h"

// void palletteAdd()
// {
//   String str = Serial.readString();
//   if (str.equals("+"))
//   {
//     CHOOSEN_PALLETTE++;
//     Serial.printf("Pallette changed to %d\n\r", CHOOSEN_PALLETTE);
//   }
// }

void setup()
{
  // Serial.begin(115200);

  // Serial.onReceive(palletteAdd);
  matrix.setModeXY();
  matrix.begin();
}

uint16_t lastPalletteSwithMS = 0;

void loop()
{
  if (lastPalletteSwithMS >= 10000)
  {
    lastPalletteSwithMS = 0;
    CHOOSEN_PALLETTE = (CHOOSEN_PALLETTE + 1) % PAL_AMOUNT;
  }
  drawBack();
  drawChoosenPallette(CHOOSEN_PALLETTE);
  lastPalletteSwithMS += 50;
  delay(50);
}
