//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY APP Firmware Version 11 (V11) with standard baudrate of 9600 for Bluetooth BLE modules.
//-- Designed to work with the Otto Eyes 
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//-------------------------------------------------------------------------
#include <SerialCommand.h>//-- Library to manage serial commands
SoftwareSerial BTserial = SoftwareSerial(11,12); //  TX  RX of the Bluetooth
SerialCommand SCmd(BTserial);  //The SerialCommand object
#include <Otto9.h>
Otto9 Otto;  //This is Otto!
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16matrix ematrix = Adafruit_8x16matrix(); //I2C 16x8 led matrix
// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
// SERVO ASSEMBLY PIN   /////////////////////////////////////////////////////////////////////
// to help assemble Otto's feet and legs - wire link between pin 10 and GND
#define PIN_ASSEMBLY    10   //ASSEMBLY pin (10) LOW = assembly    HIGH  = normal operation
// BUZZER PIN //////////////////////////////////////////////////////////////////////////////
#define PIN_Buzzer  13 //BUZZER pin (13)
// SOUND SENSOR PIN //////////////////////////////////////////////////////////////////////////
#define PIN_NoiseSensor A6  //SOUND SENSOR   ANALOG pin (A6)
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
// TOUCH SENSOR or PUSH BUTTON /////////////////////////////////////////////////////////////////
#define PIN_Button   A0 // TOUCH SENSOR Pin (A0) PULL DOWN RESISTOR MAYBE REQUIRED to stop false interrupts (interrupt PIN)
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
const char programID[] = "OttoE_APP_V11"; //Each program will have a ID
//-- Movement parameters
int T = 1000;            //Initial duration of movement
int moveId = 0;          //Number of movement
int moveSize = 15;       //Asociated with the height of some movements
static const uint8_t PROGMEM
logo_bmp[] = {  B01111110,B10000001,B10111001,B10101001,B10111001,B10010001,B10111001,B10010001,B10010001,B10111001,B10010001,B10111001,B10101001,B10111001,B10000001,B01111110},
happy_bmp[] = {  B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000,B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000},
eyes_bmp[] = {  B00000000,B00111100,B01000010,B01001010,B01000010,B01000010,B00111100,B00000000,B00000000,B00111100,B01000010,B01001010,B01000010,B01000010,B00111100,B00000000},
sad_bmp[] =  {  B00000000,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000,B00000000,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000},
xx_bmp[] =  {  B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000},
XX_bmp[] = {  B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001,B00000000,B00000000,B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001},
angry2_bmp[] = {  B00000000,B00000010,B00000100,B00001000,B00010000,B00100000,B00000000,B00000000,B00000000,B00000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000000},
sleep_bmp[] = {  B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000},
freetful_bmp[] = {  B00000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000000,B00000000,B00000000,B00000000,B00000010,B00000100,B00001000,B00010000,B00100000,B00000000},
love_bmp[] = {  B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000,B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000},
confused_bmp[] = {  B00000000,B01111100,B10000010,B10111010,B10101010,B10001010,B01111000,B00000000,B00000000,B01111100,B10000010,B10111010,B10101010,B10001010,B01111000,B00000000},
wave_bmp[] = {  B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000,B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000},
magic_bmp[] = {  B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000},
fail_bmp[] = {  B00000000,B00110000,B01111000,B01111000,B01111100,B00111100,B00001000,B00000000,B00000000,B00001000,B00111100,B01111100,B01111000,B01111000,B00110000,B00000000};

///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  //Serial communication initialization
  Serial.begin(9600);
  BTserial.begin(9600);  
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins
  ematrix.begin(0x70);  // pass in the address
  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  //SCmd.addCommand("L", receiveLED);       //  sendAck & sendFinalAck
  SCmd.addCommand("M", receiveMovement);  //  sendAck & sendFinalAck
  SCmd.addCommand("H", receiveGesture);   //  sendAck & sendFinalAck
  SCmd.addCommand("K", receiveSing);      //  sendAck & sendFinalAck
  SCmd.addDefaultHandler(receiveStop);

  Otto.sing(S_connection);
  Otto.home();
  Otto.sing(S_happy);
  ematrix.drawBitmap(0, 0, + logo_bmp , 8, 16, LED_ON);
  ematrix.writeDisplay();
  delay(200);
  ematrix.clear();
  ematrix.drawBitmap(0, 0, + eyes_bmp , 8, 16, LED_ON);
  ematrix.writeDisplay();
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
        SCmd.readSerial();    //If Otto is moving yet
        if (Otto.getRestState()==false){  
          move(moveId);
        }  
    }
