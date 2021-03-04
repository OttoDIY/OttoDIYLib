//----------------------------------------------------------------
//-- Otto Easy Calibration Serial
//
//-- With this code you can calibrate the servo motors
//-- but it will require to adjust the TRIM values
//-- April 2019: Designed to work with the basic Otto but could be compatible with PLUS or other biped robots
//-- September 2019: Added Interactive interface
//-- January 2021: (Birger T) added some #defines, commands and code for Otto9Humanoid,
//-- February 2021: (Birger T) fixed some #defines, added code for Robot Lee and Testing functions
/******************************************************************************************************
  Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
  Otto DIY invests time and resources providing open source code and hardware,
  please support by purchasing kits from (https://www.ottodiy.com)

  BSD license, all text above must be included in any redistribution
 *******************************************************************/
/*  The interface uses single character controls, see the help() function / output

   You can also enter multiple commands at once, like "++++"
   to increase the trim value of the selected "device" by 4;
*/
// ---------------------- Calibration Sketch & Pins Setup --------- //
//
#define OTTO_ARMS   true    // true for Humanoid and Robot Lee only ?!
#define OTTO_HEAD   true    // true for Robot Lee only ?!
// Set the baudrate for serial communication (set also in serial monitor)
#define BAUD  9600  //  9600 = default speed
//#define BAUD 19200  // 19200 = double default speed
//#define BAUD 38400  // 38400 = double double default speed
//#define BAUD 57600  // 57600 = double double and a half double double default speed ;-)
//
#define OTTO_OPINS  true   // true = Use the Otto Original Pin setup
//                  false = For an User Setup with rebuild pin connections
//                  US Sensor echo and trigger connected at one Pin 10;
//                  Leg servos moved from pins 2 and 3 to pins 8 and 9;
//                  Touch sensor connected to Interrupt pin 2;
//                  Noise sensor connected to Interrupt pin 3;

#if OTTO_OPINS  // the standard PIN Layout as described in the Otto Humanoid Docs
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
#define PIN_Head    10  // Servo[6] turn head
#define PIN_BLE_RX  11  // softSerial RX = BLE Modul TX
#define PIN_BLE_TX  12  // softSerial TX = BLE Modul RX
#define PIN_Buzzer  13  // Buzzer
#define PIN_Button  A0  // Touch sensor
#define PIN_CLK     A1  // Mouth 8x8 LED matrix
#define PIN_CS      A2  // Mouth 8x8 LED matrix
#define PIN_DIN     A3  // Mouth 8x8 LED matrix
//                  A4  // TWI/I2C SDA (Eyes matrix)
//                  A5  // TWI/I2C SCL (Eyes matrix)
#define PIN_Noise   A6  // Noise sensor
#define PIN_Batt    A7  // Reading the Batterie Level
//----------------------------------------------------------------- //
#else  // recommended alternative PIN Layout by Birger T:
// -- to always catch the Touchsensor's and the Noisesensor's triggers
//    they must be attached to external Interrupt Pins 2 & 3
// -- the Ultrasound Sensor HC-SR04 can be driven by one MCU Pin only
//    connect trigger and echo pins together (Y-wire) and call
//    Otto_getDistance() instead of Otto.getDistance()
//
//                  0   // IDE Serial Monitor RX
//                  1   // IDE Serial Monitor TX
#define PIN_Button  2   // Touch sensor
#define PIN_Noise   3   // Noise sensor
#define PIN_Head    3   // Servo[6] turn head
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
// -------------------------  End Of Pin Setup ------------------- //

#if OTTO_ARMS && OTTO_HEAD  // Robot Lee
#include <Otto9Humanoid.h>
#include "Otto9_LeeAddOn.h"
#define SERVOCNT 7      // Count of servo variables needed
Otto9Humanoid Otto;     // Create an Otto object
//
#elif OTTO_ARMS             // Otto Humanoid
#include <Otto9Humanoid.h>
#define SERVOCNT 6      // Count of servo variables needed
Otto9Humanoid Otto;     // Create an Otto object
//
#else
#include <Otto9.h>
#define SERVOCNT 4      // Count of servo variables needed
Otto9 Otto;             // Create an Otto object
#endif

#include "Otto9_LibAddOn.h"

