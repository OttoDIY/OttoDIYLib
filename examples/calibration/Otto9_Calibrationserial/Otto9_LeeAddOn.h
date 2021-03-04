/*************************************************************************************
//-- Otto 9 Library Add Ons 
//
//-- Additional functions that aren't included in the recent Otto V9 Library 
//-- but needed to make the Otto_TestSerial sketch run properly with the
//-- alternated PINs configuration for the Touch-, Noise- and Distance- Sensors.
//
//
//-- Written for the OttoDIY Builders Community
//-- February 2021: (Birger T)
//   Sorry..But i'm not that CPP-Programmer, and I don't want to mess around with two
//   files (.h & .cpp) when it could be done in one .inc file (but .inc isn't allowed
//   in the Arduino IDE).
/*************************************************************************************

 Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
 Otto DIY invests time and resources providing open source code and hardware,
 please support by purchasing kits from (https://www.ottodiy.com)

 BSD license, all text above must be included in any redistribution

 ************************************************************************************/
#ifndef OTTO9_LEEADDON_H
#define OTTO9_LEEADDON_H

Servo Lee_Servo;
int Lee_servo_trim;
int Lee_servo_position;
unsigned int Lee_final_time;
unsigned int Lee_partial_time;
float Lee_increment;


void Lee_moveHead(uint16_t time, int  servo_target) {

  if (time > 10) {
    Lee_increment = ((servo_target) - Lee_servo_position) / (time / 10.0);
    Lee_final_time =  (uint16_t)millis() + time;
    for (int iteration = 1; (uint16_t)millis() < Lee_final_time; iteration++) {
      Lee_partial_time = (uint16_t)millis() + 10;
      Lee_Servo.write(Lee_servo_position + (iteration * Lee_increment) + Lee_servo_trim);
      while ((uint16_t)millis() < Lee_partial_time); //pause
    }
  }
  else {
    Lee_Servo.write(servo_target + Lee_servo_trim);
  }
  Lee_servo_position = servo_target + Lee_servo_trim;
} // Lee_moveHead()


void Lee_setTrim(int trim) { Lee_servo_trim = trim; }
int Lee_getTrim() {return Lee_servo_trim; }

void Lee_initHead(int pin) {
    Lee_Servo.attach(pin); 
    Lee_servo_trim = EEPROM.read(6);
      if (Lee_servo_trim > 128) Lee_servo_trim -= 256;
    Lee_moveHead(10, 90 + Lee_servo_trim);
}
#endif // OTTO9_LEEADDON_H
