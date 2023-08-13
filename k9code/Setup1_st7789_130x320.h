#ifndef SETUP1_ST7789_130X320_H
#define SETUP1_ST7789_130X320_H

#include <TFT_eSPI.h>

#define TFT_WIDTH  172
#define TFT_HEIGHT 320

#define TFT_MOSI_PIN  35
#define TFT_SCLK_PIN  36
#define TFT_CS_PIN    39
#define TFT_DC_PIN    40
#define TFT_RST_PIN   41
#define TFT_BL_PIN    15 // Backlight control pin
#define TFT_PWR_PIN   16 // Power control pin

TFT_eSPI tft = TFT_eSPI();

#endif // SETUP1_ST7789_130X320_H