// ================================ Global Variables
int16_t positions[SERVOCNT]; //  = {90,..,90};
int8_t  trims[SERVOCNT];     //  = {0,..,0};
int16_t trimpos[SERVOCNT];   //  = trims[] + positions[]
uint8_t servo = 0;           // selected servo (0 == left leg)
#define PRINTDELAY 200       // ms to wait between print() executions
uint16_t printtimer = 0;     // disables print(), if to many commands
uint16_t millisold = 0;
uint16_t loopduration = 0;
char menuMode = 'c';         // activated functions mode 'c', 'm'
// ================================ Functions
//
#define setprinttimer() printtimer = PRINTDELAY
//void setprinttimer() {
//    printtimer = PRINTDELAY;
//}
//--------------------------------------  DISTANCE SENSOR  -------- //
//
#if (PIN_UStrigg == PIN_USecho)
// if US distance sensor's echo and trigger pins are connected
void  showDistance() {
  uint8_t dist = Otto_getDistance();
  // for displaying a 'bargraph'..
  uint8_t level = map(dist, 0, 80, 0, 40);
  Serial.print(F("Distance "));
  while (level--) {
    Serial.write('#');
  }
  Serial.write(' ');
  // ..and the measured distance
  Serial.println(dist, DEC);
} // fnDistance()
#else
// use Otto lib function with two separate pins for echo and trigger
void  showDistance() {
  uint16_t dist = Otto.getDistance();
  uint8_t level = map(dist, 0, 150, 0, 40);
  Serial.print(F("Distance "));
  while (level--) {
    Serial.write('#');
  }
  Serial.write(' ');
  Serial.println(dist, DEC);
} // fnDistance()
#endif

void setTrims() { // Set and print all trim values

#if OTTO_ARMS && OTTO_HEAD
  Otto.setTrims(trims[0], trims[1], trims[2], trims[3], trims[4], trims[5]);
  Lee_setTrim(trims[6]);
#elif OTTO_ARMS
  Otto.setTrims(trims[0], trims[1], trims[2], trims[3], trims[4], trims[5]);
#else
  Otto.setTrims(trims[0], trims[1], trims[2], trims[3]);
#endif
  if (!printtimer) {
    setprinttimer();
    Serial.print(F("Setting trim ["));
    for ( int x = 0; x < SERVOCNT ; x++) {
      Serial.print(trims[x]);
      if (x < SERVOCNT - 1) Serial.print(F(", "));
    } // for
    Serial.println(F("]"));
  } // if(!printtimer)
} // setTrims()

void setPosis() { // Constrain & print all position values

  for ( int x = 0; x < SERVOCNT ; x++) {
    if (positions[x] > 180) positions[x] = 180;
    if (positions[x] <   0) positions[x] =   0;
  }
  if (!printtimer) {
    setprinttimer();
    Serial.print(F("Setting Positions ["));
    for ( int x = 0; x < SERVOCNT ; x++) {
      Serial.print(positions[x]);
      if (x < SERVOCNT - 1) Serial.print(F(", "));
    }
    Serial.println(F("]"));
  } // if (!printtimer)
} // setPosis()

void Otto_0pos() {
  // Reset all positions
  for (int i = 0; i < SERVOCNT; i++)  {
    // init the positions to home()
    positions[i] = 90;
  }
//  setPosis();
  Otto.home();
}

