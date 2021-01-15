//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Otto_avoid   sample sketch code
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY PLUS APP Firmware version 9 (V9)
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <Otto9.h> //-- Otto Library version 9
Otto9 Otto;  //This is Otto!
//---------------------------------------------------------
//-- First step: Configure the pins where the servos are attached
/*             -------- 
              |  O  O  |
              |--------|
  RIGHT LEG 3 |        | LEFT LEG 2
               -------- 
               ||     ||
RIGHT FOOT 5 |---     ---| LEFT FOOT 4  
*/
// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  13 //BUZZER pin (13)
// SERVO ASSEMBLY PIN   /////////////////////////////////////////////////////////////////////
// to help assemble Otto's feet and legs - wire link between pin 7 and GND
#define PIN_ASSEMBLY    7   //ASSEMBLY pin (7) LOW = assembly    HIGH  = normal operation
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
int distance; // variable to store distance read from ultrasonic range finder module
bool obstacleDetected = false; // logic state for when object detected is at the distance we set
//-- Movement parameters
int T=1000;              //Initial duration of movement
int moveId=0;            //Number of movement
int moveSize=15;         //Asociated with the height of some movements
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(19200);
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  pinMode(PIN_ASSEMBLY,INPUT_PULLUP); // - Easy assembly pin - LOW is assembly Mode
  //Otto wake up!
  Otto.sing(S_connection);// Otto makes a sound
  Otto.home(); // Otto moves to its ready position
  delay(500); // wait for 500 milliseconds to allow Otto to stop
// if Pin 7 is LOW then place OTTO's servos in home mode to enable easy assembly, 
// when you have finished assembling Otto, remove the link between pin 7 and GND
  while (digitalRead(PIN_ASSEMBLY) == LOW) {
    Otto.home();// Otto moves to its ready position
    Otto.sing(S_happy_short);   // sing every 5 seconds so we know OTTO is still working
    delay(5000);// wait for 5 seconds
  }
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
 if (obstacleDetected) { // if there is an object closer than 15cm then we do the following
        Otto.sing(S_surprise); // sound a surprise
        Otto.jump(5, 500); // Otto jumps
        Otto.sing(S_cuddly); // sound 
        //Otto takes three steps back
        for (int i = 0; i < 3; i++) Otto.walk(1, 1300, -1); //repeat  three times the walk back command
        delay(50);// small delay to allow Otto to settle
        //Otto turns left 3 steps
        for (int i = 0; i < 3; i++) { //repeat  three times 
            Otto.turn(1, 1000, 1); // the walk left command
            delay(50);// small delay to allow Otto to settle
            obstacleDetector(); 
            }  
          }    
          else { // if nothing in front then walk forward
        Otto.walk(1, 1000, 1); //Otto walk straight
        obstacleDetector(); // call the function to check the ultrasonic range finder for an object closer than 15cm
        }
      
}  
//////////////////////////////////////////////////////////////////////////////////
//-- Function to read distance sensor & to actualize obstacleDetected variable //
/////////////////////////////////////////////////////////////////////////////////
void obstacleDetector() {
  int distance = Otto.getDistance(); // get the distance from the ultrasonic range finder
  if (distance < 15) obstacleDetected = true; // check to see if this distance is closer than 15cm, true if it is
  else obstacleDetected = false;// false if it is not
   Serial.print(distance);
  Serial.println("cm");
 if(distance<15){
          obstacleDetected = true;
          Serial.println ("Obstacle Detected! Avoid collision");
        }else{
          obstacleDetected = false;
          Serial.println ("No Obstacle detected! - Keep on walking");
        }
}
