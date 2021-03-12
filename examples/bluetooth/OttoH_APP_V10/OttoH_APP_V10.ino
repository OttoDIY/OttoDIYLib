//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY APP Firmware version 10 (V10) with standard baudrate of 9600 for Bluetooth modules.
//-- This code will have all modes and functions therefore memory is almost full but ignore the alert it works perfectly.
//-- Designed to work with the basic Otto or PLUS or Humanoid or other biped robots. some of these functions will need a good power source such as a LIPO battery.
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -- ADDED Progmem for MOUTHS and GESTURES: Paul Van De Veen October 2018
// -- ADDED PIN definitions for ease of use: Jason Snow November 2018
// -- ADDED Eye Matrix Progmem and control: Jason Snow November 2018
// -- REMOVED Eye Matrix Progmem and control: Jason Snow AUG 2019
// -- ADDED Battery meassurementin in mode 3 Jason Snow August 2019
// -- ADDED changed to original Software serial library Camilo Parra May 2020
// -- DELETED interrupts and modes to use BTserial Camilo Parra May 2020
// -- Fixed resets because of the code Javier Santos-Olmo  May 2020
//-------------------------------------------------------------------------
#include <EEPROM.h>
#include <SerialCommand.h>//-- Library to manage serial commands
SoftwareSerial BTserial = SoftwareSerial(11,12); //  TX  RX of the Bluetooth
SerialCommand SCmd(BTserial);  //The SerialCommand object
#include <Otto9Humanoid.h>
Otto9Humanoid Otto;  //This is Otto!

