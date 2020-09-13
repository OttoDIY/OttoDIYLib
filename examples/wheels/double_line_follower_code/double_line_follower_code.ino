#include <Servo.h>  //arduino library
#include <math.h>   //standard c library
  
Servo rightServo;  
Servo leftServo; 

int rightSpeed = 0;
int leftSpeed = 0;
int ready = 0;

//+++++++++++++++LINE SENSOR VARIABLES++++++++++++++++++++++++++++

#define leftLineIR A6
#define rightLineIR A5

//+++++++++++++++FUNCTION DECLARATIONS+++++++++++++++++++++++++++

void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void setup()
{ 
  Serial.begin(9600);
  rightServo.attach(3);       
  leftServo.attach(2);
  Serial.setTimeout(50);      //ensures the the arduino does not read serial for too long
  Serial.println("started");
  
  ready = 0;
} 

//primary arduino loop
void loop() 
{
  // +++++++++++++++++++++++++++++++++++++++++++Line Follow Mode++++++++++++++++++++++++++++++++++++++++++
  
  float rightsensor = analogRead(rightLineIR); //pulls analog value from right line follower
  float leftsensor = analogRead(leftLineIR); //pulls analog value from left line follower
  
  if (rightsensor > 550 && leftsensor < 550){ //left wheel moves forward and right wheel moves backwards when right sensor hits the line
    rightSpeed = 10;    
    leftSpeed  = -20;
    motorControl(rightSpeed,leftSpeed, rightServo, leftServo); 
  }
  
  if (rightsensor < 550 && leftsensor > 550){ //right wheel moves forward and left wheel moves backwards when left sensor hits the line
    rightSpeed = -20;    
    leftSpeed  = 10;
    motorControl(rightSpeed,leftSpeed, rightServo, leftServo); 
  }
  
  if (rightsensor < 550 && leftsensor < 550){ //both wheels move foward when both sensors are on white
    rightSpeed = 20;    
    leftSpeed  = 20;
    motorControl(rightSpeed,leftSpeed, rightServo, leftServo); 
  }
  // ++++++++++++++++++++++++++++++++++++++++End of Line Follow+++++++++++++++++++++++++++++++++++++++

} // end of primary loop

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo){

  //Set all of the servo positions
  theRightServo.write(90 - rightSpeed);  //compensate for the 0-180, -90-90 relationship
  theLeftServo.write(90 + leftSpeed);
  //mouthServo.write(desiredMouth);

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

