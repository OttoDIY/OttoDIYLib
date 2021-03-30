//----------------------------------------------------------------
//-- Otto All moves test
//-- This code will make Otto make all functions, you can reorganize moves, sounds or gestures
//-- April 2019: Designed to work with the basic Otto but could be compatible with PLUS or Humanoid or other biped robots
/******************************************************************************************************
  Make sure to have installed all libraries: https://github.com/OttoDIY/OttoDIYLib
  Otto DIY invests time and resources providing open source code and hardware, 
  please support by purchasing kits from (https://www.ottodiy.com) 
  BSD license, all text above must be included in any redistribution
 *******************************************************************/
//-----------------------------------------------------------------
#include <Otto9.h>
Otto9 Otto;  //This is Otto!
//----------------------------------------------------------------------
#define PIN_YL 2 //Left leg
#define PIN_YR 3 //Right leg
#define PIN_RL 4 //Left foot
#define PIN_RR 5 //Right foot
#define PIN_Trigger  8  //TRIGGER pin
#define PIN_Echo     9  //ECHO pin
#define PIN_Buzzer  13  //BUZZER pin
#define DIN_PIN A3 // Data In
#define CS_PIN A2  // Chip Select
#define CLK_PIN A1 // Clock
#define LED_DIRECTION 1 // 8x8 LED MATRIX orientation  Top  = 1, Bottom = 2, Left = 3, Right = 4 
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
    delay(50);
  Otto.playGesture(OttoHappy);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() { 
  Otto.walk(2,1000,1); //2 steps, "TIME". IF HIGHER THE VALUE THEN SLOWER (from 600 to 1400), 1 FORWARD
  Otto.walk(2,1000,-1); //2 steps, T, -1 BACKWARD 
  Otto.turn(2,1000,1);//3 steps turning LEFT
  Otto._tone(10, 3, 1);
  Otto.bendTones (100, 200, 1.04, 10, 10);
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
  Otto.jump(1,500); // It doesn't really jumpl ;P
  Otto.home();
     delay(100); 
  Otto.sing(S_cuddly);
  Otto.sing(S_OhOoh);
  Otto.sing(S_OhOoh2);
  Otto.sing(S_surprise);
  Otto.sing(S_buttonPushed);       
  Otto.sing(S_mode1);        
  Otto.sing(S_mode2);         
  Otto.sing(S_mode3);  
  Otto.sing(S_sleeping);
  Otto.sing(S_fart1);
  Otto.sing(S_fart2);
  Otto.sing(S_fart3);
  Otto.sing(S_happy);
  Otto.sing(S_happy_short);                   
  Otto.sing(S_superHappy);   
  Otto.sing(S_sad);               
  Otto.sing(S_confused);
  Otto.sing(S_disconnection);
    delay(100);  
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
    delay(100);  
  Otto.putMouth(zero);
  delay(100);
  Otto.putMouth(one);
  delay(100);
  Otto.putMouth(two);
  delay(100);
  Otto.putMouth(three);
  delay(100);
  Otto.putMouth(four);
  delay(100);
  Otto.putMouth(five);
  delay(100);
  Otto.putMouth(6);
  delay(100);
  Otto.putMouth(7);
  delay(100);
  Otto.putMouth(8);
  delay(100);
  Otto.putMouth(9);
  delay(100);
  Otto.putMouth(smile);
  delay(100);
  Otto.putMouth(happyOpen);
  delay(100);
  Otto.putMouth(happyClosed);
  delay(100);
  Otto.putMouth(heart);
  delay(100);
  Otto.putMouth(angry);
  delay(100);
  Otto.putMouth(smallSurprise);
  delay(100);
  Otto.putMouth(bigSurprise);
  delay(100);
  Otto.putMouth(tongueOut);
  delay(100);
  Otto.putMouth(confused);
  delay(100);
  Otto.putMouth(21); //diagonal
  delay(100);
  Otto.putMouth(27); //interrogation
  delay(100);
  Otto.putMouth(23); //sad open
  delay(100);
  Otto.putMouth(24); //sad closed
  delay(100);
  Otto.putMouth(vamp1);
  delay(100);
  Otto.putMouth(vamp2);
  delay(100);
  Otto.putMouth(xMouth);
  delay(100);
  Otto.putMouth(okMouth);
  delay(100);
  Otto.putMouth(thunder);
  delay(100);
  Otto.putMouth(lineMouth);
  delay(100);
  Otto.putMouth(culito);
    delay(100); 
  Otto.putAnimationMouth(littleUuh,0);
  delay(1000); 
  Otto.putAnimationMouth(dreamMouth, 0);
  delay(1000); 
  Otto.putAnimationMouth(dreamMouth, 1);
  delay(1000); 
  Otto.putAnimationMouth(dreamMouth, 2);
  delay(1000); 
}
