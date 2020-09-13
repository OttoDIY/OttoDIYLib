#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

Servo gripServo;
Servo rightServo;
Servo leftServo;

int gripPos;
int rightSpeed = 0;
int leftSpeed = 0;
int command;
int desiredDelay = 16;

//+++++++++++++++ULTRASONIC VARIABLES++++++++++++++++++++++++++++

#define echoPin 9 // Echo Pin
#define trigPin 8 // Trigger Pin
#define buzzerPin 13 // Pin for the buzzer

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long readDistance; // the output distance from the sensor

//+++++++++++++++FUNCTION DECLARATIONS+++++++++++++++++++++++++++
int ultraSensor(int theEchoPin, int theTrigPin);
void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

}

//primary arduino loop
void loop()
{
  //+++++++++++++++++++++++++++++++++++++Hand Follow+++++++++++++++++++++++++++++++++++++++

  int maxSpeed = 40;
  long duration, distance;

  distance = ultraSensor(echoPin, trigPin);

  //Move forward if bot is too far away
  if (distance > 25) {
    rightSpeed = -maxSpeed;   
    leftSpeed  = -maxSpeed;
    motorControl(rightSpeed, leftSpeed, rightServo, leftServo);
  }

  //Stop if you are at a distance
  if (distance >= 20 && distance < 25) {
    rightSpeed = 0;   
    leftSpeed  = 0;
    motorControl(rightSpeed, leftSpeed, rightServo, leftServo);
  }

  //Move backward if you are at the distance.
  if (distance < 20) {
    rightSpeed = maxSpeed; 
    leftSpeed  = maxSpeed;
    motorControl(rightSpeed, leftSpeed, rightServo, leftServo);
  }
}

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo) {

  //Set all of the servo positions
  theRightServo.write(90 - rightSpeed);  //compensate for the 0-180, -90-90 relationship
  theLeftServo.write(90 + leftSpeed);
  //mouthServo.write(desiredMouth);

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

    Serial.print("Distance: ");
    Serial.print(finalDistance);
    Serial.print(" cm");
    Serial.println();

  return finalDistance; //in cm
}