///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////
//-- Function to receive Stop command.
void receiveStop() {
  sendAck();
  Otto.home();
  sendFinalAck();
}

//-- Function to receive LED commands
void receiveLED() {

  //sendAck & stop if necessary
  sendAck();
  Otto.home();
  unsigned long int matrix;
  char *arg;
  char *endstr;
  arg = SCmd.next(); //Serial.println (arg);
  if (arg != NULL) {
    matrix = strtoul(arg, &endstr, 2); // Converts a char string to unsigned long integer
    Otto.putMouth(matrix, false);
  }
  else {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();
  }
  sendFinalAck();
}

//-- Function to receive movement commands
void receiveMovement() {
  sendAck();
  if (Otto.getRestState() == true) Otto.setRestState(false);

  //Definition of Movement Bluetooth commands
  //M  MoveID  T   MoveSize
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) moveId = atoi(arg);
  else {
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();
    moveId = 0; //stop
  }
  arg = SCmd.next();
  if (arg != NULL) T = atoi(arg);
  else T = 1000;
  arg = SCmd.next();
  if (arg != NULL) moveSize = atoi(arg);
  else moveSize = 15;
}

//-- Function to execute the right movement according the movement command received.
void move(int moveId) {
  bool manualMode = false;

  switch (moveId) {
    case 0:
      Otto.home();
      break;
    case 1: //M 1 1000
      Otto.walk(1, T, 1);
      break;
    case 2: //M 2 1000
      Otto.walk(1, T, -1);
      break;
    case 3: //M 3 1000
      Otto.turn(1, T, 1);
      break;
    case 4: //M 4 1000
      Otto.turn(1, T, -1);
      break;
    case 5: //M 5 1000 30
      Otto.updown(1, T, moveSize);
      break;
    case 6: //M 6 1000 30
      Otto.moonwalker(1, T, moveSize, 1);
      break;
    case 7: //M 7 1000 30
      Otto.moonwalker(1, T, moveSize, -1);
      break;
    case 8: //M 8 1000 30
      Otto.swing(1, T, moveSize);
      break;
    case 9: //M 9 1000 30
      Otto.crusaito(1, T, moveSize, 1);
      break;
    case 10: //M 10 1000 30
      Otto.crusaito(1, T, moveSize, -1);
      break;
    case 11: //M 11 1000
      Otto.jump(1, T);
      break;
    case 12: //M 12 1000 30
      Otto.flapping(1, T, moveSize, 1);
      break;
    case 13: //M 13 1000 30
      Otto.flapping(1, T, moveSize, -1);
      break;
    case 14: //M 14 1000 20
      Otto.tiptoeSwing(1, T, moveSize);
      break;
    case 15: //M 15 500
      Otto.bend(1, T, 1);
      break;
    case 16: //M 16 500
      Otto.bend(1, T, -1);
      break;
    case 17: //M 17 500
      Otto.shakeLeg(1, T, 1);
      break;
    case 18: //M 18 500
      Otto.shakeLeg(1, T, -1);
      break;
    case 19: //M 19 500 20
      Otto.jitter(1, T, moveSize);
      break;
    case 20: //M 20 500 15
      Otto.ascendingTurn(1, T, moveSize);
      break;
    default:
      manualMode = true;
      break;
  }
  if (!manualMode) sendFinalAck();
}