void help_calib() {
  Otto_0pos();
  if (!printtimer) {
    setprinttimer();
    Serial.print(F("This Sketch is used to set the trims on the servos for the biped 'Otto's\n"
                   "The interface uses single character controls\n\n"
                   "'H'/'h' - set all to home positions & show this message\n"
                   "'D'/'d' - Ultra Sonic range measuring & showing\n\n"
                   "'M'/'m' - Switch to MOVE mode for Testing\n"
                   "Use the Num-Pad digits to select the servos as follows:\n\n"
                   "\t\tLEFT and  RIGHT  as seen from Otto's View\n"
#if OTTO_HEAD
                   "\t\t     '8' = Head\n"
#endif
#if OTTO_ARMS
                   "\t\t'7' Arms '9'\n"
#endif
                   "\t\t'4' Legs '6'\n"
                   "\t\t'1' Feet '3'\n\n"
                   "'+' - add 1 to the trim of selected servo\n"
                   "'-' - sub 1 from the trim of selected servo\n"
                   "'r' - read EEPROM trims and use them\n"
                   "'w' - write the current trims to  EEPROM\n"
                   "'*' - add 10 to the pos of selected Servo\n"
                   "'_' - sub 10 from the pos of selected Servo\n\n"
                  ));
  } // if (!printtimer)
} // void help_calib()
//
void help_moves() {
  Otto_0pos();
  if (!printtimer) {
    setprinttimer();
    Serial.print(F("This Sketch is used to test the trims and the servos for the biped 'Otto's\n"
                   "'H'/'h' - set all to home positions & show this message\n"
                   "'D'/'d' - Ultra Sonic range measuring & showing\n\n"
                   "'C'/'c' - Switch to CALIBRATION mode to set the trims\n\n"
                   "Use the Num-Pad digits to select the moves as follows:\n\n"
                   "\t\tLEFT and  RIGHT  as seen from Otto's View\n"
#if OTTO_HEAD
                   "\t\t      ^     Head's\n"
                   "\t\t'7'< '8' >'9' Directions\n\n"
#endif
#if OTTO_ARMS
                   "\t\t'4' Wink  '6'\n"
#endif
                   "\t\t      ^\n"
                   "\t\t     '5'   Moves &\n"
                   "\t\t'1'< '2' >'3'  Turns\n"
                   "\t\t      v    Step by Step\n\n"
                   "Send multiple digits at once:"
                   "'555122' = Steps 3x forward, 1x left and 2x back\n\n"
                  ));
  } // if (!printtimer)
} // void help_moves()

void processChar(char c) { // ============= process the user input char command

  if (menuMode == 'c') {
    switch (c) {
      case 'm':
      case 'M':
        menuMode = 'm';
        Serial.println(F("\n\n\n\t\tM O V E T E S T - M O D E"));
        help_moves();
        break;
      case 'h':
      case 'H':
        help_calib();
        break;
      case 'd':
      case 'D':
        showDistance();
        break;
      case '4':
        Serial.println(F("Selected Left Leg"));
        servo = 0;
        break;
      case '1':
        Serial.println(F("Selected Left Foot"));
        servo = 2;
        break;
      case '6':
        Serial.println(F("Selected Right Leg"));
        servo = 1;
        break;
      case '3':
        Serial.println(F("Selected Right Foot"));
        servo = 3;
        break;
#if OTTO_ARMS
      case '7':
        Serial.println(F("Selected Left Arm"));
        servo = 4;
        break;
      case '9':
        Serial.println(F("Selected Right Arm"));
        servo = 5;
        break;
#else
      case '7':
      case '9':
        Serial.println(F("no arms - no cookies!"));
        break;
        Serial.println(F("no arms - no cookies!"));
#endif
#if OTTO_HEAD
      case '8':
        Serial.println(F("Selected Head"));
        servo = 6;
        break;
#endif
      case '+':
        trims[servo]++;
        setTrims();
        break;
      case '-':
        trims[servo]--;
        setTrims();
        break;
      case 'r':
      case 'R':
        for (int i = 0; i < SERVOCNT; i++)
        {
          int servo_trim = EEPROM.read(i);
          if (servo_trim > 128)
            servo_trim -= 255;
          trims[i] = servo_trim;
        }
        setTrims();
        break;
      case 'w':
      case 'W':
        // write the values out to EEPROM
        //      Otto.saveTrimsOnEEPROM();
        for (int i = 0; i < SERVOCNT; i++) {
          EEPROM.write(i, trims[i]);
        }
        Serial.println(F("Values written to EEPROM"));
        break;
      case '0':
        Otto_0pos();
        break;
      case '*':
        positions[servo] += 10;
        setPosis();
        break;
      case '_':
        positions[servo] -= 10;
        setPosis();
        break;
      case ' ':
        delay(100);
        break;
      case '\n':
      case '\r':
        break;
      default:
        Serial.print(F("Unknown command ("));
        Serial.print(c);
        Serial.println(F(")"));
        break;
    }
    // _moveServos() doesn't mind the trims, it moves to the raw positions
    // Otto._moveServos(10, positions);
    //
    // so we'll add positions- and trims-values..
    for (int i = 0; i < SERVOCNT; i++)  {
      trimpos[i] = trims[i] + positions[i];
    }
    // ..and move the servos to the trimmed positions
    Otto._moveServos(10, trimpos);
#if OTTO_HEAD
    Lee_moveHead(10, trimpos[6]);
#endif
  } // if(menuMode == 'c')
  else {
    switch (c) {
      case 'c':
      case 'C':
        menuMode = 'c';
        Serial.println(F("\n\n\n\t\tC A L I B R A T I O N - M O D E"));
        help_calib();
        break;
      case 'h':
      case 'H':
        help_moves();
        break;
      case 'd':
      case 'D':
        showDistance();
        break;
      case '5':
        Serial.println(F("Step forward"));
        Otto.walk(1, 700, FORWARD);
        break;
      case '2':
        Serial.println(F("Step backward"));
        Otto.walk(1, 700, BACKWARD);
        break;
      case '1':
        Serial.println(F("Turn to the left"));
        Otto.turn(1, 700, LEFT);
        break;
      case '3':
        Serial.println(F("Turn to the right"));
        Otto.turn(1, 700, RIGHT);
        break;
#if OTTO_ARMS
      case '4':
        Serial.println(F("Left Arm wink"));
        Otto.handwave(LEFT);
        break;
      case '6':
        Serial.println(F("Right Arm wink"));
        Otto.handwave(RIGHT);
        break;
#else
      case '4':
      case '6':
        Serial.println(F("no arms - no cookies!"));
        break;
#endif
#if OTTO_HEAD
      case '7':
        Serial.println(F("Turning Head left"));
        if (positions[6] <= 170) positions[6] += 10;
        Lee_moveHead(10, positions[6]);
        break;
      case '8':
        Serial.println(F("Turning Head forward"));
        positions[6] = 90 + trims[6];
        Lee_moveHead(50, positions[6]);
        break;
      case '9':
        Serial.println(F("Turning Head right"));
        if (positions[6] >= 10) positions[6] -= 10;
        Lee_moveHead(10, positions[6]);
        break;
#endif
      case '0':
        Otto_0pos();
        break;
      case ' ':
        delay(100);
        break;
      case '\n':
      case '\r':
        break;
      default:
        Serial.print(F("Unknown command ("));
        Serial.print(c);
        Serial.println(F(")"));
        break;
    }

  } // else (menuMode == 'c')
} // processChar()

