//----------------------------------------------------------------
//-- Otto ALARM
//-- This code allows Otto to detect intruders with the ultrasound and alert with the buzzer
//-- April 2019: Designed to work with the basic Otto but could be compatible with PLUS or Humanoid or other biped robots
/****************************************************************************************************** 
  Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
  Otto DIY invests time and resources providing open source code and hardware, 
  please support by purchasing kits from (https://www.ottodiy.com)
  
  BSD license, all text above must be included in any redistribution
 *******************************************************************/
//-----------------------------------------------------------------
#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <US.h>
#include <Otto9.h>
Otto9 Otto;  //This is Otto!!
//---------------------------------------------------------
//-- Make sure the servos are in the right pin
/*             -------- 
              |  O  O  |
              |--------|
  RIGHT LEG 3 |        | LEFT LEG 2
               -------- 
               ||     ||
RIGHT FOOT 5 |---     ---| LEFT FOOT 4     
*/
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
//-- Movement parameters
int T = 1000;            //Initial duration of movement
int moveId = 0;          //Number of movement
int moveSize = 15;       //Asociated with the height of some movements
//---------------------------------------------------------
bool alarmActivated = false;
int initDistance = 300;
unsigned long previousMillis = 0;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(57600); //Serial communication initialization
    Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  //Uncomment this to set the servo trims manually and save on EEPROM
  //Otto.setTrims(TRIM_YL, TRIM_YR, TRIM_RL, TRIM_RR);
  //Otto.saveTrimsOnEEPROM(); //Uncomment this only for one upload when you finaly set the trims.
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
  delay(50);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {      
        if (alarmActivated == false) {  //Arming alarm system
          Otto.ascendingTurn(2, 1000, 50);
          OttoArmingAlarmSystem();
 }
 else {
  delay(100);
          //ALARM!!!!
          if (Otto.getDistance() < initDistance) {
            delay(50);
              Otto.bendTones (note_A5, note_A7, 1.04, 5, 2);  //A5 = 880 , A7 = 3520
              delay(20);
              Otto.bendTones (note_A7, note_A5, 1.02, 5, 2);  //A5 = 880 , A7 = 3520
              delay(100);
         }
 }
}
void OttoArmingAlarmSystem() {

  int countDown = 10000; //10 sec
  while (countDown > 0) {
    countDown -= 1000;
    Otto._tone(note_A7, 50, 0); //bip'
    delay(300);
    delay(650);
  }
    alarmActivated = true;
    delay(100);
    initDistance = Otto.getDistance();
    delay(100);
    initDistance -= 10;
    previousMillis = millis();
}
