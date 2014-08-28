#include "FastLED.h"
#define NUM_LEDS 50

CRGB leds[NUM_LEDS];
int vector = 1;

uint8_t hue = 0;
uint8_t saturation = 0;
uint8_t value = 255;

void setup() {
  Serial.begin(9600);
  
  FastLED.addLeds<WS2801, 13, 11>(leds, NUM_LEDS);
  FastLED.showColor(CRGB::Black);
}

void readValuesFromSerial() {
  while(Serial.available() > 0) {
    uint8_t newHue = Serial.parseInt();
    uint8_t newSaturation = Serial.parseInt();
    uint8_t newValue = Serial.parseInt();
    
    if (Serial.read() == '\n') {
      hue = newHue;
      saturation = newSaturation;
      value = newValue;
      dumpValuesToSerial();
    }
  }
}

void dumpValuesToSerial() {
  Serial.print(hue, HEX);
  Serial.print(',');
  Serial.print(saturation, HEX);
  Serial.print(',');
  Serial.print(value, HEX);
  Serial.print('\n');  
}

void loop() {
  readValuesFromSerial();
  
  FastLED.showColor(CHSV(hue, saturation, value));
  delay(10);
}
