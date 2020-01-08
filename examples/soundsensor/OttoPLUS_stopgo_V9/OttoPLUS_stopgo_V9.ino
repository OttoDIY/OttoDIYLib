#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <US.h>
#include <EnableInterrupt.h>
#include <OttoSerialCommand.h>
#include <Otto9.h>
#include <Otto_Matrix9.h>
Otto9 Otto;  //This is Otto!!
OttoSerialCommand SCmd;  //The SerialCommand object
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
/***   Global variables and function definition  ***/
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)
bool estado = false;

void setup() {
Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
  Otto.matrixIntensity(1);// set up Matrix display intensity
    Otto.playGesture(OttoHappy);
    Otto.home();
}

void loop() {
    if (estado == true) {
        Otto.walk(1);
        
    }
    if (Otto.getNoise() >= 500) {
        Otto.putMouth(smallSurprise);
        Otto.putAnimationMouth(otto,true);
        Otto.sing(S_OhOoh);
        Otto.putMouth(bigSurprise);
        Otto.shakeLeg(1);
        if (estado == true) {
            estado = false;
            Otto.home();
        } else {
            estado = true;
            Otto.putMouth(smile);
        }
    }
}
