#include "FastLED.h"
//Horizontal, Pixel ORder Top right, snake
//Modify this to match your matrix
#define WIDTH 18
#define HEIGHT 29
#define DATA_PIN 6

const int NUM_LEDS = WIDTH * HEIGHT;

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  Serial.begin(115200);
  FastLED.setBrightness(10);
}

void loop() {}

void serialEvent() {
  switch (Serial.read()) //read header
  {
    case 0x05: //request for matrix definition
      Serial.println(WIDTH);
      Serial.println(HEIGHT);
      break;

    case 0x41: //frame data
      Serial.readBytes((char*)leds, NUM_LEDS * 3);
      FastLED.show();
      Serial.write(0x06); //acknowledge
      break;

    case 0x42:
      byte sDat[2] = {0};
      for (int i = 0; i < NUM_LEDS; i++) {
        Serial.readBytes(sDat, 2);
        leds[i].r = sDat[0] >> 3;
        leds[i].g = (sDat[0] & B00000111) << 3 | sDat[1] >> 5;
        leds[i].b = sDat[1] & B00011111;
      }
      FastLED.show();
      Serial.write(0x06); //acknowledge
      break;

    case 0x43:
      Serial.readBytes((char*)leds, NUM_LEDS * 3);
      FastLED.show();
      Serial.write(0x06); //acknowledge
      break;

  }
}