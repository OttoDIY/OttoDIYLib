//----------------------------------------------------------------
//-- Otto Avoid obstacles with TRIMS servo calibration values to modify
//-- With this code Otto will walk until detects an obstacle with his ultrasound sensor you can open serial to read distance and test
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
#include <US.h>
#include <Otto.h>
Otto Otto;  //This is Otto!
//----------------------------------------------------------------------
//-- Make sure the servos are in the right pin, double check your Ultrasound zensor some have TRIG and ECHO swapped!
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
 
#define TRIM_LEFT_LEG   0 
#define TRIM_RIGHT_LEG  0
#define TRIM_LEFT_FOOT  0 
#define TRIM_RIGHT_FOOT 0
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  Serial.println("Hello Otto Builder");
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  Otto.setTrims(TRIM_LEFT_LEG,TRIM_RIGHT_LEG, TRIM_LEFT_FOOT, TRIM_RIGHT_FOOT);
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() 
{
  bool obstacleDetected = false;
  
  obstacleDetected = obstacleDetector(); 
  
  if(obstacleDetected)
  { 
        Otto.sing(S_surprise); 
        Otto.walk(7,1000,BACKWARD); 
        delay(1000);
        Otto.sing(S_happy);
        Otto.turn(10,1000,RIGHT);                
        delay(2000); 
          
   }        
   else
   { 
          Otto.walk(1,1000,FORWARD);  
    }          
 }  

///////////////////////////////////////////////////////////////////
//-- Function to read distance sensor & to actualize obstacleDetected variable
bool obstacleDetector()
{
   int distance = Otto.getDistance();
   
   Serial.println(distance);
   
   if(distance<15)
   {
      return true;
   }
   else
   {
      return false;
   }
}
