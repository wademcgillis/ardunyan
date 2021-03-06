// PLEASE DO NOT SUE ME MR. NYAN CAT MAN.
#define GRAYSCALE_AND_RAINBOW_ENABLED 1 // flag to disable or enable grayscale & rainbow at compiler time. Parts of screen will rapidly flicker black and white to simulate gray.

#include <Arduboy.h>
#include <audio/audio.h>
#include <core/core.h>

#include <EEPROM.h>
#include <SPI.h>

Arduboy arduboy;

void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(90);
}

// colors of the rainbow rr gg bb
// Bit pairs get multiplied by 127.
// The second bit-pair trio in the array below is 10 01 00.
// 2 red, 1 green, 0 blue, which ends up being RGB 254, 127, 00.
// Orange.
char rainbow[] = {
    0b100000,
    0b100100,
    0b101000,
    0b001000,
    0b000010,
    0b010010
  };

// bitmap data for the background sparkles. typed this by hand
PROGMEM const uint8_t sparkle[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00001000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,
    0b00000000,
    0b00001000,
    0b00010100,
    0b00001000,
    0b00000000,
    0b00000000,
    0b00000000,
    
    0b00000000,
    0b00001000,
    0b00001000,
    0b00110110,
    0b00001000,
    0b00001000,
    0b00000000,
    0b00000000,
    
    0b00001000,
    0b00001000,
    0b00000000,
    0b01101011,
    0b00000000,
    0b00001000,
    0b00001000,
    0b00000000,
    
    0b00001000,
    0b00100010,
    0b00000000,
    0b01000001,
    0b00000000,
    0b00100010,
    0b00001000,
    0b00000000,
    
    0b00001000,
    0b00000000,
    0b00000000,
    0b01000001,
    0b00000000,
    0b00000000,
    0b00001000,
    0b00000000
  };

