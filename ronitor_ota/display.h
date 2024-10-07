#ifndef DISPLAY_H

#define DISPLAY_H
#include <SPI.h>
#include <DHT.h>              // include Adafruit DHT library code
#include <TFT_eSPI.h> // Graphics and font library for ST7739 driver chip

#include <WiFiUdp.h>
#include <NTPClient.h>        // include NTPClient library
#include <TimeLib.h>          // Include Arduino time library
#include <FlickerFreePrint.h>

// Include the jpeg decoder library
#include <TJpg_Decoder.h>

#include "moon.h"

// Include LittleFS
#include <FS.h>
#include "LittleFS.h"
#include "pitches.h"
void DontWorry();
void drawProgress(uint8_t percentage, String text) ;
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);
void PlaySound();
void RTC_display();
#endif

//#define LED_GPIO   35
#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  5000


#define DHTPIN      38           // DHT11 data pin is connected to NodeMCU pin D1 (GPIO5)
#define DHTTYPE     DHT11        // DHT11 sensor is used

#define BUZZERPIN   18
#define DISPLAY     4
#define BUTTON      8


#define SMALL_FONT    "fonts/RobotoCondensedBold28"
#define BIG_FONT      "fonts/BebasNeueRegular90"
#define MEDIUM_FONT   "fonts/BebasNeueRegular50"


#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

// Next is a special 16 bit colour value that encodes to 8 bits
// and will then decode back to the same 16 bit value.
// Convenient for 8 bit and 16 bit transparent sprites.
#define TFT_TRANSPARENT 0x0120 // This is actually a dark green
