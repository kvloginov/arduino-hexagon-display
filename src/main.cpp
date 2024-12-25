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
#include "config.h"
#include "redraw.h"
#include "matrix.h"
#include "palettes.h"
#include <WiFiConnector.h>
#include <Looper.h>
#include "settings.h"
#include <GyverNTP.h>
#include <RunningGFX.h>

void runString(String str) {
    RunningGFX run(matrix);
    matrix.clear();
    matrix.setModeDiag();
    run.setSpeed(10);
    run.setWindow(0, matrix.width(), 1);
    run.setColor24(0x003300);
    run.setFont(gfx_font_3x5);
    run.setText(str);
    run.start();

    while (run.tick() != RG_FINISH) {
        delay(10);
        yield();
    }
}

void setup()
{
  Serial.begin(115200);

  WiFiConnector.setName(PROJECT_NAME);

  WiFiConnector.onConnect([]()
                          {
      NTP.begin();
      Serial.print("Connected: ");
      Serial.println(WiFi.localIP());
      
      runString("Connected: " + WiFi.localIP().toString());
      });

  WiFiConnector.onError([]()
                        {
      Serial.print("Error! AP: ");
      Serial.println(WiFi.localIP()); 
      WiFiConnector.connect(""); // start our own AP

      runString("Started AP: " + WiFi.localIP().toString());
      });

  matrix.setModeXY();
  matrix.begin();
}

void loop()
{
  Looper.loop();
}