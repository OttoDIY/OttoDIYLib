//----------------------------------------------------------------
//-- Otto Foot align
//-- This code will make the servo motors to find the ZERO position for assembly
//-- April 2019: Designed to work with the basic Otto but could be compatible with PLUS or Humanoid or other biped robots
/******************************************************************************************************
  Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
  Otto DIY invests time and resources providing open source code and hardware, 
  please support by purchasing kits from (https://www.ottodiy.com)
  
  BSD license, all text above must be included in any redistribution
 *******************************************************************/
//-----------------------------------------------------------------
#include <Servo.h>
//-- First step: Configure the pins where the servos are attached
/*             -------- 
              |  O  O  |
              |--------|
  RIGHT LEG 3 |        | LEFT LEG 2
               -------- 
               ||     ||
RIGHT FOOT 5 |---     ---| LEFT FOOT 4  
*/
Servo YL_Servo;  // create servo object to control a YL 2
Servo YR_Servo;  // create servo object to control a YR 3
Servo RL_Servo;  // create servo object to control a RL 4
Servo RR_Servo;  // create servo object to control a RR 5

int minim = 10;
int maxim = 170;

void setup() {
  YL_Servo.attach(2);  // attaches the servo on pin 2 to the servo object 1
  YR_Servo.attach(3);  // attaches the servo on pin 3 to the servo object 2 
  RL_Servo.attach(4);  // attaches the servo on pin 4 to the servo object 3
  RR_Servo.attach(5);  // attaches the servo on pin 5 to the servo object 4 

  YL_Servo.write(minim);                  // sets the servo 1 position 
  YR_Servo.write(minim);                  // sets the servo 2 position
  RL_Servo.write(minim);                  // sets the servo 3 position 
  RR_Servo.write(minim);                  // sets the servo 4 position
delay(3000);
  YL_Servo.write(maxim);                  // sets the servo 1 position 
  YR_Servo.write(maxim);                  // sets the servo 2 position
  RL_Servo.write(maxim);                  // sets the servo 3 position 
  RR_Servo.write(maxim);                  // sets the servo 4 position
delay(3000);
}

void loop() {
  YL_Servo.write(90);                  // sets the servo 1 position 
  YR_Servo.write(90);                  // sets the servo 2 position
  RL_Servo.write(90);                  // sets the servo 3 position 
  RR_Servo.write(90);                  // sets the servo 4 position
}
