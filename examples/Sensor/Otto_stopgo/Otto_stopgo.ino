#include <Otto.h>
Otto Otto;  //This is Otto!!
/***   Global variables and function definition  ***/
#define LeftLeg 2 
#define RightLeg 3
#define LeftFoot 4 
#define RightFoot 5 
#define Buzzer  13 
#define DIN A3 // Data In pin
#define CS A2  // Chip Select pin
#define CLK A1 // Clock pin
#define Orientation 1 // 8x8 LED Matrix orientation  Top  = 1, Bottom = 2, Left = 3, Right = 4 
bool estado = false;

void setup() {
 Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); //Set the servo pins and Buzzer pin
  Otto.initMATRIX( DIN, CS, CLK, Orientation); 
  Otto.matrixIntensity(7);// set up Matrix display intensity
    Otto.playGesture(OttoHappy);
    Otto.home();
}

void loop() {
    if (estado == true) {
        Otto.walk(1);
  
    }
    if (digitalRead(A0)) {
        Otto.putMouth(smallSurprise);
        Otto.putAnimationMouth(otto,true);
        Otto.sing(S_OhOoh);
        Otto.putMouth(bigSurprise);
        Otto.shakeLeg(1);
        if (estado == true) {
            estado = false;
            Otto.home();
        } else {
            estado = true;
            Otto.putMouth(smile);
        }
    }
}