// There was no way I was going to convert nyan cat by hand
// So I wrote a converter and used that.
PROGMEM const uint8_t nyan_png_data[] = {
0x00,0x80,0x80,0x80,0x80,0x00,0x00,0xFC,0x02,0xF1,0xF9,0xED,0xFD,0xFD,0xFD,0x7D,
0xF5,0xFD,0xFD,0xF5,0xFD,0xFD,0xFD,0xDD,0xF9,0xF1,0x02,0xFC,0xC0,0xE0,0xE0,0xC0,
0x00,0x00,0x00,0x03,0x07,0x0F,0x1F,0x1F,0x3E,0xFF,0x00,0x3F,0x77,0xBF,0xFD,0xFF,
0xDF,0xFF,0xFB,0xFF,0xFF,0xFF,0xFF,0xEF,0xE7,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xE7,
0xFF,0xFF,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0F,0x0F,0x0F,0x06,0x02,0x0E,
0x0E,0x0E,0x06,0x02,0x02,0x02,0x02,0x03,0x07,0x0F,0x0F,0x0F,0x03,0x07,0x0F,0x0F,
0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x02,0xF1,0xF9,
0xED,0xFD,0xFD,0xFD,0x7D,0xF5,0xFD,0xFD,0xF5,0xFD,0xFD,0xFD,0xDD,0xF9,0xF1,0x02,
0xFC,0x80,0xC0,0xE0,0xE0,0xC0,0x00,0x00,0x06,0x0F,0x1F,0x1E,0x3C,0x3C,0xFF,0x00,
0x3F,0x77,0xBF,0xFD,0xFF,0xDF,0xFF,0xFB,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xE7,0xFF,
0xFF,0xFF,0xFF,0xFF,0xEF,0xE7,0xFF,0xFF,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x06,
0x0F,0x0F,0x06,0x02,0x06,0x0E,0x0E,0x0E,0x02,0x02,0x02,0x02,0x02,0x03,0x07,0x0F,
0x0F,0x0F,0x03,0x07,0x0F,0x0F,0x0F,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xF8,0x04,0xE2,0xF2,0xDA,0xFA,0xFA,0xFA,0xFA,0xEA,0xFA,0xFA,0xEA,
0xFA,0xFA,0xFA,0xBA,0xF2,0xE2,0x04,0xF8,0x00,0x80,0xC0,0xC0,0x80,0x00,0x00,0x60,
0xE0,0xF0,0xF0,0xF0,0x78,0xFF,0x00,0x7F,0xEF,0x7F,0xFB,0xFF,0xBF,0xFE,0xF7,0xFF,
0xFF,0xFF,0xFF,0xFF,0xDF,0xCF,0xFF,0xFF,0xFE,0xFF,0xFF,0xDF,0xCF,0xFF,0xFF,0xF8,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x1E,0x1C,0x0C,0x05,0x0D,0x1D,0x1D,0x1D,
0x05,0x05,0x05,0x05,0x07,0x07,0x0F,0x1F,0x1F,0x1F,0x07,0x0F,0x1F,0x1F,0x1F,0x03,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0xE2,0xF2,0xDA,0xFA,0xFA,
0xFA,0xFA,0xEA,0xFA,0xFA,0xEA,0xFA,0xFA,0xFA,0xBA,0xF2,0xE2,0x04,0xF8,0x00,0x80,
0xC0,0xC0,0x80,0x00,0x00,0xC0,0xE0,0xF0,0xF0,0x78,0x78,0xFF,0x00,0x7F,0xEF,0x7F,
0xFB,0xFF,0xBF,0xFE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xDF,0xCF,0xFF,0xFF,0xFE,0xFF,
0xFF,0xDF,0xCF,0xFF,0xFF,0xF8,0x00,0x00,0x01,0x01,0x00,0x00,0x0C,0x1F,0x1E,0x0C,
0x04,0x0D,0x1D,0x1D,0x1D,0x05,0x05,0x05,0x05,0x05,0x07,0x0F,0x1F,0x1F,0x1F,0x07,
0x0F,0x1F,0x1F,0x1F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,
0x04,0xE2,0xF2,0xDA,0xFA,0xFA,0xFA,0xFA,0xEA,0xFA,0xFA,0xEA,0xFA,0xFA,0xFA,0xBA,
0xF2,0xE2,0x04,0xF8,0x80,0xC0,0xC0,0x80,0x00,0x00,0x0C,0x0E,0x1E,0x1E,0x1E,0x3C,
0x3C,0xFF,0x00,0x7F,0xEF,0x7F,0xFB,0xFF,0xBF,0xFE,0xF7,0xFF,0xFF,0xFF,0xFF,0xDF,
0xCF,0xFF,0xFF,0xFF,0xFE,0xFF,0xDF,0xCF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,
0x0C,0x1E,0x1F,0x0F,0x06,0x0C,0x1C,0x1D,0x1D,0x05,0x05,0x05,0x05,0x05,0x05,0x0F,
0x1F,0x1F,0x1F,0x07,0x0F,0x1F,0x1F,0x1F,0x07,0x07,0x03,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xF8,0x04,0xE2,0xF2,0xDA,0xFA,0xFA,0xFA,0xFA,0xEA,0xFA,
0xFA,0xEA,0xFA,0xFA,0xFA,0xBA,0xF2,0xE2,0x04,0xF8,0xC0,0xE0,0xE0,0xC0,0x00,0x00,
0x00,0x06,0x0F,0x1F,0x1E,0x3C,0x3C,0xFF,0x00,0x7F,0xEF,0x7F,0xFB,0xFF,0xBF,0xFE,
0xF7,0xFF,0xFF,0xFF,0xFF,0xEF,0xE7,0xFF,0xFF,0xFF,0xFF,0xFF,0xEF,0xE7,0xFF,0xFF,
0x7C,0x00,0x00,0x00,0x00,0x00,0x1C,0x1E,0x1F,0x0F,0x06,0x1C,0x1C,0x1D,0x0D,0x05,
0x05,0x05,0x05,0x05,0x05,0x1D,0x1F,0x1F,0x0F,0x07,0x0F,0x1F,0x1F,0x1F,0x03,0x03,
0x01,0x00,0x00,0x00
};

