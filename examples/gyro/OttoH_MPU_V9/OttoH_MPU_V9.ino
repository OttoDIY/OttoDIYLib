
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Servo.h"
#include <Otto9Humanoid.h>
MPU6050 mpu;
Otto9Humanoid Otto;  //This is Otto!

int16_t ax, ay, az;
int16_t gx, gy, gz;
 
Servo servo1;
Servo servo2;
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_LA 6 //servo[4]  Left arm if enabled
#define PIN_RA 7 //servo[5]  Right arm if enabled
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)
#define PIN_NoiseSensor A6  //SOUND SENSOR   ANALOG pin (A6)
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1=top 2=bottom 3=left 4=right
#define PIN_Button   A0 // TOUCH SENSOR Pin (A0)
 
int val1;
int val2;
int prevVal1;
int prevVal2;
int touchVal = 0;
int noiseVal = 0;

void setup() 
{
    Wire.begin();
    Serial.begin(38400);
    Serial.println("Initialize MPU");
    mpu.initialize();
    Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
    servo1.attach(4);
    servo2.attach(5);
    Otto.initHUMANOID(PIN_YL, PIN_YR, PIN_RL, PIN_RR, PIN_LA, PIN_RA, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins
    Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
    Otto.matrixIntensity(1);// set up Matrix display intensity
}
 
void loop() 
{
      mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        Serial.print("a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
    
  val1 = map(ax, -17000, 17000, 0, 179);
    if (val1 != prevVal1)
    {
        servo1.write(val1);
        prevVal1 = val1;
         Otto.playGesture(OttoHappy);
    Otto.putMouth(sad);
    }
    val2 = map(ay, -17000, 17000, 0, 179);
    if (val2 != prevVal2)
    {
        servo2.write(val2);
        prevVal2 = val2;
        
     Otto.playGesture(OttoConfused);
     Otto.putMouth(interrogation);
    }
 
    delay(50);



  // test touch sensor
  touchVal = digitalRead(PIN_Button);  //  touch sensor
  if (touchVal == HIGH) {
    Otto.playGesture(OttoLove);
    delay(10);
    touchVal = !touchVal;
  }

    // test noise sensor
  noiseVal = Otto.getNoise();
  delay(50);
  Serial.println(noiseVal);
  if (noiseVal > 30) {
    Otto.home();
    Otto.playGesture(OttoFart);
    Otto.putMouth(culito);
    Otto.bendTones (100, 200, 1.04, 10, 10);
    delay (100);
    noiseVal = 0;
  }
  
}
