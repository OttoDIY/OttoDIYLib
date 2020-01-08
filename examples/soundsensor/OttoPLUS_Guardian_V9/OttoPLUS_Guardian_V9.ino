//----------------------------------------------------------------
//-- Otto Guardian
//-- This is a code for Otto DIY starters,Otto will stay alert with the ultrasound & sound sensor for intruders.
//-- Designed specifically to work with Otto DIY+(PLUS) & Humanoid
/*************************************************** 
  make sure to install first all libraries from here: https://wikifactory.com/+OttoDIY/otto-diy-plus/
  Otto DIY invests time and resources providing this open source code, 
  please support Otto DIY and open-source hardware by purchasing kits from (http://ottodiy.com)
  
  BSD license, all text above must be included in any redistribution
 *******************************************************************/
//-----------------------------------------------------------------
#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <US.h>
#include <EnableInterrupt.h>
#include <OttoSerialCommand.h>
#include <Otto9.h>
Otto9 Otto;  //This is Otto!!
OttoSerialCommand SCmd;  //The SerialCommand object

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
//---------------------------------------------------------
#define PIN_Touch1 A0
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
//-- Movement parameters
int T = 1000;            //Initial duration of movement
int moveId = 0;          //Number of movement
int moveSize = 15;       //Asociated with the height of some movements
//---------------------------------------------------------
//--    * MODE = 0: Otto is awaiting
//--    * MODE = 1: Arming alarm system
//--    * MODE = 2: Otto Guardian
volatile int MODE = 0; //State of Otto in the principal state machine.
volatile int currentMode = 1; //To remember current mode when changing modes
//---------------------------------------------------------
volatile bool buttonPushed = false; //Variable to remember when a button has been pushed
volatile bool buttonAPushed = false; //Variable to remember when A button has been pushed
unsigned long previousMillis = 0;
bool obstacleDetected = false;
bool alarmActivated = false;
int initDistance = 999;
unsigned long int arming_symbol =   0b00111111100001100001100001111111;
unsigned long int alarm_symbol =    0b00111111111111111111111111111111;
int angryPos2[4] =    {90, 90, 70, 110};
int headLeft2[4] =    {110, 110, 90, 90};
int headRight2[4] =   {70, 70, 90, 90};
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600); //Serial communication initialization
  pinMode(PIN_Touch1, INPUT);
Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  //Otto.setTrims(TRIM_YL, TRIM_YR, TRIM_RL, TRIM_RR);
  //Otto.saveTrimsOnEEPROM(); //Uncomment this only for one upload when you finaly set the trims.
  enableInterrupt(PIN_Touch1, Touch1Pushed, RISING);
  SCmd.addDefaultHandler(receiveStop);   //Setup callbacks for SerialCommand commands
  Otto.sing(S_connection); //Otto wake up!
  delay(500);
  Otto.home();
  delay(50);
  previousMillis = millis();
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {

  if (Serial.available() > 0 && MODE != 3) {

    MODE = 3;
    Otto.putMouth(happyOpen);
    /*
        //Disable Pin Interruptions
        disableInterrupt(PIN_Touch1);

        buttonPushed = false;
    */
  }

  //First attemp to initial software
  if (buttonPushed) {

    Otto.home();
    //delay(100); //Wait for all buttons
    Otto.sing(S_buttonPushed);
    //delay(200); //Wait for all buttons
    MODE = currentMode;
    Otto.putMouth(MODE);
    if (MODE == 1) {
      Otto.sing(S_mode1);
    }

    else if (MODE == 2) {
      Otto.sing(S_mode2);
    }

    int showTime = 1500;
    while ((showTime > 0)) { //Wait to show the MODE number
      showTime -= 10;
      delay(10);
    }
    Otto.clearMouth();
    buttonPushed = false;
    buttonAPushed = false;
    alarmActivated = false;
    previousMillis = millis();

  } else {

    switch (MODE) {

      //-- MODE 0 - Otto is awaiting
      //---------------------------------------------------------
      case 0:

        //Every 60 seconds in this mode, Otto falls asleep
        if (millis() - previousMillis >= 60000) {
          OttoSleeping_withInterrupts(); //ZZzzzzz...
          previousMillis = millis();
        }
        break;
        
      //-- MODE 1 - Otto Arming alarm system
      //---------------------------------------------------------
      case 1:

        if (alarmActivated == false) {  //Arming alarm system
          OttoArmingAlarmSystem();

        } else {

          delay(100);
          int obstacleDistance = Otto.getDistance();
          int noise = Otto.getNoise();
          delay(100);
Serial.print(noise);
          //ALARM!!!!
          if ((noise >= 600) || (obstacleDistance < initDistance)) {

            delay(50);
            while (!buttonPushed) {
              Serial.print(noise);
              Otto.putMouth(alarm_symbol, 0);
              Otto.bendTones (note_A5, note_A7, 1.04, 5, 2);  //A5 = 880 , A7 = 3520

              delay(20);

              Otto.bendTones (note_A7, note_A5, 1.02, 5, 2);  //A5 = 880 , A7 = 3520
              Otto.clearMouth();
              delay(100);
            }
          }

        }
        break;

      //-- MODE 2 - Otto Guardian
      //---------------------------------------------------------
      case 2:

        if (alarmActivated == false) { //Arming alarm system
          OttoArmingAlarmSystem();

        } else {

          delay(100);
          int obstacleDistance = Otto.getDistance();
          int noise = Otto.getNoise();
          delay(100);

          //ALARM!!!!
          if ((noise >= 600) || (obstacleDistance < initDistance)) {
Serial.print(noise);
            delay(50);
            while (!buttonPushed) {

              Otto.putMouth(alarm_symbol, 0);
              Otto.bendTones (note_A5, note_A7, 1.04, 5, 2);  //A5 = 880 , A7 = 3520
              delay(20);

              Otto.bendTones (note_A7, note_A5, 1.02, 5, 2);  //A5 = 880 , A7 = 3520
              Otto.clearMouth();
              delay(100);
            }
          }

          if (millis() - previousMillis >= 8000) { //8sec

            OttoGuardian();

            if (!buttonPushed) {

              delay(100);
              initDistance = Otto.getDistance();
              delay(100);
              initDistance -= 10;
            }

            previousMillis = millis();

          }
        }

        break;

      case 3:

        SCmd.readSerial();
        break;

      default:
        MODE = 3;
        break;
    }

  }

}
///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////

