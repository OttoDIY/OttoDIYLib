//----------------------------------------------------------------
//-- Otto Align
//-- This code will make the servo motors to find the "center" position for assembly
//-- March 2020: Designed to work with the Otto Humanoid but could be compatible with Otto DIY, PLUS or other biped robots
/******************************************************************************************************
  Otto DIY invests time and resources providing open source code and hardware, 
  please support by purchasing kits from (https://www.ottodiy.com)
  
  BSD license, all text above must be included in any redistribution
 *******************************************************************/
//-----------------------------------------------------------------
#include <Servo.h>
//-- First step: Configure the pins where the servos are attached
/*             -------- 
  RIGHT ARM 7 |  O  O  | LEFT ARM 6
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
Servo LA_Servo;  // create servo object to control a RR 6
Servo RA_Servo;  // create servo object to control a RR 7

int minim = 10;
int maxim = 170;

void setup() {
  YL_Servo.attach(2);  // attaches the servo on pin 2 to the servo object 1
  YR_Servo.attach(3);  // attaches the servo on pin 3 to the servo object 2 
  RL_Servo.attach(4);  // attaches the servo on pin 4 to the servo object 3
  RR_Servo.attach(5);  // attaches the servo on pin 5 to the servo object 4 
  LA_Servo.attach(6);  // attaches the servo on pin 5 to the servo object 5 
  RA_Servo.attach(7);  // attaches the servo on pin 5 to the servo object 6 

  YL_Servo.write(minim);                  // sets the servo 1 position 
  YR_Servo.write(minim);                  // sets the servo 2 position
  RL_Servo.write(minim);                  // sets the servo 3 position 
  RR_Servo.write(minim);                  // sets the servo 4 position
  LA_Servo.write(minim);                  // sets the servo 5 position
  RA_Servo.write(minim);                  // sets the servo 6 position
delay(3000);
  YL_Servo.write(maxim);                  // sets the servo 1 position 
  YR_Servo.write(maxim);                  // sets the servo 2 position
  RL_Servo.write(maxim);                  // sets the servo 3 position 
  RR_Servo.write(maxim);                  // sets the servo 4 position
  LA_Servo.write(maxim);                  // sets the servo 5 position
  RA_Servo.write(maxim);                  // sets the servo 6 position
delay(3000);
}

void loop() {
  YL_Servo.write(90);                  // sets the servo 1 position 
  YR_Servo.write(90);                  // sets the servo 2 position
  RL_Servo.write(90);                  // sets the servo 3 position 
  RR_Servo.write(90);                  // sets the servo 4 position
  delay(500);
  LA_Servo.write(90);                  // sets the servo 5 position
  RA_Servo.write(90);                  // sets the servo 6 position
}
