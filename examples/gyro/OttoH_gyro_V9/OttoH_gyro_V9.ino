#include <Servo.h>
#include <Otto9.h>
#include "Gyro.h"
Otto9 Otto;  // This is Otto!
Gyro gyro;
//---------------------------------------------------------
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)
#define TOUCH_PIN A0
#define NoiseSensor_PIN A6
int touchVal = 0;
int noiseVal = 0;

unsigned long int rock_symbol =    0b00000000001100011110011110001100;
unsigned long int paper_symbol =   0b00011110010010010010010010011110;
unsigned long int scissors_symbol = 0b00000010010100001000010100000010;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(38400); //setup your bluetooth module to match this baudrate (or change it here)
  gyro.begin();
  pinMode(TOUCH_PIN, INPUT);
Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
  Otto.matrixIntensity(1);// set up Matrix display intensity
  Otto.sing(S_connection); // Otto wake up!
  Otto.home();
  delay(50);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  gyro.update();
  Serial.read();
    Serial.print("X:");
    Serial.print(gyro.getAngleX());
    Serial.print(" Y:");
    Serial.print(gyro.getAngleY());
    Serial.print(" Z:");
    Serial.println(gyro.getAngleZ());
  delay(10);

  if ((abs(gyro.getAngleX()) > -18 && abs(gyro.getAngleX() < 0)) && (abs(gyro.getAngleY()) > 0 && abs(gyro.getAngleY() < 10)))
  { 
    Otto.putMouth(xMouth);
  }

    if((abs(gyro.getAngleX() > 0) && abs(gyro.getAngleX() < 18)) ||  (abs(gyro.getAngleY() > -10) && abs(gyro.getAngleY() < 0)))
    {
    Otto.putMouth(diagonal);
     }

  // test touch sensor
  touchVal = digitalRead(TOUCH_PIN);  //  touch sensor
  if (touchVal == HIGH) {
    Otto.playGesture(OttoLove);
    delay(10);
    touchVal = !touchVal;
  }

  // test noise sensor
  noiseVal = Otto.getNoise();
  delay(50);
      Serial.print("sound sensor:");
  Serial.println(noiseVal);
  if (noiseVal > 30) {
    Otto.home();
    Otto.putAnimationMouth(littleUuh, 0);
    Otto.putMouth(culito);
    Otto.bendTones (100, 200, 1.04, 10, 10);
    delay (100);
    Otto.putAnimationMouth(dreamMouth, 0);
    delay (100);
    Otto.putMouth(okMouth);     
    delay (100);
    Otto.putMouth(rock_symbol, 0);
    delay (100);
    Otto.putMouth(scissors_symbol, 0);
    delay (100);
    Otto.putMouth(paper_symbol, 0);
    delay (100);
    noiseVal = 0;
  }
}
