#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define MATRIX_PIN 1
#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 16

#define BUTTON_PIN 2

#define MAX_COLOR_MODES 5

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
  MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

uint8_t _t = 0;
uint8_t _mode = 0;

void setup() {
  matrix.begin();
  matrix.setBrightness(10);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) {
    _mode = ++_mode % MAX_COLOR_MODES;
  }

  int meter0Reading = analogRead(A0);
  int brightness = min(meter0Reading / 20, 40);
  matrix.setBrightness(brightness);

  int meter1Reading = analogRead(A1);
  int v = min(meter1Reading / 50, 20);  

  matrix.fillScreen(0);
  gradient(_t, v);
  matrix.show();  
  delay(100);  

  _t = ++_t % 256;
}


uint16_t rainbow(uint8_t x, uint8_t y, uint8_t v) {
  return  matrix.Color(255 - y * 12, 48 + y * 12, 48 + x * 12);
}

uint16_t rainbowVariable(uint8_t x, uint8_t y, uint8_t v) {
  return matrix.Color(255 - y * v, (v * 4) + y * v, (v * 4) + x * v);
}

uint16_t redBlue(uint8_t x, uint8_t y, uint8_t v) {
  return matrix.Color(255 - y * v, 0, 255 - x * v);
}

uint16_t greenBlue(uint8_t x, uint8_t y, uint8_t v) {
  return matrix.Color(0, 255 - y * v, 255 - x * v);
}

uint16_t cyanPink(uint8_t x, uint8_t y, uint8_t v) {
  return matrix.Color(255 - y * v, 96, 96);
}


void gradient(uint8_t t, uint8_t v) {
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {  
      switch (_mode) {
        case 0:
          matrix.drawPixel(x, y, rainbow(x, y, v));
          break;          
        case 1:
          matrix.drawPixel(x, y, rainbowVariable(x, y, v));
          break;
        case 2:
          matrix.drawPixel(x, y, cyanPink(x, y, v));
          break;
        case 3:
          matrix.drawPixel(x, y, greenBlue(x, y,v ));
          break;
        case 4:
          matrix.drawPixel(x, y, redBlue(x, y,v ));
          break;
      }
    }
  }
}
