#include <Otto9.h>
Otto9 Otto;  //This is Otto!!
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)

void setup() {
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
    Otto.playGesture(OttoHappy);
    Otto.home();
}

void loop() {
    if (Otto.getNoise() >= 500) {
        Otto.walk(1);
       
    }
}
