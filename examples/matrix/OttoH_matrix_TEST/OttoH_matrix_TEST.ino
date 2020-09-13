#include <LedControl.h>

const int DIN_PIN = A3;
const int CS_PIN = A2;
const int CLK_PIN = A1;
const int numDevices = 1;

const byte IMAGES[][8] = {
{B00000000,B01100110,B01100110,B00000000,B00000000,B00000000,B01111110,B00000000},
{B00000000, B10100101, B11100111, B00000000, B00000000, B00100100,B00000000},
{B00000000, B11100111, B00100001, B00000000, B00000000, B01000010,B00111100, B00000000},
{B00000000,B11100111,  B01000010,  B00000000,B00000000,  B01000010,  B00111100,  B00000000},
{B00000000,  B11100111, B10000100, B00000000, B00000000, B01000010, B00111100, B00000000},
{B00000000, B01100110, B01100110, B00000000, B00000000,B00111100, B01000010, B00000000},
{B01000010, B11100111, B01000010, B00000000,B00111100, B01000010, B00111100, B00000000},
{B00000000, B11000011, B01100110,B00100100, B00000000, B00111100, B01000010, B00000000}};
const int IMAGES_LEN = sizeof(IMAGES)/8;
LedControl matrix_led = LedControl(DIN_PIN, CLK_PIN, CS_PIN,numDevices);

void setup() {
  matrix_led.clearDisplay(0);
  matrix_led.shutdown(0, false);
  matrix_led.setIntensity(0, 1);
}

void displayImage(byte *image) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix_led.setLed(0, i, j, bitRead(image[i], 7-j));
    }
  }
}

int i = 0;

void loop() {
  displayImage(IMAGES[i]);
  if (++i >= IMAGES_LEN ) {
    i = 0;
  }
  delay(1000);
}
