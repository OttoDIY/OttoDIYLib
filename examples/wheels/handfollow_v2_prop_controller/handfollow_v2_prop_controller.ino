// This Sketch uses a proportional controller in order to move close and far away from the hand

#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

//Define the servos
Servo gripServo; // not used in this program but keep the gripper frozen if using LittleBot Plus
Servo rightServo;
Servo leftServo;

int gripPos;
int rightSpeed = 0;
int leftSpeed = 0;
int command;
int desiredDelay = 16;
int ready = 0;

//+++++++++++++++ULTRASONIC SENSOR VARIABLES++++++++++++++++++++++++++++

#define echoPin 9 // Echo Pin
#define trigPin 8 // Trigger Pin
#define buzzerPin 13 // Pin for the buzzer

//+++++++++++++++FUNCTION DECLARATIONS++++++++++++++++++++++++++++++++++++

int ultraSensor(int theEchoPin, int theTrigPin);
void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(50);      //ensures the the arduino does not read serial for too long
  Serial.println("started");

  gripServo.attach(4);
  rightServo.attach(3); //pin s9 on Meped
  leftServo.attach(2);

  gripServo.write(50);        //intial positions of servos

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  ready = 0;
}

//primary arduino loop
void loop()
{
  //+++++++++++++++++++++++++++++++++++++Hand Follow+++++++++++++++++++++++++++++++++++++++

  //Local Variables
  long duration, distance;
  float followDistance = 30; // vary to change reactivity
  float distanceDifference = 0.0;

  // read the ultrasonic sensor (returns distance in cm)
  distance = ultraSensor(echoPin, trigPin);

  //find out how close or far the robot is from the hand
  distanceDifference = distance - followDistance;

  //Set the maximum speed that the motor can reach
  //(choose between 0-90) larger is a faster reaction.
  int maxSpeed = 150 ; 

  //Adjust the speed of the motor.
  //We are using just the leftMotor speed because the other motor just has to move in the same direction. So there is no reason to have 2 speeds.
  leftSpeed = -maxSpeed * (distanceDifference / followDistance);


  // error checking so the speed is not a number that is too large
  if (leftSpeed > 90) {
    leftSpeed = 90;
  }

  if (leftSpeed < -90) {
    leftSpeed = -90;
  }

  //print the final motorspeed
  Serial.print("MotorSpeed ");
  Serial.println(leftSpeed);

  motorControl(leftSpeed, leftSpeed, rightServo, leftServo);

} //end of main loop

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo) {

  //Speed are entered into this function as values between -90-90.
  // -90 is backward, 0 is stopped, and 90 is forward
  //Those values are then converted to a 0-180 range which is what the servo function of Arduino needs
  //Set all of the servo positions
  theRightServo.write(90 - rightSpeed);  //compensate for the 0-180, -90-90 relationship
  theLeftServo.write(90 + leftSpeed);

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int ultraSensor(int theEchoPin, int theTrigPin) {
  //this function caluclates and returns the distance in cm

  long duration, distance; // Duration used to calculate distance
  long numOfMeasurements = 10.0;
  int count = 0;
  long sumOfDistance = 0;
  long finalDistance = 0;

  for (int count = 0; count <= numOfMeasurements; count++) {

    /* The following trigPin/echoPin cycle is used to determine the
      distance of the nearest object by bouncing soundwaves off of it. */
    digitalWrite(theTrigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(theTrigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(theTrigPin, LOW);
    duration = pulseIn(theEchoPin, HIGH);

    //Calculate the distance (in cm) based on the speed of sound.
    distance = duration / 58.2;
    //Serial.println(distance);

    sumOfDistance = distance + sumOfDistance;


  }

  finalDistance = sumOfDistance / numOfMeasurements;

  //  Serial.print("Distance: ");
  //  Serial.print(finalDistance);
  //  Serial.print(" cm");
  //  Serial.println();

  return finalDistance; //in cm

}
