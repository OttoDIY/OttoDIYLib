// Copyright (C) 2017 EngineerDog.com LLC
//
// This work is licensed under the Creative Commons 
// Attribution-NonCommercial-ShareAlike 4.0 International License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
// Attribution for derivations of this work should be made to: EngineerDog.com LLC
// 
// You may use this work for noncommercial purposes without cost as long as you give us
// credit for our work (attribution) and any improvements you make are licensed in a way
// no more restrictive than our license. See the license for more details.
//
// For example, you may build a SimpleSumo yourself and use this code for your own experiments,
// or you can build one and give the SimpleSumo running this code to a friend, as long as you
// don't charge for it. 
// If you have a question about whether a contemplated use is in compliance with the license,
// just ask us. We're friendly. Email us at MechEngineerMike.com
//
// For more information about SimpleSumo see https://engineerdog.com/simplesumo/,  
//
//***************************************************************************
//
// SimpleSumo Robotics "Sumo_Battle_Program"
// Original created September, 2017, by Michael Graham, www.EngineerDog.com
//
// This program operates the SimpleSumo robot in sumo wrestling mode.  
// The program is written to facilitate the customization of the SimpleSumo Robot modules & behaviour.
// Inputs are 2 buttons, 1 Ultrasonic Sensor, 3 edge sensors.
// Outputs are 2 continuous servo motors and a buzzer. Optional outputs include 2 180deg servos. 
//
// LEFT and RIGHT point of view defined by looking at top of robot when it is driving away from you. 
//
// The Circuit: 
//  A0 = 
//  A1 = 
//  A2 = 
//  A3 = Edge Sensor 'A0', Back
//  A4 = Edge Sensor 'A0', Right
//  A5 = Edge Sensor 'A0', Left
//  A6 = 
//  A7 = 
//  D0 = (reserved for Bluetooth connection later)
//  D1 = (reserved for Bluetooth connection later)
//  D2 = Buzzer long leg
//  D3~ = Continuous Servo, Right
//  D4 = 
//  D5~ = Continuous Servo, Left
//  D6~ = Ultrasonic 'Trig'
//  D7 = Ultrasonic 'Echo'
//  D8 = 
//  D9~ = Accessory Servo SG90, Right
//  D10~ = Button Input, Right
//  D11~ = Button Input, Left
//  D12 = Accessory Servo SG90, Left 
//  D13 = (reserved for the embedded LED on this pin)
//  Any 5V = Ultrasonic 'Vcc', 3x Edge Sensor 'Vcc', Servo Motor 'Vcc', 
//  Any GND = Ultrasonic 'GND', 3x  Edge Sensor 'GND', Servo Motor 'Gnd', 2x Button Output, Buzzer short leg

//***************************************************************************

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____DECLARATIONS SECTION__________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Servo.h>  //Arduino library that allows for simple control of servo motors. 

Servo LeftServo;   //Assigns a name to the left side continuous wheel servo
Servo RightServo;  //Assigns a name to the right side continuous wheel servo

Servo LeftServoExtra;  //Assigns a name to the left side 0-180 deg accessory servo
Servo RightServoExtra;  //Assigns a name to the right side 0-180 deg accessory servo

const int echoPin = 7; // Echo Pin
const int trigPin = 6; // Trigger Pin
const int MaximumRange = 60; // Ultrasonic sensor maximum range needed, in cm
const int MinimumRange = 0; // Ultrasonic sensor minimum range needed, in cm
long Duration, Distance; // Variables used to calculate the distance of sensed objects. 
 
const int RightButtonPin = 10; // Right Button, Digital Pin #
const int LeftButtonPin = 11; // Left Button, Digital Pin #

const int RightServoPin = 3; // Right Continuous Servo, Digital Pin #
const int LeftServoPin = 5; // Left Continuous Servo, Digital Pin #

const int RightServoExtraPin = 9; // Right Continuous Servo, Digital Pin #
const int LeftServoExtraPin = 12; // Left Continuous Servo, Digital Pin #

const int EdgeBackPin = 3;  //Edge Sensor Rear Center, Analog Pin #
const int EdgeRightPin = 4;  //Edge Sensor Front Left, Analog Pin #
const int EdgeLeftPin = 5;  //Edge Sensor Front Right, Analog Pin #

const int BuzzerPin = 2; //Buzzer sound output pin

const int EdgeSensitivity = 500;  //Between 0-1000, White surfaces read LOW, dark surfaces read High. This variable determines when line is crossed. 

int BackEdgeState = 0;    //Logic variable for recording if this edge was triggered or not
int RightEdgeState = 0;   //Logic variable for recording if this edge was triggered or not
int LeftEdgeState = 0;    //Logic variable for recording if this edge was triggered or not

int RightButtonState = HIGH; // Default state of the right button
int LeftButtonState = HIGH; // Default state of the left button

