//----------------------------------------------------------------
//-- CC BY SA (http://ottodiy.com)
//-- 11 December 2018
//-----------------------------------------------------------------
//-- Otto withTouch Sensor!
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
/*
              --^--       <== Touch Sensor A0
         --------------- 
        |     O   O     |
        |---------------|
YR 3==> |               | <== YL 2
         --------------- 
            ||     ||
RR 5==>   -----   ------  <== RL 4
         |-----   ------|


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
int movimiento = 0;
boolean izqder = true;
// TEMPO: 97 BPM

//---------------------------------------------------------
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
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {

   int estado = digitalRead(sensorPin);
 
   if (estado == HIGH)
   {
     Otto.sing(S_buttonPushed);
     movimiento = movimiento + 1;
   }

   if (movimiento == 1)
   {
        if(obstacleDetected){
          Serial.println("OBJETO DETECTADO"); 
          Otto.sing(S_OhOoh);
          
          if (izqder == true)
          {
            Otto.turn(4,1000,1);//2 steps turning RIGHT   
            izqder = false;
          }
          else
          {
            Otto.turn(4,1000,-1);//2 steps turning LEFT   
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

   if (movimiento == 2)
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

   if (movimiento == 3)
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
      movimiento = 0;
   }
}

///////////////////////////////////////////////////////////////////
//-- Function to read distance sensor & to actualize obstacleDetected variable
void obstacleDetector(){
   int distance = Otto.getDistance();
        if(distance<15){
          obstacleDetected = true;
        }else{
          obstacleDetected = false;
        }
}
