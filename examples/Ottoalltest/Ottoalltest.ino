#include <Otto9Humanoid.h>
Otto9Humanoid Otto;
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16matrix ematrix = Adafruit_8x16matrix();
#include <US.h>

int i;
int j;
const char data[] = "VARIABLE#";
unsigned long int matrix;
int distance;
bool obstacleDetected = false;
volatile bool buttonPushed = false;
bool estado = false;
int moveSize = 20;

#define PIN_YL 2 // left leg, servo[0]
#define PIN_YR 3 // right leg, servo[1]
#define PIN_RL 4 // left foot, servo[2]
#define PIN_RR 5 // right foot, servo[3]
#define PIN_LA 6 //servo[4]  Left arm
#define PIN_RA 7 //servo[5]  Right arm
#define PIN_Trigger 8 // ultrasound
#define PIN_Echo 9 // ultrasound
#define PIN_NoiseSensor A6
#define PIN_Buzzer  13 //buzzer
#define DIN_PIN A3
#define CS_PIN A2
#define CLK_PIN A1
#define LED_DIRECTION 1
static const uint8_t PROGMEM
logo_bmp[] = {  B01111110,B10000001,B10111001,B10101001,B10111001,B10010001,B10111001,B10010001,B10010001,B10111001,B10010001,B10111001,B10101001,B10111001,B10000001,B01111110},
happy_bmp[] = {  B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000,B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000},
eyes_bmp[] = {  B00000000,B00111100,B01000010,B01001010,B01000010,B01000010,B00111100,B00000000,B00000000,B00111100,B01000010,B01001010,B01000010,B01000010,B00111100,B00000000},
sad_bmp[] =  {  B00000000,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000,B00000000,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000},
xx_bmp[] =  {  B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000},
XX_bmp[] = {  B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001,B00000000,B00000000,B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001},
angry_bmp[] = {  B00000000,B00011110,B00111100,B01111000,B01110000,B00100000,B00000000,B00000000,B00000000,B00000000,B00100000,B01110000,B01111000,B00111100,B00011110,B00000000},
angry2_bmp[] = {  B00000000,B00000010,B00000100,B00001000,B00010000,B00100000,B00000000,B00000000,B00000000,B00000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000000},
sleep_bmp[] = {  B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000},
freetful_bmp[] = {  B00000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000000,B00000000,B00000000,B00000000,B00000010,B00000100,B00001000,B00010000,B00100000,B00000000},
love_bmp[] = {  B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000,B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000},
confused_bmp[] = {  B00000000,B01111100,B10000010,B10111010,B10101010,B10001010,B01111000,B00000000,B00000000,B01111100,B10000010,B10111010,B10101010,B10001010,B01111000,B00000000},
wave_bmp[] = {  B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000,B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000},
magic_bmp[] = {  B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000},
fail_bmp[] = {  B00000000,B00110000,B01111000,B01111000,B01111100,B00111100,B00001000,B00000000,B00000000,B00001000,B00111100,B01111100,B01111000,B01111000,B00110000,B00000000},
full_bmp[] =  {   B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111 };
#define PIN_Trigger 8 // ultrasound
#define PIN_Echo 9 // ultrasound
#define PIN_Button A0
#define PIN_NoiseSensor A6
#define DIN_PIN A3
#define CS_PIN A2
#define CLK_PIN A1
#define LED_DIRECTION 1

void do2() {
  Otto.clearMouth();
  Otto.putMouth(okMouth);
  Otto.swing(1, 1000, 25);
  Otto.flapping(1, 1000, 25, 1);
  Otto.handsup();
  Otto.sing(S_disconnection);
}


void setup() {
  Serial.begin(9600);
  Otto.initHUMANOID(PIN_YL, PIN_YR, PIN_RL, PIN_RR, PIN_LA, PIN_RA, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);
  ematrix.begin(0x70);  // pass in the address
  pinMode( 13 , OUTPUT);
    Otto.home();
  Otto.sing(S_connection);
  for (i=0 ; i<=7 ; i=i+1) {
    for (j=0 ; j<=15 ; j=j+1) {
      Otto.setLed(i,j,1);
      ematrix.drawPixel(i,j, LED_ON);
      ematrix.writeDisplay();
      delay(10);
      delay(30);
    }
  }
   ematrix.clear();
  ematrix.drawBitmap(0, 0, + logo_bmp , 8, 16, LED_ON);
  ematrix.writeDisplay();
  delay(10);
  do2();

}

void loop() {
  // Begin serial & setup the communication speed.
  // After Check & Upload the code,
  // click on theSerial monitor button on the top right,
  // then click in the start button, to see the sensors measurements data.

    if ((Otto.getDistance() <10)) {
      tone( 13,261,125);
      delay(125);
      tone( 13,293,125);
      delay(125);
      tone( 13,329,125);
      delay(125);
      itoa(Otto.getDistance(), data, 10);// convert integer into a string so we can display this on the matrix
      Otto.clearMouth();
      Otto.writeText (data,50); // show the data with a fast scroll
      delay(50);tone( 13,329,125);
      delay(125);
    }
    if (digitalRead(PIN_Button)) {
       ematrix.clear();
       ematrix.clear();
      ematrix.drawBitmap(0, 0, + magic_bmp , 8, 16, LED_ON);
      ematrix.writeDisplay();
      delay(10);
      Otto.playGesture(OttoSuperHappy);
    }
    if (Otto.getNoise() > 700) {
       ematrix.clear();
      ematrix.drawBitmap(0, 0, + angry2_bmp , 8, 16, LED_ON);
      ematrix.writeDisplay();
      delay(10);
      tone( 13,880,250);
      delay(250);
      Otto.putMouth(smallSurprise);
    }
    if (Otto.getNoise() > 1000) {
       ematrix.clear();
      ematrix.drawBitmap(0, 0, + angry_bmp , 8, 16, LED_ON);
      ematrix.writeDisplay();
      delay(10);
      Otto.sing(S_fart1);
      Otto.putMouth(bigSurprise);
    }
    Serial.print("sound sensor reading:");
    Serial.print(Otto.getNoise());
    Serial.print(" ; ");
    Serial.print("ultrasound sensor reading:");
    Serial.print(Otto.getDistance());
    Serial.print("cm");
    Serial.print("\n");

}