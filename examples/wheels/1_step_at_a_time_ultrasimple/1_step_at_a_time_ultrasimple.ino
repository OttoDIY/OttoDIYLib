#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

Servo gripServo;
Servo rightServo;
Servo leftServo;

int desiredMouth;
int gripPos;
int rightSpeed = 0;
int leftSpeed = 0;
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
void motorControl2(int rightSpeed, int leftSpeed, int stepDelay);
void speakWalter (int soundPin, int maxWords);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void setup()
{
  Serial.begin(9600);

  //Set the Intial Positions of Everything
  gripServo.write(50);
  rightServo.write(90);
  leftServo.write(90);
  delay(1000);

  //Initialize the positions of everything
  gripServo.attach(4);
  rightServo.attach(3);
  leftServo.attach(2);

  Serial.setTimeout(50);      //ensures the the arduino does not read serial for too long
  Serial.println("started");

  //Active Non-Servo Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

}

//Primary arduino loop
void loop()
{

  //Simple Motor Control - Unsing the Provided LittleBot Functions
  // -90 - full speed backwards, 90 - full speed forwards, 0 - stopped

  // Forward for 2 seconds
  motorControl2 (45, 45, 2 );

  //Stop
  motorControl2 (0, 0, 2);

  // Turn Right for 2 seconds
  motorControl2 (0, 45, 2);

  //Stop
  motorControl2 (0, 0, 2);

  // Add more steps below here.
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*  Remove the "/*" comments from around code to activate it.
  
  //Simple Servo Control using Arduino Servo Functions
  // 0 - full speed backward, 180 full speed forward, 0 - stopped

  // Forward for 2 seconds
  rightServo.write(135);  // 90+45
  leftServo.write(45);    // 90-45
  delay (2000);

  //Stop
  rightServo.write(90);
  leftServo.write(90);
  delay (1000);

*/



} // end of primary loop

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl2(int rightSpeed, int leftSpeed, int stepDelay) {

  //Set all of the servo positions
  rightServo.write(90 + rightSpeed);  //compensate for the 0-180, -90-90 relationship
  leftServo.write(90 - leftSpeed);
  delay(stepDelay*1000);

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int ultraSensor(int theEchoPin, int theTrigPin) {
  //this function calculates and returns the distance in cm

  long duration, distance; // Duration used to calculate distance
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
  return distance;

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

void speakWalter (int soundPin, int maxWords) {
  int toneDuration;
  int numberOfWords;
  int toneFreq;

  numberOfWords = random (1, maxWords);
  //Serial.print("Number of words = ");
  //Serial.println(numberOfWords);

  for ( int i; i <= numberOfWords; i++) {
    toneDuration = random (25, 150);
    toneFreq = random (100, 1800);
    tone(soundPin, toneFreq, toneDuration);

    delay(toneDuration);

  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int servoParallelControl (int thePos, Servo theServo, int theSpeed ) {

  int startPos = theServo.read();        //read the current pos
  int newPos = startPos;
  //int theSpeed = speed;

  //define where the pos is with respect to the command
  // if the current position is less that the actual move up
  if (startPos < (thePos - 5)) {
    newPos = newPos + 1;
    theServo.write(newPos);
    delay(theSpeed);
    return 0;
  }

  else if (newPos > (thePos + 5)) {
    newPos = newPos - 1;
    theServo.write(newPos);
    delay(theSpeed);
    return 0;
  }

  else {
    return 1;
  }

}