//-- Function to receive gesture commands
void receiveGesture() {
  //sendAck & stop if necessary
  sendAck();
  Otto.home();

  //Definition of Gesture Bluetooth commands
  //H  GestureID
  int gesture = 0;
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) gesture = atoi(arg);
  else     delay(2000); //Otto.putMouth(xMouth); //Otto.clearMouth();

   switch (gesture) {
      case 1: //H 1 
        ematrix.clear(); ematrix.drawBitmap(0, 0, + happy_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoHappy);
        ematrix.clear(); ematrix.drawBitmap(0, 0, + eyes_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        break;
      case 2: //H 2 
        ematrix.clear(); ematrix.drawBitmap(0, 0, + happy_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoSuperHappy);
        ematrix.clear(); ematrix.drawBitmap(0, 0, + happy_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        break;
      case 3: //H 3 
        ematrix.clear(); ematrix.drawBitmap(0, 0, + sad_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoSad);
        ematrix.clear(); ematrix.drawBitmap(0, 0, + eyes_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        break;
      case 4: //H 4 
        ematrix.clear(); ematrix.drawBitmap(0, 0, + sleep_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoSleeping);
        break;
      case 5: //H 5  
        ematrix.clear(); ematrix.drawBitmap(0, 0, + xx_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoFart);
        break;
      case 6: //H 6 
        ematrix.clear(); ematrix.drawBitmap(0, 0, + confused_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoConfused);
        break;
      case 7: //H 7 
        ematrix.clear(); ematrix.drawBitmap(0, 0, + love_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoLove);
        break;
      case 8: //H 8 
        Otto.playGesture(OttoAngry);
        ematrix.clear(); ematrix.drawBitmap(0, 0, + angry2_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        break;
      case 9: //H 9  
        ematrix.clear(); ematrix.drawBitmap(0, 0, + freetful_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoFretful);
        break;
      case 10: //H 10
        ematrix.clear(); ematrix.drawBitmap(0, 0, + magic_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoMagic);
        break;  
      case 11: //H 11
        ematrix.clear(); ematrix.drawBitmap(0, 0, + wave_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoWave);
        break;   
      case 12: //H 12
        ematrix.clear(); ematrix.drawBitmap(0, 0, + magic_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoVictory);
        break; 
      case 13: //H 13
        ematrix.clear(); ematrix.drawBitmap(0, 0, + fail_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        Otto.playGesture(OttoFail);
        ematrix.clear(); ematrix.drawBitmap(0, 0, + XX_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
        break;         
      default:
        break;
    }
  sendFinalAck();
}

//-- Function to receive sing commands
void receiveSing() {
  //sendAck & stop if necessary
  sendAck();
  Otto.home();

  //Definition of Sing Bluetooth commands
  //K  SingID
  int sing = 0;
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) sing = atoi(arg);
  else     delay(2000); // Otto.putMouth(xMouth); //Otto.clearMouth();

  switch (sing) {
    case 1: //K 1
      Otto.sing(S_connection);
      break;
    case 2: //K 2
      Otto.sing(S_disconnection);
      break;
    case 3: //K 3
      Otto.sing(S_surprise);
      break;
    case 4: //K 4
      Otto.sing(S_OhOoh);
      break;
    case 5: //K 5
      Otto.sing(S_OhOoh2);
      break;
    case 6: //K 6
      Otto.sing(S_cuddly);
      break;
    case 7: //K 7
      Otto.sing(S_sleeping);
      break;
    case 8: //K 8
      Otto.sing(S_happy);
      break;
    case 9: //K 9
      Otto.sing(S_superHappy);
      break;
    case 10: //K 10
      Otto.sing(S_happy_short);
      break;
    case 11: //K 11
      Otto.sing(S_sad);
      break;
    case 12: //K 12
      Otto.sing(S_confused);
      break;
    case 13: //K 13
      Otto.sing(S_fart1);
      break;
    case 14: //K 14
      Otto.sing(S_fart2);
      break;
    case 15: //K 15
      Otto.sing(S_fart3);
      break;
    case 16: //K 16
      Otto.sing(S_mode1);
      break;
    case 17: //K 17
      Otto.sing(S_mode2);
      break;
    case 18: //K 18
      Otto.sing(S_mode3);
      break;
    case 19: //K 19
      Otto.sing(S_buttonPushed);
      break;
    default:
      break;
  }
  sendFinalAck();
}

//-- Function to send Ack comand (A)
void sendAck() {
  delay(30);
  Serial.print(F("&&"));
  Serial.print(F("A"));
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send final Ack comand (F)
void sendFinalAck() {
  delay(30);
  Serial.print(F("&&"));
  Serial.print(F("F"));
  Serial.println(F("%%"));
  Serial.flush();
}