PROGMEM const uint8_t nyan2_png_data[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x0E,0x06,0x12,0x02,0x02,0x02,0x82,
0x0A,0x02,0x02,0xCA,0xC2,0x82,0x02,0x22,0x06,0x0E,0xFC,0x00,0x80,0xC0,0xC0,0x00,
0x00,0x00,0x00,0x00,0x01,0x03,0x06,0x0C,0x08,0x00,0xFF,0xC0,0x88,0x40,0x02,0x00,
0x20,0x00,0x04,0x00,0x7C,0x9F,0x9F,0xF7,0x3F,0x7E,0x7E,0x3E,0x6E,0x7F,0x37,0xFF,
0x9F,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x02,0x01,0x01,0x01,
0x05,0x05,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x05,0x05,0x01,0x01,0x01,0x05,0x05,
0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x0E,0x06,
0x12,0x02,0x02,0x02,0x82,0x0A,0x02,0x02,0x0A,0xC2,0xC2,0x82,0x22,0x06,0x0E,0xFC,
0x00,0x00,0x80,0xC0,0xC0,0x00,0x00,0x00,0x00,0x06,0x0E,0x08,0x18,0x18,0x00,0xFF,
0xC0,0x88,0x40,0x02,0x00,0x20,0x00,0x04,0x00,0x00,0x7C,0x9F,0x9F,0xF7,0x3F,0x7E,
0x7E,0x3E,0x6E,0x7F,0x37,0xFF,0x9F,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x06,0x06,0x01,0x01,0x01,0x05,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x05,
0x05,0x01,0x01,0x01,0x05,0x05,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xF8,0x1C,0x0C,0x24,0x04,0x04,0x04,0x04,0x14,0x04,0x04,0x14,
0x84,0x84,0x04,0x44,0x0C,0x1C,0xF8,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,
0x40,0x60,0x60,0x20,0x20,0x00,0xFF,0x80,0x10,0x80,0x04,0x00,0x40,0x01,0x08,0x00,
0x00,0xF8,0x3F,0x3F,0xEF,0x7E,0xFC,0xFC,0x7D,0xDC,0xFE,0x6F,0xFF,0x3F,0x38,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0D,0x0B,0x03,0x02,0x02,0x0A,0x0A,0x02,
0x02,0x02,0x02,0x02,0x01,0x03,0x03,0x0A,0x0A,0x02,0x02,0x02,0x0A,0x0A,0x03,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1C,0x0C,0x24,0x04,0x04,
0x04,0x04,0x14,0x04,0x04,0x14,0x84,0x84,0x04,0x44,0x0C,0x1C,0xF8,0x00,0x00,0x00,
0x80,0x80,0x00,0x00,0x00,0x00,0xC0,0xE0,0x20,0x30,0x30,0x00,0xFF,0x80,0x10,0x80,
0x04,0x00,0x40,0x01,0x08,0x00,0x00,0xF8,0x3F,0x3F,0xEF,0x7E,0xFC,0xFC,0x7D,0xDC,
0xFE,0x6F,0xFF,0x3F,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0D,0x03,
0x03,0x02,0x0A,0x0A,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x03,0x0B,0x0A,0x02,0x02,
0x02,0x0A,0x0A,0x02,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xF8,0x1C,0x0C,0x24,0x04,0x04,0x04,0x04,0x14,0x04,0x04,0x94,0x84,0x04,0x04,0x44,
0x0C,0x1C,0xF8,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x04,0x0C,0x0C,0x08,0x08,
0x10,0x00,0xFF,0x80,0x10,0x80,0x04,0x00,0x40,0x01,0x08,0x00,0xF8,0x3F,0x3F,0xEF,
0x7E,0xFC,0xFC,0x7C,0xDD,0xFE,0x6F,0xFF,0x3F,0x38,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0C,0x0C,0x04,0x01,0x03,0x0B,0x0A,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,
0x0B,0x0B,0x02,0x02,0x02,0x0A,0x0A,0x02,0x02,0x03,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1C,0x0C,0x24,0x04,0x04,0x04,0x04,0x14,0x04,
0x04,0xD4,0xC4,0x84,0x04,0x44,0x0C,0x1C,0xF8,0x00,0x80,0xC0,0xC0,0x00,0x00,0x00,
0x00,0x00,0x06,0x0E,0x08,0x18,0x18,0x00,0xFF,0x80,0x10,0x80,0x04,0x00,0x40,0x01,
0x08,0x00,0x7C,0x9F,0x9F,0xF7,0x3F,0x7E,0x7E,0x3E,0x6E,0x7F,0x37,0xFF,0x9F,0x1C,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0E,0x04,0x01,0x03,0x0B,0x0A,0x02,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x09,0x09,0x01,0x01,0x01,0x09,0x0D,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x0E,0x06,0x12,
0x02,0x02,0x02,0x82,0x0A,0x02,0x02,0x0A,0x02,0x02,0x02,0x22,0x06,0x0E,0xFC,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xC0,
0x88,0x40,0x02,0x00,0x20,0x00,0x04,0x00,0x00,0x00,0x00,0x10,0x18,0x00,0x00,0x00,
0x00,0x00,0x10,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFC,0x0E,0x06,0x12,0x02,0x02,0x02,0x82,0x0A,0x02,0x02,0x0A,0x02,0x02,0x02,
0x22,0x06,0x0E,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xFF,0xC0,0x88,0x40,0x02,0x00,0x20,0x00,0x04,0x00,0x00,0x00,0x00,
0x00,0x10,0x18,0x00,0x00,0x00,0x00,0x00,0x10,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1C,0x0C,0x24,0x04,0x04,0x04,0x04,
0x14,0x04,0x04,0x14,0x04,0x04,0x04,0x44,0x0C,0x1C,0xF8,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x80,0x10,0x80,0x04,0x00,
0x40,0x01,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x30,0x00,0x00,0x01,0x00,0x00,0x20,
0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x02,
0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1C,
0x0C,0x24,0x04,0x04,0x04,0x04,0x14,0x04,0x04,0x14,0x04,0x04,0x04,0x44,0x0C,0x1C,
0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0x80,0x10,0x80,0x04,0x00,0x40,0x01,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x30,
0x00,0x00,0x01,0x00,0x00,0x20,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x01,0x03,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xF8,0x1C,0x0C,0x24,0x04,0x04,0x04,0x04,0x14,0x04,0x04,0x14,
0x04,0x04,0x04,0x44,0x0C,0x1C,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x80,0x10,0x80,0x04,0x00,0x40,0x01,0x08,0x00,
0x00,0x00,0x00,0x20,0x30,0x00,0x00,0x00,0x01,0x00,0x20,0x30,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x02,0x02,0x02,0x02,0x02,
0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1C,0x0C,0x24,0x04,0x04,
0x04,0x04,0x14,0x04,0x04,0x14,0x04,0x04,0x04,0x44,0x0C,0x1C,0xF8,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x80,0x10,0x80,
0x04,0x00,0x40,0x01,0x08,0x00,0x00,0x00,0x00,0x10,0x18,0x00,0x00,0x00,0x00,0x00,
0x10,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,
0x03,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// did the same for the rainbow data.
PROGMEM const uint8_t rainbow_png_data[] = {
0x8E,0x8E,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0x8E,0x8E,0x8E,0x8E,0x8E,0x8E,
0xE3,0xE3,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71,0xE3,0xE3,0xE3,0xE3,0xE3,0xE3,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xC7,0x8E,0x8E,0x8E,0x8E,0x8E,0x8E,0x8E,0x8E,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,
0x71,0xE3,0xE3,0xE3,0xE3,0xE3,0xE3,0xE3,0xE3,0x71,0x71,0x71,0x71,0x71,0x71,0x71,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x70,0x70,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x70,0x70,0x70,0x70,0x70,0x70,
0x1C,0x1C,0x8E,0x8E,0x8E,0x8E,0x8E,0x8E,0x8E,0x8E,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,
0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,
0x38,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x38,0x38,0x38,0x38,0x38,0x38,0x38,
0x8E,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x8E,0x8E,0x8E,0x8E,0x8E,0x8E,0x8E,
0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x03,0x03
};

// Ticker stuff leftover from the start. Still used, but really shouldn't be.
// I was in a rush ;)
#define frame(i) ((((i))/8)%12)

// convert a bit-pair to its full value
#define rainbowcolor(i,j) (0x7F*((rainbow[i] >> (j))&3))


// Sparkle starting positions.
// I resized the original nyan cat gif to 64 and used the positions of the sparkles.
// Then I doubled the number of sparkles and shifted them 64 pixels to the right
// because the arduboy width is 2x height.
int16_t sparklesX[] = { 4, 33, 43,  63, 31, 2,
                        64+4, 64+33, 64+43,  64+63, 64+31, 64+2};
char sparklesY[] = {1,  10, 20, 41, 51, 61, 1,  10, 20, 41, 51, 61};
// Starting frames for each sparkle.
char sparklesF[] = {1, 3, 1, 5, 5, 2, 1, 3, 1, 5, 5, 2};

// start the cat centered in the screen
char catX = 47;
char catY = 20;

void loop()
{
  if (!(arduboy.nextFrame()))
    return;
  static uint16_t i = 0;
  // ANNOYING RAINBOW. As soon as I got the led rainbow to work
  // I disabled it until I recorded the video because it is grating.
#if GRAYSCALE_AND_RAINBOW_ENABLED
  arduboy.setRGBled(rainbowcolor(frame(i)/2,4),rainbowcolor(frame(i)/2,2),rainbowcolor(frame(i)/2,0));//2 0 0
#endif

  // clear the screen
  arduboy.clear();

 // move cat
 if (arduboy.pressed(LEFT_BUTTON))
    if (catX > 1)
      catX -= 1;
  if (arduboy.pressed(RIGHT_BUTTON))
    if (catX < 128-34)
      catX += 1;  

  if (arduboy.pressed(UP_BUTTON))
    if (catY > 1)
      catY -= 1;
  if (arduboy.pressed(DOWN_BUTTON))
    if (catY < 64-24)
      catY += 1;  

#if GRAYSCALE_AND_RAINBOW_ENABLED
  // draw "rainbow"
  for(char k=catX-6;k+16 > -16; k -= 16)
    arduboy.drawBitmap(k,catY + 1,rainbow_png_data + (16*3)*((frame(i)/5)%2) + (16*3*2*(i%2)),16,24,WHITE);
#endif  

  // draw sparkles
  for(char j=0;j<12;j++)
  {
    sparklesX[j] -= 1;/*-8*frame(i)));*/
    if (sparklesX[j] < -8)
      sparklesX[j] += 128;
    arduboy.drawBitmap(sparklesX[j],sparklesY[j]-3,sparkle + 8*((
      (frame(i)/2) + sparklesF[j]
      
    )%6),8,8,WHITE);
  }

  arduboy.drawBitmap(catX,catY,nyan_png_data + (34*3)*(frame(i)/2),34,24,WHITE);
  // draw cat
  arduboy.drawBitmap(catX,catY,nyan2_png_data + (34*3)*(frame(i)/2)
#if GRAYSCALE_AND_RAINBOW_ENABLED  
  + (34*3*6*(i%2))
#endif
  ,34,24,BLACK);
    
  // push to screen
  arduboy.display();

  // increase ticker
  i++;
  if (i == 192*10)
    i = 0;
}