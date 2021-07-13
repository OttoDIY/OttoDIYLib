//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Otto avoid obstacles with ultrasonic sensor sample sketch code
// https://www.ottodiy.com
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <Otto.h>
Otto Otto;  //This is Otto!
//----------------------------------------------------------------------
#define LeftLeg 2 
#define RightLeg 3
#define LeftFoot 4 
#define RightFoot 5 
#define Buzzer  13 
#define Trigger 8 // ultrasonic sensor trigger pin
#define Echo 9 // ultrasonic sensor echo pin

long ultrasound() {
   long duration, distance;
   digitalWrite(Trigger,LOW);
   delayMicroseconds(2);
   digitalWrite(Trigger, HIGH);
   delayMicroseconds(10);
   digitalWrite(Trigger, LOW);
   duration = pulseIn(Echo, HIGH);
   distance = duration/58;
   return distance;
}


void setup() {
  Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); //Set the servo pins and Buzzer pin
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT); 

}

void loop() {
    if (ultrasound() <= 15) {
      Otto.sing(S_surprise);
      Otto.playGesture(OttoConfused);
      Otto.walk(2,1000,-1); // BACKWARD x2
      Otto.turn(3,1000,1); // LEFT x3
    }
    Otto.walk(1,1000,1); // FORWARD x1

}
