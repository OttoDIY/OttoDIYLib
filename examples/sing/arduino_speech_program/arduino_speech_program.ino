#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

//++++++++++++++DEFINITION OF VARIABLES+++++++++++++++++++++++++
Servo rightServo;  
Servo leftServo; 

int rightSpeed = 0;
int leftSpeed = 0;
int ready = 0;

#define buzzerPin 13 // Pin for the buzzer

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++FUNCTION DECLARATIONS+++++++++++++++++++++++++++
void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo);
void speakWalter (int soundPin, int maxWords);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++CODE SETUP+++++++++++++++++++++++++++++++++
void setup()
{ 
  Serial.begin(9600);
  rightServo.attach(3); 
  leftServo.attach(2);
  
  Serial.setTimeout(50);      //ensures the the arduino does not read serial for too long
  Serial.println("started");
 
  pinMode(buzzerPin, OUTPUT);
  
  ready = 0;
} 
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++PRIMARY ARDUINO LOOP++++++++++++++++++++++++++++++++++
void loop() 
{ 

 speakWalter (buzzerPin,20); //initiates speech function "speakWalter" to create a word, then say it
 delay(50);                  //Delay is used to put spaces in between the words
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
 speakWalter (buzzerPin,20);
 delay(50);
  rightSpeed = 40;          //Sets speed variable for the right motor
  leftSpeed  = -40;         //Sets speed variable for the left motor
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);  //initiates motor movement function, rotating the wheels
 delay(200);                //Delay for how long the wheels will move
 
  rightSpeed = 0;           //Sets stopped variable for the right motor
  leftSpeed  = 0;           //Sets stopped variable for the left motor
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);  //initiates motor movement function, stopping the wheels
  delay(150);               //Delay for motors to stop


  
  
  speakWalter (buzzerPin,20);
  delay(50);
  rightSpeed = -40;    
  leftSpeed  = 40;
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
  delay(400);
 
  rightSpeed = 0;    
  leftSpeed  = 0;
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
  delay(150);

  
  
  speakWalter (buzzerPin,20);
  delay(50);
  rightSpeed = -40;    
  leftSpeed  = 40;
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
  delay(200);
 
  rightSpeed = 0;    
  leftSpeed  = 0;
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
  delay(150);


  
  
  speakWalter (buzzerPin,20);
  delay(50);
  rightSpeed = 40;    
  leftSpeed  = -40;
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
  delay(400);
 
  rightSpeed = 0;    
  leftSpeed  = 0;
  motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
  delay(150);
}


//Code then loops to beginning, repeating process

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo){

 
  //Set all of the servo positions
  theRightServo.write(90 - rightSpeed);  //compensate for the 0-180, -90-90 relationship
  theLeftServo.write(90 + leftSpeed);
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
    toneDuration = random (100, 150);
    toneFreq = random (100, 400);
    tone(soundPin, toneFreq, toneDuration);  
    
    delay(toneDuration);

  }
  
}
