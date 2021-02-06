//----------------------------------------------------------------
//-- CC BY SA (http://ottodiy.com)
//-- 03 March 2020
//-----------------------------------------------------------------
//-- Otto with Touch Sensor!
//-- Author: Alberto Prieto
//-- This Otto uses a touch sensor to switch three modes
//-- Mode 1: Otto avoid obstacles
//-- Mode 2: Otto follow the hand
//-- Mode 3: Otto dances!
//-----------------------------------------------------------------
#include <Servo.h> 
#include <Oscillator.h>
#include <US.h>
#include <Otto9.h>

Otto9 Otto;  //This is Otto!
//---------------------------------------------------------
//-- First step: Make sure the pins for servos are in the right position
/*             -------- 
              |  O  O  |
              |--------|
  RIGHT LEG 3 |        | LEFT LEG 2
               -------- 
               ||     ||
RIGHT FOOT 5 |---     ---| LEFT FOOT 4  
*/
  #define PIN_YL 2 //servo[2]
  #define PIN_YR 3 //servo[3]
  #define PIN_RL 4 //servo[4]
  #define PIN_RR 5 //servo[5]

  const int sensorPin = A0;
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
//-- Movement parameters
int movement = 0;
boolean izqder = true;
// TEMPO: 97 BPM
bool obstacleDetected = false;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
  Otto.init(PIN_YL,PIN_YR,PIN_RL,PIN_RR,true, -1, 13, 8, 9);
  pinMode(sensorPin, INPUT);
  Otto.home();
  Otto.sing(S_happy); // a happy Otto :)
}
// touch sensor is in "toggle mode", initial value LOW
// program expects HIGH value for first touch
// and then expects LOW value for second touch
// and then again HIGH and LOW etc.
//
  int estado = HIGH;
  
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  
   if (digitalRead(sensorPin) == estado)
   {
     if (estado == HIGH)
     {
     estado = LOW;
     }
     else
     {
      estado = HIGH;
     }
     Otto.sing(S_buttonPushed);
     movement = movement + 1;
     delay(500);
   }
   if (movement == 1)
   {
        if(obstacleDetected){
          Serial.println("OBJETO DETECTADO"); 
          Otto.sing(S_OhOoh);
          
          if (izqder == true)
          {
            Otto.walk(2,1000,-1); 
            Otto.turn(2,1000,1);//2 steps turning RIGHT   
            izqder = false;
          }
          else
          {
            Otto.walk(2,1000,-1); 
            Otto.turn(2,1000,-1);//2 steps turning LEFT   
            izqder = true;
          }
          
          //Otto.home(); 
          delay(50); 
          obstacleDetector();
        } 
        else
        {
          Otto.walk(1,500,1); 
          delay(50);
          //Otto.home();
          obstacleDetector();
        }
   }

   if (movement == 2)
   {
      if(obstacleDetected){ 
        Serial.println("OBJETO DETECTADO");  
        Otto.walk(2,1000,1);
        Otto.home(); 
        //delay(60);
        obstacleDetector();
        //delay(10) ;
        }        
        else{ 
            obstacleDetector(); 
        } 
   }

   if (movement == 3)
   {
      Otto.jitter(10,500,40); 
      Otto.home();
      Otto.moonwalker(2,1000,30,1);
      Otto.home();
      Otto.ascendingTurn(2,500,50); 
      Otto.home();
      Otto.tiptoeSwing(2,1000,30); 
      Otto.home();
      Otto.flapping(2,500,40,1);
      Otto.home();
      Otto.crusaito(2,3000,40,1);
      Otto.home();
      Otto.shakeLeg(2,1000,1);  
      Otto.home();
      Otto.sing(S_disconnection);
      movement = 0;
   }
}

///////////////////////////////////////////////////////////////////
//-- Function to read distance sensor & to update obstacleDetected variable
void obstacleDetector(){
   int distance = Otto.getDistance();
        if(distance<15){
          obstacleDetected = true;
        }else{
          obstacleDetected = false;
        }
}