//-- Function executed when second button is pushed
void Touch1Pushed() {

  buttonAPushed = true;

  if (!buttonPushed) {
    buttonPushed = true;
    Otto.putMouth(smallSurprise);
  }
  currentMode = MODE + 1;
  if (currentMode > 2) {
    currentMode = 0;
  }
}
//-- Function to receive Stop command.
void receiveStop() {

  sendAck();
  Otto.home();
  sendFinalAck();
}
//-- Function to send Ack comand (A)
void sendAck() {

  delay(30);

  Serial.print(F("&&"));
  Serial.print(F("A"));
  Serial.println(F("%%"));
  Serial.flush();
}
//-- Function to send final Ack comand (F)
void sendFinalAck() {

  delay(30);

  Serial.print(F("&&"));
  Serial.print(F("F"));
  Serial.println(F("%%"));
  Serial.flush();
}
//-- Functions with animatics
//--------------------------------------------------------

void OttoSleeping_withInterrupts() {

  int bedPos_0[4] = {100, 80, 60, 120}; //{100, 80, 40, 140}

  if (!buttonPushed) {
    Otto._moveServos(700, bedPos_0);  //800
  }

  for (int i = 0; i < 4; i++) {

    if (buttonPushed) {
      break;
    }
    
    Otto.putAnimationMouth(dreamMouth, 0);
    Otto.bendTones (100, 200, 1.04, 10, 10);

    if (buttonPushed) {
      break;
    }
    Otto.putAnimationMouth(dreamMouth, 1);
    Otto.bendTones (200, 300, 1.04, 10, 10);

    if (buttonPushed) {
      break;
    }
    Otto.putAnimationMouth(dreamMouth, 2);
    Otto.bendTones (300, 500, 1.04, 10, 10);

    delay(500);

    if (buttonPushed) {
      break;
    }
    Otto.putAnimationMouth(dreamMouth, 1);

    Otto.bendTones (400, 250, 1.04, 10, 1);

    if (buttonPushed) {
      break;
    }
    Otto.putAnimationMouth(dreamMouth, 0);
    Otto.bendTones (250, 100, 1.04, 10, 1);

    delay(500);
  }

  if (!buttonPushed) {
    Otto.putMouth(lineMouth);
    Otto.sing(S_cuddly);
  }

  Otto.home();
  if (!buttonPushed) {
    Otto.putMouth(happyOpen);
  }
}

void OttoArmingAlarmSystem() {

  int countDown = 10000; //10 sec
  while ((countDown > 0) && (!buttonPushed)) {

    countDown -= 1000;
    Otto.putMouth(arming_symbol, 0);
    Otto._tone(note_A7, 50, 0); //bip'
    Otto.clearMouth();
    delay(300);
    delay(650);
    if (buttonPushed) {
      break;
    }
  }
  if (!buttonPushed) {
    alarmActivated = true;
    delay(100);
    initDistance = Otto.getDistance();
    delay(100);
    initDistance -= 10;
    previousMillis = millis();
  }
}

void OttoGuardian() {

  int fretfulPos[4] =  {90, 90, 90, 110};

  if (!buttonPushed) {
    Otto.putMouth(smallSurprise);
    delay(100);
    Otto.sing(S_cuddly);
    delay(500);
  }

  if (!buttonPushed) {
    Otto.putMouth(angry);
    Otto.bendTones(note_A5, note_D6, 1.02, 20, 4);
    Otto.bendTones(note_A5, note_E5, 1.02, 20, 4);
    delay(300);
    Otto.putMouth(lineMouth);
  }

  for (int i = 0; i < 4; i++) {
    if (buttonPushed) {
      break;
    }
    Otto._moveServos(100, fretfulPos);
    Otto.home();
  }

  if (!buttonPushed) {
    Otto.putMouth(angry);
    delay(500);
  }
  if (!buttonPushed) {
    Otto._moveServos(1000, headLeft2);
    delay(400);
  }
  if (!buttonPushed) {
    Otto.home();
    delay(400);
  }

  if (!buttonPushed) {
    Otto._moveServos(1000, headRight2);
    delay(400);
  }

  if (!buttonPushed) {
    delay(300);
    Otto.home();
    Otto.putMouth(smile);
    delay(100);
  }

  if (!buttonPushed) {
    Otto.sing(S_happy_short);
    delay(800);
    Otto.clearMouth();
  }
}
