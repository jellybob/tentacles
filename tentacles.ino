#include "FastLED.h"
#define NUM_LEDS 50

CRGB leds[NUM_LEDS];
int vector = 1;

// Blue is GND
const int yellowPin = 13;
const int greenPin = 11;

String command = "white";
int argCount = 0;
uint8_t args[99];

void setup() {
  Serial.begin(115200);
  
  FastLED.addLeds<WS2801, yellowPin, greenPin>(leds, NUM_LEDS);
  setColor(CRGB::Black);
}

void readValuesFromSerial() {
  command = "";
  int commandRead = 0;
  int argCursor = 0;
  
  while(Serial.available() > 0) {
    if (commandRead == 0) {
      command = Serial.readStringUntil(',');
      commandRead = 1;
    } else {
      if (Serial.peek() != '\n') {
        args[argCursor] = Serial.parseInt();
        argCursor += 1;
      } else {
        Serial.read();
        argCount = argCursor;
        dumpValuesToSerial();
      }
    }
  }
}

void dumpValuesToSerial() {
  Serial.print("Command: ");
  Serial.print(command);
  
  for (int i = 0; i < argCount; i++) {
    Serial.print(", Arg");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(args[i]); 
    
    i += 1;
  }

  Serial.print('\n');  
}

void loop() {
  if (Serial.available() > 0) {
    readValuesFromSerial();
  }
  
  int delayTime = 10;
  if (command == "off") {
    fadeTo(CRGB::Black);
  } else if (command == "white") {
    setColor(CRGB::White);
  } else if (command == "red") {
    setColor(CRGB::Red);
  } else if (command == "green") {
    setColor(CRGB::Green);    
  } else if (command == "blue") {
    setColor(CRGB::Blue);
  } else if (command == "rgb") {
    setColor(CRGB(args[0], args[1], args[2]));
  } else if (command == "hsv") {
    setColor(CHSV(args[0], args[1], args[2]));    
  } else if (command == "rainbow") {
    rainbow();
  } else if (command == "snake") {
    delayTime = 50;
    snake();
  } else {
    boolean red = false;
    
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] =  red ? CRGB::Red : CRGB::Yellow;
      red = !red;
    }
    FastLED.show();
  }
  
  delay(delayTime);
}

void setColor(const struct CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void fadeTo(const struct CRGB color) {
  static struct CRGB targetColor;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    if (leds[i].red > targetColor.red) {
      leds[i].red -= 1;
    } else if (leds[i].red < targetColor.red) {
      leds[i].red += 1;
    }
    
    if (leds[i].blue > targetColor.blue) {
      leds[i].blue -= 1;
    } else if (leds[i].blue < targetColor.blue) {
      leds[i].blue += 1;
    }
    
    if (leds[i].green > targetColor.green) {
      leds[i].green -= 1;
    } else if (leds[i].green < targetColor.green) {
      leds[i].green += 1;
    }
  }
  FastLED.show();
}

void rainbow() {
  static uint8_t rainbowHue = 0;
 
  setColor(CHSV(rainbowHue, 255, 255));
  rainbowHue += 1;
}

void snake() {
  static int snakeStart = 5;
  static int snakeEnd = 0;
  
  snakeStart += 1;
  snakeEnd += 1;
  if (snakeStart > NUM_LEDS) {
    snakeStart = 5;
    snakeEnd = 0;
  }
  
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i < snakeStart && i > snakeEnd) {
      leds[i] = CRGB::White;
    } else {
      leds[i] = CRGB::Black;
    }
  }
  
  FastLED.show();
}
