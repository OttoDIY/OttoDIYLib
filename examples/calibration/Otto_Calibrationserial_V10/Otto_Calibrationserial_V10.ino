//----------------------------------------------------------------
//-- Otto Calibration Serial for all Ottos
//
//-- With this code you can calibrate the servo motors
//-- but it will require to adjust the TRIM values
//-- April 2019: Designed to work with the basic Otto but could be compatible with PLUS or other biped robots
//-- September 2019: Added Interactive interface
//-- January 2021: (Birger T) added some #defines, commands and code for Otto9Humanoid,
/******************************************************************************************************
  Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
  Otto DIY invests time and resources providing open source code and hardware,
  please support by purchasing kits from (https://www.ottodiy.com)

  BSD license, all text above must be included in any redistribution
 *******************************************************************/
//-- VERY IMPORTANT only calibrate ONE TIME per board! to avoid damage EEPROM memory
//-----------------------------------------------------------------
/*  The interface uses single character controls, see the help() output
    Serial.print(F(
                 "This Sketch is used to set the trims on the servos for the biped 'Otto's\n"
                 "The interface uses single character controls\n\n"
                 "'L' - select the Left Leg\n"
                 "'l' - select the Left Foot\n"
                 "'R' - select the Right Leg\n"
                 "'r' - select the Right Foot\n"
#if (OTTO == H)
                 "'a' - select the Left Arm\n"
                 "'A' - select the Right Arm\n"
#endif
                 "\n'e' - read EEPROM trims and use them\n"
                 "'+' - add 1 to the trim of selected\n"
                 "'-' - sub 1 from the trim of selected\n"
                 "'w' - write the current trims to  EEPROM\n\n"
                 "'*' - add 10 to the pos of selected\n"
                 "'#' - sub 10 from the pos of selected\n"
                 "'_' - set all to home positions\n\n"
                 "'h' - this message\n\n"
               ));
               
   You can also enter multiple commands at once, like "++++" 
   to increase the trim value of the selected "device" by 4;
   or try these (copy the line, and paste it into the Serial monitor [CTRL]+[V])
_ a### A*** a### A*** a### A***
   the undocumented space char ' ' inserts a delay(100), so you can
   do some movements but remember, the serial input buffer is limited.
   after doing the line above, copy the next line and paste and send it repeatly (3x)
l*** r### a*** A### a*** A### 
   then repeat the next line (6x) and the again the line above
l### r*** a### A*** a### A***
    Have fun, Bye Bye..
_a # # # # * * * * * * * * # # # # # # # _
*/
// ---------------------- Calibration Setup ----------------------- //
//
// ?????????????????  which Otto do you want to calibrate??????????
//
// B = Basic, C = Lasercut, E = Eyes, P = Plus, H = Humanoid,
// (L = Lee not implemented yet)
#define OTTO H
// Set the baudrate for serial communication (set also in serial monitor)
#define BAUD  9600  //  9600 = default speed
//#define BAUD 19200  // 19200 = double default speed
//#define BAUD 38400  // 38400 = double double default speed
//#define BAUD 57600  // 57600 = double double and a half double double default speed ;-)
//
// ------------------  End Of Calibration Setup ------------------- //
//

#if (OTTO == H)
#include <Otto9Humanoid.h>
#define SERVOCNT 6      // Count of servo variables needed
Otto9Humanoid Otto;     // Create an Otto object
#else
#include <Otto9.h>
#define SERVOCNT 4      // Count of servo variables needed
Otto9 Otto;             // Create an Otto object
#endif

// ================================ Global Variables
int16_t positions[SERVOCNT]; //  = {90,..,90};
int8_t  trims[SERVOCNT];     //  = {0,..,0};
int16_t trimpos[SERVOCNT];   //  = trims[] + positions[]
uint8_t servo = 0;           // selected servo (0 == left leg)
#define PRINTDELAY 200       // ms to wait between print() executions
uint16_t printtimer = 0;      // disables print(), if to many commands
uint16_t millisold = 0;
uint16_t loopduration = 0;
//
// ================================ Functions
//
#define setprinttimer() printtimer = PRINTDELAY
//void setprinttimer() {
//    printtimer = PRINTDELAY;
//}

