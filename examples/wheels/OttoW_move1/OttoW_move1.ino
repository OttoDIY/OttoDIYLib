#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

Servo rightServo;
Servo leftServo;

int rightSpeed = 0;
int leftSpeed = 0;

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl2(int rightSpeed, int leftSpeed, int stepDelay) {   // -90 - full speed backwards, 90 - full speed forwards, 0 - stopped

  //Set all of the servo positions
  rightServo.write(90 + rightSpeed);  //compensate for the 0-180, -90-90 relationship
  leftServo.write(90 - leftSpeed);
  delay(stepDelay*1000);

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void setup()
{
  //Set the Intial Positions of Everything
  rightServo.write(90);
  leftServo.write(90);
  delay(1000);
  //Initialize the positions of everything
  rightServo.attach(3);
  leftServo.attach(2);
}

void loop()
{
  motorControl2 (45, 45, 2 ); // Forward for 2 seconds
  motorControl2 (45, -45, 2 ); // turn
  motorControl2 (-45, 45, 2 ); // turn
  motorControl2 (-45, -45, 2 ); // back
  motorControl2 (0, 0, 2); //Stop 
} 
