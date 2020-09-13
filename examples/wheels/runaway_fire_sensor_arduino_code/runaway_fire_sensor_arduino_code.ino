#include <Servo.h>  //arduino library
#include <math.h>   //standard c library


const int RED_PIN = A1; //Defines RGB Red Pin
const int GREEN_PIN = A2; //Defines RGB Green Pin
const int BLUE_PIN = A3; //Defines RGB Blue Pin

Servo rightServo;  
Servo leftServo; 

int rightSpeed = 0;
int leftSpeed = 0;


//+++++++++++++++FUNCTION DECLARATIONS+++++++++++++++++++++++++++
void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo);


void setup() {
  Serial.begin(9600); 
  pinMode(RED_PIN, OUTPUT); //Determines that Red Pin is OUTPUT
  pinMode(GREEN_PIN, OUTPUT); //DEtermines that Green Pin is OUTPUT
  pinMode(BLUE_PIN, OUTPUT); //Determines that Blue Pin is OUTPUT
  
  rightServo.attach(3); //pin s9 on Meped
  leftServo.attach(2);
  
  Serial.setTimeout(50);      //ensures the the arduino does not read serial for too long
  Serial.println("started");
  
 
}

//Start of Primary Loop
void loop() {
int sensorReading = analogRead(A0);  // read the sensor on analog A0:

if (sensorReading <64){  //If statement runs if there is fire detected
/*  Serial.println("Fire"); */ //Serial monitor notification
  digitalWrite(RED_PIN, HIGH); //If sensor detects fire, it will turn on the Red LED
  digitalWrite(BLUE_PIN, LOW); //If sensor detects fire, it will turn off the Blue LED
  digitalWrite(GREEN_PIN, LOW); //If sensor detects fire, it will turn off the Green LED
  
    rightSpeed = 100;          //Sets speed variable for the right motor
  leftSpeed  = -100;         //Sets speed variable for the left motor
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);  //initiates motor movement function, rotating the wheels
 delay(500);                //Delay for how long the wheels will move
 
  rightSpeed = 0;           //Sets stopped variable for the right motor
  leftSpeed  = 0;           //Sets stopped variable for the left motor
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);  //initiates motor movement function, stopping the wheels
  delay(150);               //Delay for motors to stop
    rightSpeed = -100;          //Sets speed variable for the right motor
  leftSpeed  = -100;         //Sets speed variable for the left motor
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);  //initiates motor movement function, rotating the wheels
 delay(50000);                //Delay for how long the wheels will move
 
  rightSpeed = 0;           //Sets stopped variable for the right motor
  leftSpeed  = 0;           //Sets stopped variable for the left motor
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);  //initiates motor movement function, stopping the wheels
  delay(150);               //Delay for motors to stop
  
  
}else{ //Else statement runs when there is no fire detected               
 /* Serial.println("No Luck Kiddo"); */ //Serial monitor notification
  digitalWrite(RED_PIN, LOW); //If sensor doesn't detect fire, it will turn off the Red LED
  digitalWrite(BLUE_PIN, HIGH); //If sensor doesn't detect fire, it will turn on the Blue LED
  digitalWrite(GREEN_PIN, HIGH); //If sensor doesn't detect fire, it will turn on the Green LED
}

  delay(100); //short delay before resetting the loop
}


void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo){

 
  //Set all of the servo positions
  theRightServo.write(90 - rightSpeed);  //compensate for the 0-180, -90-90 relationship
  theLeftServo.write(90 + leftSpeed);
}

