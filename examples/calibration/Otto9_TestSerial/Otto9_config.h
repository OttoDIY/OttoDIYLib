
//----------------------------------------------------------------
//-- Otto config
//
//-- These many code lines are needed to setup the Otto in each sketch..
//-- Why messing around with them..?!
//-- Edit this file once so it is suitable for your Otto, and in your sketch you'll
//-- have to include only two lines to make it..
//----------------------------------------------------------------

/*   
// ================================= Template / Basic Sketch to copy in a new file: ====
//
#include "Otto_config.h" // includes all the files needed to make Otto run
//
void setup() {
    Otto_Start(); // does all the setup for your Otto
    // put your setup code here, to run once:

} // setup()

void loop() {
    // put your main code here, to run repeatedly:

} // loop()

=======================================================================================
*/
//   Written for the Otto Builder Community
//   January 2021: (Birger T)
//   February 2021: Birger T fixed preprocessor defines that doesn't work
/*************************************************************************************

     Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
     Otto DIY invests time and resources providing open source code and hardware,
     please support by purchasing kits from (https://www.ottodiy.com)

     BSD license, all text above must be included in any redistribution

 ************************************************************************************/
#ifndef Otto_config_h
#define Otto_config_h
// -------------------------------------------------------------------------------- //
//                                                      OTTO SKETCH CONFIGURATION
// -------------------------------------------------------------------------------- //
// Set the baudrate for serial communication (set also inside serial monitor)
//#define BAUD 0      // or disable all 'Serial.'-communication code
#define BAUD  9600  //  9600 = default speed
//#define BAUD 19200  // 19200 = double default speed
//#define BAUD 38400  // 38400 = double double default speed
//#define BAUD 57600  // 57600 = double double and a half double double default speed ;-)
//
/* Edit: this part is useles and doesn't work as expected
// ? ? ? ? ? ----- which Otto do you want to program ? ? ? ? ?
// Edit 
// B = Basic, C = Lasercut, E = Eyes, P = Plus, H = Humanoid,
// (L = Lee not implemented yet)
//#define OTTO H
*/
// ? ? ? ? ? ----- which Otto Components to enable ? ? ? ? ?
#define OTTO_MOUTH  true    // true to enable the 8x8 LED matrix / false to disable
#define OTTO_EYES   true    // true if you got the Eyes matrix on board / else false
#define OTTO_EARS   true    // and what's about noise recognitions microphone
#define OTTO_RANGE  true    // true for the ultrasonic range meter
#define OTTO_FEEL   true    // also for the touchsensor
#define OTTO_LEGS   true    // true for bipeds, false for wheels or no legs
#define OTTO_ARMS   true    // true for Humanoid only ?!
#define OTTO_HEAD   false   // true for Robot Lee only ?!
#define OTTO_LAPOS  170     // Left arm servo home position
#define OTTO_RAPOS  10      // Right arm servo home position
#define OTTO_WHEELS false   // true for wheels, false for bipeds or no wheels
#define OTTO_FLYS   false   // true when Otto can fly (but not yet supported)
#define OTTO_BTAPP  false   // true for enabling Bluetooth App remote control
#define OTTO_BATT   true    // true for enabling the Batterie Reader
//
#define OTTO_OPINS  false   // true = Use the Otto Original Pin setup
                            // false = For an User Setup with rebuild pin connections
                            // US Sensor echo and trigger connected at one Pin 10;
                            // Leg servos moved from pins 2 and 3 to pins 8 and 9;
                            // Touch sensor connected to Interrupt pin 2;
                            // Noise sensor connected to Interrupt pin 3;
// -------------------------------------------------------------------------------- //
//                                                                  OTTO PIN SETUPs
// -------------------------------------------------------------------------------- //
#if OTTO_OPINS
//
// the standard PIN Layout as described in the Otto Humanoid Docs
//
//                  0   // IDE Serial Monitor RX
//                  1   // IDE Serial Monitor TX
#define PIN_LL      2   // Servo[0] left leg
#define PIN_RL      3   // Servo[1] right leg
#define PIN_LF      4   // Servo[2] left foot
#define PIN_RF      5   // Servo[3] right foot
#define PIN_LA      6   // Servo[4] left arm
#define PIN_RA      7   // Servo[5] right arm
#define PIN_UStrigg 8   // Ultrasound
#define PIN_USecho  9   // Ultrasound
//                  10  // not in use
//                  11  // softSerial RX = BLE Modul TX
//                  12  // softSerial TX = BLE Modul RX
#define PIN_Buzzer  13  // Buzzer
#define PIN_Button  A0  // Touchsensor
#define PIN_CLK     A1  // Mouth 8x8 LED matrix
#define PIN_CS      A2  // Mouth 8x8 LED matrix
#define PIN_DIN     A3  // Mouth 8x8 LED matrix
//                  A4  // TWI/I2C SDA (Eyes matrix)
//                  A5  // TWI/I2C SCL (Eyes matrix)
#define PIN_Noise   A6  // Noise sensor
#define PIN_Batt    A7  // Reading the Batterie Level
#else
//----------------------------------------------------------------- //
// Suggested alternative PIN Layout, to get the real states 
// of the Touchsensor and Noisesensor
//                  0   // IDE Serial Monitor RX
//                  1   // IDE Serial Monitor TX
#define PIN_Button  2   // Touchsensor
#define PIN_Noise   3   // Noise sensor
#define PIN_LF      4   // Servo[2] left foot
#define PIN_RF      5   // Servo[3] right foot
#define PIN_LA      6   // Servo[4] left arm
#define PIN_RA      7   // Servo[5] right arm
#define PIN_LL      8   // Servo[0] left leg
#define PIN_RL      9   // Servo[1] right leg
#define PIN_UStrigg 10  // Ultrasound
#define PIN_USecho  10  // Ultrasound
#define PIN_BLE_RX  11  // softSerial RX = BLE Modul TX
#define PIN_BLE_TX  12  // softSerial TX = BLE Modul RX
#define PIN_Buzzer  13  // Buzzer
//                  A0  // not in use
#define PIN_CLK     A1  // Mouth 8x8 LED matrix
#define PIN_CS      A2  // Mouth 8x8 LED matrix
#define PIN_DIN     A3  // Mouth 8x8 LED matrix
//                  A4  // TWI/I2C SDA (Eyes matrix)
//                  A5  // TWI/I2C SCL (Eyes matrix)
//                  A6  // not in use
#define PIN_Batt    A7  // Reading the Batterie Level