void setTrims() { // Set and print all trim values

#if (OTTO == H)
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


void help() {
  Otto.home();
  if (!printtimer) {
    setprinttimer();
    Serial.print(F(
                 "This Sketch is used to set the trims on the servos for the biped 'Otto's\n"
                 "The interface uses single character controls\n\n"
                 "'L' - select the Left Leg\n"
                 "'l' - select the Left Foot\n"
                 "'R' - select the Right Leg\n"
                 "'r' - select the Right Foot\n"
#if (OTTO == H)
                 "'a' - select the Left Arm\n"
                 "'A' - select the Right Arm\n"
#endif
                 "\n'e' - read EEPROM trims and use them\n"
                 "'+' - add 1 to the trim of selected\n"
                 "'-' - sub 1 from the trim of selected\n"
                 "'w' - write the current trims to  EEPROM\n\n"
                 "'*' - add 10 to the pos of selected\n"
                 "'#' - sub 10 from the pos of selected\n"
                 "'_' - set all to home positions\n\n"
                 "'h' - this message\n\n"
               ));
  } // if (!printtimer)
} // void help()

void processChar(char c) { // ============= process the user input char command

  switch (c) {
    case 'h':
    case 'H':
      help();
      break;
    case 'L':
      Serial.println(F("Selected Left Leg"));
      servo = 0;
      break;
    case 'l':
      Serial.println(F("Selected Left Foot"));
      servo = 2;
      break;
    case 'R':
      Serial.println(F("Selected Right Leg"));
      servo = 1;
      break;
    case 'r':
      Serial.println(F("Selected Right Foot"));
      servo = 3;
      break;
#if (OTTO == H)
    case 'a':
      Serial.println(F("Selected Left Arm"));
      servo = 4;
      break;
    case 'A':
      Serial.println(F("Selected Right Arm"));
      servo = 5;
      break;
#else
      Serial.println(F("no arms - no cookies!"));
#endif
    case '+':
      trims[servo]++;
      setTrims();
      break;
    case '-':
      trims[servo]--;
      setTrims();
      break;
    case 'e':
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
      Otto.saveTrimsOnEEPROM();
      Serial.println(F("Values written to EEPROM"));
      break;

    case '_':
      for (int i = 0; i < SERVOCNT; i++)  {
        // init the positions to home()
        positions[i] = 90;
      }
      setPosis();
      break;
    case '*':
      positions[servo] += 10;
      setPosis();
      break;
    case '#':
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

} // processChar()

void setup(void) // ==================================== init all things
{
  // Init Serial port
  Serial.begin(BAUD);

  // init Otto; Set the servo pins and ultrasonic pins and Buzzer pin
#if (OTTO == H)
  Otto.initHUMANOID(2, 3, 4, 5, 6, 7, true, A6, 13, 8, 9);
#else
  Otto.init(2, 3, 4, 5, true, A6, 13, 8, 9);
#endif

  help();

  // read the EEPROM and use those values
  for (int i = 0; i < SERVOCNT; i++)  {
    int servo_trim = EEPROM.read(i);
    if (servo_trim > 128)
      servo_trim -= 255;
    trims[i] = servo_trim;
    // init the positions too
    positions[i] = 90;
  }
  Serial.println(F("Getting EEPROM Trim values"));
  // set the trims
  setTrims();

  //  Otto._moveServos(10, positions);
  Otto.home();

  Serial.println(F("\nSelected Left Leg")); // servo == 0 == left leg
  millisold = (uint16_t)millis();
} // setup()

void loop() {  // ======================== see if the user wants anything
  // we won't mess aroung with unsigned long or uint32_t
  uint16_t millisnow = (uint16_t)millis();
  loopduration = millisnow - millisold;
  millisold = millisnow;
  // countdown of printtimer
  if(printtimer > loopduration) {
    printtimer -= loopduration;
  }
  else {
    printtimer = 0;
  } // if(printtimer..)
  
  //  Otto.home();
  // get input from Serial port
  while (Serial.available() > 0)  {
    processChar(Serial.read());
  }
  
} // loop()
