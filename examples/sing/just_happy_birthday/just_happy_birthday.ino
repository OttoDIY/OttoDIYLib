#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

  const int buzzer = 13;//birthdaybuzzerboi

void setup()
{ 
  Serial.begin(9600);

  
  //***Happy Birthday***
  pinMode(buzzer, OUTPUT);
  //***End of BDAY******
  
} 

void loop(){
        
  tone(buzzer,262);
  delay(250);
  tone(buzzer, 20000);
  delay(20);
  tone(buzzer,262);
  delay(250);
  tone(buzzer,294);
  delay(500);
  tone(buzzer,262);
  delay(500);
  tone(buzzer,349);
  delay(500);
  tone(buzzer,330);
  delay(500);
  
  tone(buzzer, 20000);
  delay(200);
  
  tone(buzzer,262);
  delay(250);
  tone(buzzer, 20000);
  delay(20);
  tone(buzzer,262);
  delay(250);
  tone(buzzer,294);
  delay(500);
  tone(buzzer,262);
  delay(500);
  tone(buzzer,392);
  delay(500);
  tone(buzzer,349);
  delay(500); 
  
  tone(buzzer, 20000);
  delay(200);
  
  tone(buzzer,262);
  delay(250);
  tone(buzzer, 20000);
  delay(20);
  tone(buzzer,262);
  delay(250);
  tone(buzzer,523);
  delay(500);
  tone(buzzer,440);
  delay(500);
  tone(buzzer,349);
  delay(500);
  tone(buzzer,330);
  delay(500);
  tone(buzzer,294);
  delay(750);
  
  tone(buzzer, 20000);
  delay(200);
  
  tone(buzzer,466);
  delay(250);
  tone(buzzer, 20000);
  delay(20);
  tone(buzzer,466);
  delay(250);
  tone(buzzer,440);
  delay(500);
  tone(buzzer,349);
  delay(500);
  tone(buzzer,392);
  delay(500);
  tone(buzzer,349);
  delay(500);
  
  tone(buzzer, 20000);
  delay(200);
  


}