#endif

// -------------------------------------------------------------------------------- //
//                                             INCLUDE LIBRARIES & CREATE OBJECTS
// -------------------------------------------------------------------------------- //

// #if (OTTO == H) doesn't work
#if OTTO_ARMS && OTTO_HEAD  // then it's a Robot Lee
#include <Otto9Humanoid.h>
//include RobotLeeLib
#define SERVOCNT 7      // Count of servo variables needed
Otto9Humanoid Otto;     // Create an Otto object
#elif OTTO_ARMS         // when it's a Humanoid
#include <Otto9Humanoid.h>
#define SERVOCNT 6      // Count of servo variables needed
Otto9Humanoid Otto;     // Create an Otto object
#else                   // all other Bipeds
#include <Otto9.h>
#define SERVOCNT 4      // Count of servo variables needed
Otto9 Otto;             // Create an Otto object
#endif
// --------------------------------------------------------- MOUTH
#define LED_DIR      1  // Mouth 8x8 LED matrix Orientation

#if OTTO_MOUTH
// the nesessary matrix lib is included by the main lib
#endif
// --------------------------------------------------------- EYES
#if OTTO_EYES
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16matrix Eyes = Adafruit_8x16matrix();
#include "Otto_eyes5.h"
//
static const uint8_t PROGMEM
logo_bits[] = { B01111110, B10000001, B10111101, B10100101,
                B10111101, B10010001, B10111001, B10010001,
                B10010001, B10111001, B10010001, B10111101,
                B10100101, B10111101, B10000001, B01111110
              };
/* copy the next lines..
  #define logo_width   8
  #define logo_height 16
  static unsigned char logo_bits[] = {
  0x7E, 0x81, 0xB9, 0xA9,
  0xB9, 0x91, 0xB9, 0x91,
  0x91, 0xB9, 0x91, 0xB9,
  0xA9, 0xB9, 0x81, 0x7E};
*/
// ..and paste them into an editor, save file as "ottologo.xbm"
// Open "ottologo.xbm in grafic program (GIMP) - funny ?!
void logoE() {
  Eyes.clear();
  Eyes.drawBitmap(0, 0, logo_bits , 8, 16, LED_ON);
  Eyes.writeDisplay();
}
#endif

//----------------------------------------------------------------- //
//----------------------------------------------------------------- //
#if OTTO_BTAPP
#include <SoftwareSerial.h>
SoftwareSerial BTserial = SoftwareSerial(PIN_BLE_RX,PIN_BLE_RX);
#endif

#if OTTO_FLYS
#error No Wings attached
#endif
// -------------------------------------------------------------------------------- //
//                                                                     OTTO SETUP()
// -------------------------------------------------------------------------------- //
void Otto_Start() {
  // Init Serial port
#if (BAUD != 0)
  Serial.begin(BAUD);
#endif

// init Otto; Set the servo pins and ultrasonic pins and Buzzer pin
//#if (OTTO == H) doesn't work
#if OTTO_ARMS && OTTO_HEAD
  Otto.initHUMANOID(PIN_LL, PIN_RL, PIN_LF, PIN_RF, PIN_LA, PIN_RA, true, PIN_Noise, PIN_Buzzer, PIN_UStrigg, PIN_USecho);
//
#elif OTTO_ARMS
  Otto.initHUMANOID(PIN_LL, PIN_RL, PIN_LF, PIN_RF, PIN_LA, PIN_RA, true, PIN_Noise, PIN_Buzzer, PIN_UStrigg, PIN_USecho);
//
#else
  Otto.init(PIN_LL, PIN_RL, PIN_LF, PIN_RF, true, PIN_Noise, PIN_Buzzer, PIN_UStrigg, PIN_USecho);
#endif
//
#if OTTO_MOUTH
  Otto.initMATRIX( PIN_DIN, PIN_CS, PIN_CLK, LED_DIR);
#endif
//
#if OTTO_EYES
  Eyes.begin(0x70);          // pass in the TWI address
  Eyes.setRotation(0);
  Eyes.clear();
  Eyes.setBrightness(8);
  logoE();
#endif
  Otto.home();
} // Otto_Start()
//
#endif // Otto_config_h
