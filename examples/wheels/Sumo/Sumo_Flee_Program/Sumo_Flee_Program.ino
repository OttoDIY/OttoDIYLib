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
// SimpleSumo Robotics "Flee_Program"
// Original created September, 2017, by Michael Graham, www.EngineerDog.com
// 
// This program operates the SimpleSumo robot in 'Flee Mode' so that it will run from walls as sensed with the ultrasonic & button sensors.
// If captured (as sensed by the IR signal from being lifted off the ground) the robot will stop rotating its wheels and beep.
//
// This is a program segment intended to be used in conjunction with the "Sumo_Battle_Program".
//
// To make this work copy all of the code below and replace the entire 'SETUP SECTION' and 'LOOP SECTION' in the "Sumo_Battle_Program".
//
//
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
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//____ LOOP SECTION_________________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop(){
  CheckSensors(); //Review all input sensors for fresh data
  if (EdgeSensed == true){ //If Edge is Detected
     Buzz();
           SitStill();
                 delay(50);
        }else if (ButtonSensed == true){  //Enemy Detected via Button sensor
    Buzz();
        if (random(1,2)<2){ 
    MoveBackwardRight();
    }else{MoveBackwardLeft();}
    delay(random(50,300));
  }else if (UltraSensed == true){  //Enemy Detected via Ultrasonic sensor
                Buzz();
                    MoveBackPivot();
                    delay(random(50,1000));
                    RotateRight();
          delay(random(1,100));
  }else Search();  // Move in roving pattern seeking enemy

}