void setup(void) // ==================================== init all things
{
  // Init Serial port
  Serial.begin(BAUD);

  // init Otto; Set the servo pins and ultrasonic pins and Buzzer pin
#if OTTO_ARMS && OTTO_HEAD
  Otto.initHUMANOID(PIN_LL, PIN_RL, PIN_LF, PIN_RF, PIN_LA, PIN_RA, true, PIN_Noise, PIN_Buzzer, PIN_UStrigg, PIN_USecho);
  Lee_initHead(PIN_Head);
#elif OTTO_ARMS
  Otto.initHUMANOID(PIN_LL, PIN_RL, PIN_LF, PIN_RF, PIN_LA, PIN_RA, true, PIN_Noise, PIN_Buzzer, PIN_UStrigg, PIN_USecho);
  //  Otto.initHUMANOID(2, 3, 4, 5, 6, 7, true, A6, 13, 8, 9);
#else
  Otto.init(PIN_LL, PIN_RL, PIN_LF, PIN_RF, true, PIN_Noise, PIN_Buzzer, PIN_UStrigg, PIN_USecho);
  //  Otto.init(2, 3, 4, 5, true, A6, 13, 8, 9);
#endif

  menuMode = 'c';
  Serial.println(F("\n\n\n\t\tC A L I B R A T I O N - M O D E"));
  help_calib();

  Serial.println(F("Getting EEPROM Trim values"));
  // read the EEPROM and use those values
  for (int i = 0; i < SERVOCNT; i++)  {
    int servo_trim = EEPROM.read(i);
    if (servo_trim > 128)
      servo_trim -= 255;
    trims[i] = servo_trim;
    // init the positions too
    positions[i] = 90;
  }
  // set and show the trims
  printtimer = 0;
  setTrims();

  Otto_0pos();

  Serial.println(F("\nSelected Left Leg")); // servo == 0 == left leg
  millisold = (uint16_t)millis();
} // setup()

void loop() {  // ======================== see if the user wants anything
  // we won't mess around with unsigned long or uint32_t
  uint16_t millisnow = (uint16_t)millis();
  loopduration = millisnow - millisold;
  millisold = millisnow;
  // countdown of printtimer
  if (printtimer > loopduration) {
    printtimer -= loopduration;
  }
  else {
    printtimer = 0;
  } // if(printtimer..)

  // get input from Serial port
  while (Serial.available() > 0)  {
    processChar(Serial.read());
  }

} // loop()
