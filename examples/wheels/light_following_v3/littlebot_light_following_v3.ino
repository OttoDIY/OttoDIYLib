#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

Servo gripServo;  
Servo rightServo;  
Servo leftServo; 

int desiredMouth;
int gripPos;
int rightSpeed = 0;
int leftSpeed = 0;
int command;
int desiredDelay = 16;
int ready = 0;
int  robotMode = 222;

//+++++++++++++++ULTRASONIC VARIABLES++++++++++++++++++++++++++++

#define echoPin 9 // Echo Pin
#define trigPin 8 // Trigger Pin
#define buzzerPin 13 // Pin for the buzzer

bool holder = 1;
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long readDistance; // the output distance from the sensor

//+++++++++++++++FUNCTION DECLARATIONS+++++++++++++++++++++++++++
int ultraSensor(int theEchoPin, int theTrigPin);
void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo);
void speakWalter (int soundPin, int maxWords);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void setup()
{ 
  Serial.begin(9600);
  gripServo.attach(4);
  rightServo.attach(3); //pin s9 on Meped
  leftServo.attach(2);

  Serial.setTimeout(50);      //ensures the the arduino does not read serial for too long
  Serial.println("started");

  gripServo.write(50);        //intial positions of servos

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  ready = 0;
} 

//primary arduino loop
void loop() 
{
  // ++++++++++++++Follow Light++++++++++++++++++++++++++
  float LightValue2;
  LightValue2 = analogRead(A4);
  
  float LightValue3;
  LightValue3 = analogRead(A5);
  
  //Any value over 980 shown on the first sensor is converted back to 980
  if(LightValue2>980){
    LightValue2=980;
  }
  
  //Takes the first sensor and makes its value about the same as the second sensor
  LightValue2=1115-LightValue2;
  
  Serial.println("LightValue2");
  Serial.println(LightValue2);
  
  Serial.println("LightValue3");
  Serial.println(LightValue3);
  
  //Equations that determine at which speed each motor should be moving based on the light shown on each photocell
  rightSpeed = (LightValue3/LightValue2)*(-30);
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
  leftSpeed = (LightValue2/LightValue3)*(30);
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
  
  //Stops the bot if light value is over 450
  if(LightValue2 > 450&&LightValue3 > 450){
    rightSpeed=0;
    leftSpeed=0;
    motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
  }
  
  Serial.println("Right Speed");
  Serial.println(rightSpeed);

  Serial.println("Left Speed");
  Serial.println(leftSpeed);
  
  // +++++++++++++End of Follow Light++++++++++++++++++

} // end of primary loop

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo){


  //Set all of the servo positions
  theRightServo.write(90 - rightSpeed);  //compensate for the 0-180, -90-90 relationship
  theLeftServo.write(90 + leftSpeed);
  //mouthServo.write(desiredMouth);

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  int ultraSensor(int theEchoPin, int theTrigPin){
  //this fucntion caluclates and returns the distance in cm

  long duration, distance; // Duration used to calculate distance
  /* The following trigPin/echoPin cycle is used to determine the
   distance of the nearest object by bouncing soundwaves off of it.*/
  digitalWrite(theTrigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(theTrigPin, HIGH);
  delayMicroseconds(10); 

  digitalWrite(theTrigPin, LOW);
  duration = pulseIn(theEchoPin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;  
  return distance;

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

void speakWalter (int soundPin, int maxWords){
  int toneDuration;
  int numberOfWords;
  int toneFreq;

  numberOfWords = random (1,maxWords);
  //Serial.print("Number of words = ");
  //Serial.println(numberOfWords);

  for ( int i; i<= numberOfWords; i++){
    toneDuration = random (25, 150);
    toneFreq = random (100, 1800);
    tone(soundPin, toneFreq, toneDuration);  

    delay(toneDuration);

  }

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int servoParallelControl (int thePos, Servo theServo, int theSpeed ){

  int startPos = theServo.read();        //read the current pos
  int newPos = startPos;
  //int theSpeed = speed;

  //define where the pos is with respect to the command
  // if the current position is less that the actual move up
  if (startPos < (thePos-5)){     
    newPos = newPos + 1;
    theServo.write(newPos);
    delay(theSpeed);
    return 0;      
  }

  else if (newPos > (thePos + 5)){
    newPos = newPos - 1;
    theServo.write(newPos);
    delay(theSpeed);
    return 0;
  }  
  else {
    return 1;
  }
}