// SERVO PINs //////////////////////////////////////////////////////////////////////////////
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_LA 6 //servo[4]  Left arm if enabled
#define PIN_RA 7 //servo[5]  Right arm if enabled
// ULTRASONIC PINs /////////////////////////////////////////////////////////////////////////
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
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
// SERVO ASSEMBLY PIN   /////////////////////////////////////////////////////////////////////
// to help assemble Otto's feet and legs - wire link between pin 10 and GND
#define PIN_ASSEMBLY    10   //ASSEMBLY pin (10) LOW = assembly    HIGH  = normal operation
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
//-- Movement parameters
int T = 1000;            //Initial duration of movement
int moveId = 0;          //Number of movement
int moveSize = 15;       //Asociated with the height of some movements
volatile bool buttonPushed=false;  //Variable to remember when a button has been pushed
bool obstacleDetected = false;
unsigned long int matrix;
unsigned long timerMillis = 0;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  //Serial communication initialization
  Serial.begin(9600);
  BTserial.begin(9600);  
  Otto.initHUMANOID(PIN_YL, PIN_YR, PIN_RL, PIN_RR, PIN_LA, PIN_RA, true, PIN_NoiseSensor, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
  Otto.matrixIntensity(1);// set up Matrix display intensity
//  Otto.initBatLevel(PIN_Battery);// set up Battery percent read pin - MUST BE AN ANALOG PIN
  randomSeed(analogRead(PIN_NoiseSensor));   //Set a random seed
  pinMode(PIN_ASSEMBLY,INPUT_PULLUP); // - Easy assembly pin - LOW is assembly Mode
  pinMode(PIN_Button,INPUT); // - ensure pull-down resistors are used

  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  SCmd.addCommand("L", receiveLED);       //  sendAck & sendFinalAck
  SCmd.addCommand("M", receiveMovement);  //  sendAck & sendFinalAck
  SCmd.addCommand("H", receiveGesture);   //  sendAck & sendFinalAck
  SCmd.addCommand("K", receiveSing);      //  sendAck & sendFinalAck
  SCmd.addCommand("D", requestDistance);
  SCmd.addDefaultHandler(receiveStop);
  
  //Otto wake up!
  Otto.sing(S_connection);
  Otto.home();
  // Animation Uuuuuh - A little moment of initial surprise
  //-----
  for (int i = 0; i < 2; i++) {
    for (int i = 0; i < 8; i++) {
      Otto.putAnimationMouth(littleUuh, i);
      delay(150);
    }
  }
  //Smile for a happy Otto :)
  Otto.putMouth(smile);
  Otto.sing(S_happy);
  delay(200);
  Otto.putMouth(happyOpen);
// if Pin assembly is LOW then place OTTO's servos in home mode to enable easy assembly, 
// when you have finished assembling Otto, remove the link in pin assembly
  while (digitalRead(PIN_ASSEMBLY) == LOW) {
    Otto.home();
    Otto.sing(S_happy_short);   // sing every 5 seconds so we know OTTO is still working
    delay(5000);
  }
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

//-- Function to read distance sensor & to actualize obstacleDetected variable
void obstacleDetector() {
  int distance = Otto.getDistance();
  if (distance < 15) obstacleDetected = true;
  else obstacleDetected = false;
}

//-- Function to receive Stop command.
void receiveStop() {
  sendAck();
  Otto.home();
  sendFinalAck();
}

//-- Function to send ultrasonic sensor measure (distance in "cm")
void requestDistance() {
  Otto.home();  //stop if necessary
  int distance = Otto.getDistance();
  Serial.print(F("&&"));
  Serial.print(F("D "));
  Serial.print(distance);
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send noise sensor measure
void requestNoise() {
  Otto.home();  //stop if necessary
  int microphone = Otto.getNoise(); //analogRead(PIN_NoiseSensor);
  Serial.print(F("&&"));
  Serial.print(F("N "));
  Serial.print(microphone);
  Serial.println(F("%%"));
  Serial.flush();
}

////-- Function to receive LED commands
void receiveLED() {

  //sendAck & stop if necessary
  sendAck();
  Otto.home();
//  //Examples of receiveLED Bluetooth commands
  //L 000000001000010100100011000000000
  unsigned long int matrix;
  char *arg;
  char *endstr;
  arg = SCmd.next();
  //Serial.println (arg);
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
    case 21: //M 21
      Otto.handsup();
      break;
    case 22: //M 22 right arm
      Otto.handwave(1);
      break;
    case 23: //M 23 left arm
      Otto.handwave(-1);
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
      Otto.playGesture(OttoHappy);
      break;
    case 2: //H 2
      Otto.playGesture(OttoSuperHappy);
      break;
    case 3: //H 3
      Otto.playGesture(OttoSad);
      break;
    case 4: //H 4
      Otto.playGesture(OttoSleeping);
      break;
    case 5: //H 5
      Otto.playGesture(OttoFart);
      break;
    case 6: //H 6
      Otto.playGesture(OttoConfused);
      break;
    case 7: //H 7
      Otto.playGesture(OttoLove);
      break;
    case 8: //H 8
      Otto.playGesture(OttoAngry);
      break;
    case 9: //H 9
      Otto.playGesture(OttoFretful);
      break;
    case 10: //H 10
      Otto.playGesture(OttoMagic);
      break;
    case 11: //H 11
      Otto.playGesture(OttoWave);
      break;
    case 12: //H 12
      Otto.playGesture(OttoVictory);
      break;
    case 13: //H 13
      Otto.playGesture(OttoFail);
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

//-- Function to receive Name command
void receiveName() {
  //sendAck & stop if necessary
  sendAck();
  Otto.home();
  char newOttoName[11] = "";  //Variable to store data read from Serial.
  int eeAddress = 5;          //Location we want the data to be in EEPROM.
  char *arg;
  arg = SCmd.next();
  if (arg != NULL) {
    //Complete newOttoName char string
    int k = 0;
    while ((*arg) && (k < 11)) {
      newOttoName[k] = *arg++;
      k++;
    }
    EEPROM.put(eeAddress, newOttoName);
  }
  else
  {
    //Otto.putMouth(xMouth);
    delay(2000);
    //Otto.clearMouth();
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

//-- Function executed when  button is pushed / Touch sensor VIA INTERRUPT routine
void ButtonPushed(){ 
    if(!buttonPushed){
        buttonPushed=true;
        Otto.putMouth(smallSurprise);
    } 
} 