bool EdgeSensed = false;
bool UltraSensed = false;
bool ButtonSensed = false;

int StuckCounter = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____SETUP SECTION_________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 void setup()
{
 Serial.begin(9600);        // initialize serial communication at 9600 baud. The serial monitor is used for data collection.
 pinMode(trigPin, OUTPUT);  //Sets ultrasonic sensor trigger pin as an electronic output. 
 pinMode(echoPin, INPUT);    //Sets ultrasonic sensor echo pin as an electronic output. 

 RightServo.attach(RightServoPin);  //Uses the servo library to set the right servo pin #
 RightServo.write(90);  // Continuous Servo: 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
 
 LeftServo.attach(LeftServoPin);  //Uses the servo library to set this pin # as the left servo
 LeftServo.write(90);  // Continuous Servo: 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
 
 RightServoExtra.attach(RightServoExtraPin);  //Uses the servo library to set this pin # as the left servo
 RightServoExtra.write(90);  // Continuous Servo: 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  
 LeftServoExtra.attach(LeftServoExtraPin);  //Uses the servo library to set this pin # as the left servo
 LeftServoExtra.write(90);  // Continuous Servo: 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds) 
  
 pinMode(RightButtonPin, INPUT_PULLUP); //Input_pullup sets the pin to normally HIGH, which then gets pull LOW if the button (normally open) is pressed. 
 pinMode(LeftButtonPin, INPUT_PULLUP);  //Input_pullup sets the pin to normally HIGH, which then gets pull LOW if the button (normally open) is pressed. 

 pinMode(BuzzerPin, OUTPUT);
 

 Countdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____ LOOP SECTION_________________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop(){
   //SitStill(); //This is a function written for troubleshooting purposes
   PrintSerial(); //Uncomment this only for troubleshooting. It will slow down your robots reaction time during a battle!
   //AccessoryLoop(); //A simple loop written to activate the accessory servos for display purposes.
  CheckSensors(); //Review all input sensors for fresh data
  if (EdgeSensed == true){ //If Edge is Detected
    //Buzz();
    StayInRing(); // Move & reorient the robot away from ring edge
  }else if (ButtonSensed == true){  //Enemy Detected via Button sensor
    Buzz();
    Attack(); // Brief charge forward
  }else if (UltraSensed == true){  //Enemy Detected via Ultrasonic sensor
                Buzz();
        Persue(); // Reorient the robot with intent to attack
        }else Search();  // Move in roving pattern seeking enemy

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____ FUNCTIONS SECTION___________________________________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UltraSense(){ // The following trigPin/echoPin cycle is used to determine the distance of the nearest object by bouncing soundwaves off of it. 
 digitalWrite(trigPin, LOW); // Clears the trigPin 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH); // Sets trigPin to HIGH for 10 micro seconds
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW); 

 Duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
 
 Distance = (Duration/2)/29.1; //Calculate the distance in cm based on the speed of sound in air at STP.
 
 if (Distance >= MaximumRange || Distance < MinimumRange){
   Distance = MaximumRange;
   }
 } 
 
void CheckSensors(){ //Review all input sensors for fresh data and set the robot state

LeftButtonState = digitalRead(LeftButtonPin);
RightButtonState = digitalRead(RightButtonPin);
BackEdgeState = analogRead(EdgeBackPin);
RightEdgeState = analogRead(EdgeRightPin);
LeftEdgeState = analogRead(EdgeLeftPin);
UltraSense();
 
if (BackEdgeState > EdgeSensitivity || RightEdgeState > EdgeSensitivity || LeftEdgeState > EdgeSensitivity){
  EdgeSensed = true; //Edge positively sensed
  } else {
      EdgeSensed = false;
      }

if (Distance < MaximumRange){
  UltraSensed= true;  //Enemy Detected via Ultrasonic sensor
  } else {
      UltraSensed = false;
      }
  
if (LeftButtonState == LOW || RightButtonState == LOW ){
  ButtonSensed = true;  //Enemy Detected via Button sensor
  } else {
      ButtonSensed = false;
      }
}

void Buzz() { //Increasing the duration of the buzz changes the volume drastically. Set delay to 80+ms for a loud beep!
   digitalWrite(BuzzerPin, HIGH);
   delay(5);
   digitalWrite(BuzzerPin, LOW);
}

void BuzzLong() {  //Increasing the duration of the buzz changes the volume drastically. Set delay to 80+ms for a loud beep!
   digitalWrite(BuzzerPin, HIGH);
   delay(50);
   digitalWrite(BuzzerPin, LOW);
}


void StayInRing(){   // Move & reorient the robot away from ring edge
        if (RightEdgeState > EdgeSensitivity && LeftEdgeState > EdgeSensitivity){
      MoveBackward();
      delay (random(300,700)); //Duration of this motion in milliseconds
      RotateRight();
      delay (random(10,550)); //Duration of this motion in milliseconds
      } else if (RightEdgeState > EdgeSensitivity){
        MoveBackwardRight();
        delay (random(300,700)); //Duration of this motion in milliseconds
        RotateLeft(); 
        delay (random(10,550)); //Duration of this motion in milliseconds
      } else if (LeftEdgeState > EdgeSensitivity){
        MoveBackwardLeft();
        delay (random(300,700)); //Duration of this motion in milliseconds
        RotateRight(); 
        delay (random(10,550)); //Duration of this motion in milliseconds
      }
    if (BackEdgeState > EdgeSensitivity ) {
      MoveForwardFast();
      delay (1000); //Duration of this motion in milliseconds
      }
}

void Persue(){ // Reorient the robot with intent to attack
        MoveForwardFast();
    delay (100); //This is the motion duration
}

void Attack(){// Brief charge forward
        if (RightButtonState == LOW && LeftButtonState == LOW) {
           MoveForwardFast();
           delay (100); //Duration of this motion in milliseconds
            }else if (LeftButtonState == LOW) {
           PivotLeft();
           delay (100); //Duration of this motion in milliseconds
            } else if (RightButtonState == LOW) {
           PivotRight();
           delay (100); //Duration of this motion in milliseconds
            }
}

void Evade(){} // Reorient the robot with intent to escape

void Search(){ // Move in roving pattern seeking enemy
    MoveForwardSlow();
    delay (50); //Duration of this motion in milliseconds
  }


void Countdown(){ //This pauses the robot for 5 seconds (5000 milliseconds) after it is turned on, per competition requirements. Then it beeps the 5 sec countdown. 
  Buzz();
  delay (995); 
  Buzz();
  delay (995); 
  Buzz();
  delay (995); 
  Buzz();
  delay (995); 
  BuzzLong();
  delay (950); 
}

void AccessoryLoop(){ //This pauses the robot for 5 seconds (5000 milliseconds) after it is turned on, per competition requirements. Then it beeps the 5 sec countdown. 
   LeftServoExtra.write(0);  //(>90 is forward rotation for left motor). (0 and 180 are the opposing extreme POSITIONS)
   RightServoExtra.write(180);  //(<90 is forward rotation for right motor).  (0 and 180 are the opposing extreme POSITIONS)
       delay(1000);
   LeftServoExtra.write(180);  //(>90 is forward rotation for left motor). (0 and 180 are the opposing extreme POSITIONS)
   RightServoExtra.write(0);  //(<90 is forward rotation for right motor).  (0 and 180 are the opposing extreme POSITIONS)
       delay(1000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____ MOTION COMMANDS (Just a sublevel of the functions)__________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PivotLeft(){
   LeftServo.write(90+15);  //(>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
   RightServo.write(90-70);  //(<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}  

void PivotRight(){
   LeftServo.write(90+70);  //(>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
   RightServo.write(90-15);  //(<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}  

void RotateRight(){
   LeftServo.write(90+25);  //(>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
   RightServo.write(90+25);  //(<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
} 

void RotateLeft(){
   LeftServo.write(90-25);  //(>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
   RightServo.write(90-25);  //(<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
} 


void SitStill(){
    LeftServo.write(90);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(90);  // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}

void MoveBackward(){
    LeftServo.write(90-25);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(90+25);  // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}

void MoveBackwardLeft(){
    LeftServo.write(90-20);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(90+45);  // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}

void MoveBackwardRight(){
    LeftServo.write(90-45);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(90+20);  // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
}

void MoveForwardSlow(){
    LeftServo.write(90+15);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds) 
  RightServo.write(90-15); // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds) 
}

void MoveForwardFast(){
    LeftServo.write(180);  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(0); // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds) 
}

void MoveRandom(){
    LeftServo.write(random(0,180));  // (>90 is forward for left motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds)
  RightServo.write(random(0,180)); // (<90 is forward for right motor). 90 sets servo speed to zero. (0 and 180 are the opposing extreme speeds) 
}

int PrintSerial()                        //This function prints all available data to the serial monitor
{
  Serial.print(millis()/1000);                // print the time in milliseconds since the program started
  Serial.print(',');                     // print a comma
  Serial.print(LeftButtonState);         // print the status of the left button
  Serial.print(',');                     // print a comma
  Serial.print(RightButtonState);        // print the status of the right button
  Serial.print(',');                     // print a comma
  Serial.print(Distance);                // print the ultrasonic distance sensor output
  Serial.print(',');                     // print a comma 
  Serial.print(BackEdgeState);           // print the status of the back edge sensor
  Serial.print(',');                     // print a comma
  Serial.print(RightEdgeState);          // print the status of the right edge sensor
  Serial.print(',');                     // print a comma
  Serial.println(LeftEdgeState);         // print the status of the left edge sensor
}

