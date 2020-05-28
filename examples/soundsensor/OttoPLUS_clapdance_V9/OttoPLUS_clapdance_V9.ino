#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <US.h>
#include <Otto9.h>
Otto9 Otto;  //This is Otto!!
/***   Global variables and function definition  ***/
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)
int randomDance=0;
int randomSteps=0;
int T=1000;              //Initial duration of movement
int moveId=0;            //Number of movement
int moveSize=15;   

/***   Setup  ***/
void setup() {
    Serial.begin(9600); //Serial communication initialization
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin

    Otto.playGesture(OttoHappy);
    Otto.home();
  randomSeed(analogRead(A6));
}

/***   Loop  ***/
void loop() {
  int noise = Otto.getNoise();
          delay(100);
              Serial.print("Noise level:");
Serial.println(noise);
    if (Otto.getNoise() >= 500) {
        int noise = Otto.getNoise();
          delay(100);
              Serial.print("Noise level:");
       delay(50);  //Wait for the possible 'lag' of the button interruptions. 
                      //Sometimes, the noise sensor detect the button before the interruption takes efect 
 
            Otto.putMouth(bigSurprise);
            Otto.sing(S_OhOoh);
            Otto.putMouth(random(10,21));
            randomDance=random(5,21);
            move(randomDance);
            Otto.home();
            delay(500); //Wait for possible noise of the servos while get home
            Otto.putMouth(happyOpen); 
    }
}

void move(int moveId){

  bool manualMode = false;

  switch (moveId) {
    case 0:
      Otto.home();
      break;
    case 1: //M 1 1000 
      Otto.walk(1,T,1);
      break;
    case 2: //M 2 1000 
      Otto.walk(1,T,-1);
      break;
    case 3: //M 3 1000 
      Otto.turn(1,T,1);
      break;
    case 4: //M 4 1000 
      Otto.turn(1,T,-1);
      break;
    case 5: //M 5 1000 30 
      Otto.updown(1,T,moveSize);
      break;
    case 6: //M 6 1000 30
      Otto.moonwalker(1,T,moveSize,1);
      break;
    case 7: //M 7 1000 30
      Otto.moonwalker(1,T,moveSize,-1);
      break;
    case 8: //M 8 1000 30
      Otto.swing(1,T,moveSize);
      break;
    case 9: //M 9 1000 30 
      Otto.crusaito(1,T,moveSize,1);
      break;
    case 10: //M 10 1000 30 
      Otto.crusaito(1,T,moveSize,-1);
      break;
    case 11: //M 11 1000 
      Otto.jump(1,T);
      break;
    case 12: //M 12 1000 30 
      Otto.flapping(1,T,moveSize,1);
      break;
    case 13: //M 13 1000 30
      Otto.flapping(1,T,moveSize,-1);
      break;
    case 14: //M 14 1000 20
      Otto.tiptoeSwing(1,T,moveSize);
      break;
    case 15: //M 15 500 
      Otto.bend(1,T,1);
      break;
    case 16: //M 16 500 
      Otto.bend(1,T,-1);
      break;
    case 17: //M 17 500 
      Otto.shakeLeg(1,T,1);
      break;
    case 18: //M 18 500 
      Otto.shakeLeg(1,T,-1);
      break;
    case 19: //M 19 500 20
      Otto.jitter(1,T,moveSize);
      break;
    case 20: //M 20 500 15
      Otto.ascendingTurn(1,T,moveSize);
      break;
    default:
        manualMode = true;
      break;
  }       
}
