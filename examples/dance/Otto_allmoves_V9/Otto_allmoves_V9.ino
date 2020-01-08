//----------------------------------------------------------------
//-- Otto All moves test
//-- This code will make Otto make all functions, you can reorganize moves, gestures or uncoment sings in the principal loop
//-- April 2019: Designed to work with the basic Otto but could be compatible with PLUS or Humanoid or other biped robots
/******************************************************************************************************
  Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
  Otto DIY invests time and resources providing open source code and hardware, 
  please support by purchasing kits from (https://www.ottodiy.com)
  
  BSD license, all text above must be included in any redistribution
 *******************************************************************/
//-----------------------------------------------------------------
#include <Otto9.h>
Otto9 Otto;  //This is Otto!
//----------------------------------------------------------------------
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
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
  delay(50);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() { 
          Otto.walk(2,1000,1); //2 steps, "TIME". IF HIGHER THE VALUE THEN SLOWER (from 600 to 1400), 1 FORWARD
          Otto.walk(2,1000,-1); //2 steps, T, -1 BACKWARD 
          Otto.turn(2,1000,1);//3 steps turning LEFT
          Otto.home();
          delay(100);  
          Otto.turn(2,1000,-1);//3 steps turning RIGHT 
          Otto.bend (1,500,1); //usually steps =1, T=2000
          Otto.bend (1,2000,-1);     
          Otto.shakeLeg (1,1500, 1);
          Otto.home();
          delay(100);
          Otto.shakeLeg (1,2000,-1);
          Otto.moonwalker(3, 1000, 25,1); //LEFT
          Otto.moonwalker(3, 1000, 25,-1); //RIGHT  
          Otto.crusaito(2, 1000, 20,1);
          Otto.crusaito(2, 1000, 20,-1);
          delay(100); 
          Otto.flapping(2, 1000, 20,1);
          Otto.flapping(2, 1000, 20,-1);
          delay(100);        
          Otto.swing(2, 1000, 20);
          Otto.tiptoeSwing(2, 1000, 20);
          Otto.jitter(2, 1000, 20); //(small T)
          Otto.updown(2, 1500, 20);  // 20 = H "HEIGHT of movement"T 
          Otto.ascendingTurn(2, 1000, 50);
          Otto.jump(1,2000);
        Otto._tone(10, 3, 1);
        Otto.bendTones (100, 200, 1.04, 10, 10);
          delay(50); 
Otto.putMouth(zero);
Otto.putMouth(one);
Otto.putMouth(two);
Otto.putMouth(three);
Otto.putMouth(four);
Otto.putMouth(five);
Otto.putMouth(6);
Otto.putMouth(8);
Otto.putMouth(9);
Otto.putMouth(smile);
Otto.putMouth(happyOpen);
Otto.putMouth(happyClosed);
Otto.putMouth(heart);
Otto.putMouth(angry);
Otto.putMouth(smallSurprise);
Otto.putMouth(bigSurprise);
Otto.putMouth(tongueOut);
Otto.putMouth(confused);
Otto.putMouth(21); //diagonal
Otto.putMouth(27); //interrogation
Otto.putMouth(23); //sad open
Otto.putMouth(24); //sad closed
Otto.putMouth(vamp1);
Otto.putMouth(vamp2);
Otto.putMouth(xMouth);
Otto.putMouth(okMouth);
Otto.putMouth(thunder);
Otto.putMouth(lineMouth);
Otto.putMouth(culito);
Otto.putAnimationMouth(littleUuh,0);
Otto.putAnimationMouth(dreamMouth, 0);
Otto.putAnimationMouth(dreamMouth, 1);
Otto.putAnimationMouth(dreamMouth, 2);
          //Otto.sing(S_cuddly);
          //Otto.sing(S_OhOoh);
          //Otto.sing(S_OhOoh2);
          //Otto.sing(S_surprise);
          //Otto.sing(S_buttonPushed);       
          //Otto.sing(S_mode1);        
          //Otto.sing(S_mode2);         
          //Otto.sing(S_mode3);  
          //Otto.sing(S_sleeping);
          //Otto.sing(S_fart1);
          //Otto.sing(S_fart2);
          //Otto.sing(S_fart3);
          //Otto.sing(S_happy);
          //Otto.sing(S_happy_short);                   
          //Otto.sing(S_superHappy);   
          //Otto.sing(S_sad);               
          //Otto.sing(S_confused);
          //Otto.sing(S_disconnection);
          Otto.home();
          Otto.playGesture(OttoHappy);
          Otto.playGesture(OttoSuperHappy);
          Otto.playGesture(OttoSad);
          Otto.playGesture(OttoVictory); 
          Otto.playGesture(OttoAngry); 
          Otto.playGesture(OttoSleeping);
          Otto.playGesture(OttoFretful);
          Otto.playGesture(OttoLove);
          Otto.playGesture(OttoConfused);        
          Otto.playGesture(OttoFart);
          Otto.playGesture(OttoWave);
          Otto.playGesture(OttoMagic);
          Otto.playGesture(OttoFail);
          Otto.home();  
}
