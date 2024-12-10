#pragma once
#include <Arduino.h>

#define PAL_AMOUNT 31

String getPaletteList();
uint32_t getPaletteColor(uint8_t pal, uint16_t idx, uint8_t bright);