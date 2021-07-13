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
#include <Otto.h>
Otto Otto;  //This is Otto!
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
#define LeftLeg 2 
#define RightLeg 3
#define LeftFoot 4 
#define RightFoot 5 
#define Buzzer  13 
#define Trigger 8 // ultrasonic sensor trigger pin
#define Echo 9 // ultrasonic sensor echo pin

const int sensorPin = A0;

  long ultrasound() {
   long duration, distance;
   digitalWrite(Trigger,LOW);
   delayMicroseconds(2);
   digitalWrite(Trigger, HIGH);
   delayMicroseconds(10);
   digitalWrite(Trigger, LOW);
   duration = pulseIn(Echo, HIGH);
   distance = duration/58;
   return distance;
}
int movement = 0;
boolean izqder = true; // TEMPO: 97 BPM
bool obstacleDetected = false;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
  Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); //Set the servo pins and Buzzer pin
  pinMode(sensorPin, INPUT);
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);
  Otto.home();
  Otto.sing(S_happy); // a happy Otto :)
}
// touch sensor is in "toggle mode", initial value LOW
// program expects HIGH value for first touch
// and then expects LOW value for second touch
// and then again HIGH and LOW etc.
//
  int estado = HIGH; //first expected touch value
  
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  
   if (digitalRead(sensorPin) == estado)
   {
     if (estado == HIGH)
     {
     estado = LOW; //toggle expect value
     }
     else
     {
      estado = HIGH; //toggle expect value
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
        Serial.println("Object detected");  
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
   int distance = ultrasound();
        if(distance<15){
          obstacleDetected = true;
        }else{
          obstacleDetected = false;
        }
}
