//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY Scratch AI firmware with standard baudrate of 9600 for BLE Bluetooth modules.
//-- This code have all modes and functions therefore memory is almost full but ignore the alert it works perfectly.
//-- Designed to work with Otto Eyes. 
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -- ADDED changed to original Software serial library Camilo Parra May 2020
// -- DELETED interrupts and modes to use BTserial Camilo Parra May 2020
// -- ADDED BLE communication for Scratch AI Camilo Parra August 2020 https://ottoschool.com/scratch/
//-------------------------------------------------------------------------
#include <Otto9.h>
Otto9 Otto;
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_8x16matrix ematrix = Adafruit_8x16matrix();
#include <SerialCommand.h>//-- Library to manage serial commands
SoftwareSerial BTserial = SoftwareSerial(11,12); //  TX  RX of the Bluetooth
SerialCommand SCmd(BTserial);  //The SerialCommand object
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
const char programID[]="Otto_APP_V09"; //Each program will have a ID
int T=1000;              //Initial duration of movement
int moveId=0;            //Number of movement
int moveSize=15;         //Asociated with the height of some movements
unsigned long int matrix;
static const uint8_t PROGMEM
logo_bmp[] = {  B01111110,B10000001,B10111001,B10101001,B10111001,B10010001,B10111001,B10010001,B10010001,B10111001,B10010001,B10111001,B10101001,B10111001,B10000001,B01111110},
happy_bmp[] = {  B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000,B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000},
eyes_bmp[] = {  B00000000,B00111100,B01000010,B01001010,B01000010,B01000010,B00111100,B00000000,B00000000,B00111100,B01000010,B01001010,B01000010,B01000010,B00111100,B00000000},
sad_bmp[] =  {  B00000000,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000,B00000000,B00010000,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000},
xx_bmp[] =  {  B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000},
XX_bmp[] = {  B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001,B00000000,B00000000,B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001},
angry_bmp[] = {  B00000000,B00011110,B00111100,B01111000,B01110000,B00100000,B00000000,B00000000,B00000000,B00000000,B00100000,B01110000,B01111000,B00111100,B00011110,B00000000},
angry2_bmp[] = {  B00000000,B00000010,B00000100,B00001000,B00010000,B00100000,B00000000,B00000000,B00000000,B00000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000000},
sleep_bmp[] = {  B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000},
freetful_bmp[] = {  B00000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000000,B00000000,B00000000,B00000000,B00000010,B00000100,B00001000,B00010000,B00100000,B00000000},
love_bmp[] = {  B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000,B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000},
confused_bmp[] = {  B00000000,B01111100,B10000010,B10111010,B10101010,B10001010,B01111000,B00000000,B00000000,B01111100,B10000010,B10111010,B10101010,B10001010,B01111000,B00000000},
wave_bmp[] = {  B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000,B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000},
magic_bmp[] = {  B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000},
fail_bmp[] = {  B00000000,B00110000,B01111000,B01111000,B01111100,B00111100,B00001000,B00000000,B00000000,B00001000,B00111100,B01111100,B01111000,B01111000,B00110000,B00000000},
full_bmp[] =  {   B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111 };

/*             -------- 
              |  ^  ^  |
              |--------|
  RIGHT LEG 3 |        | LEFT LEG 2
               -------- 
               ||     ||
RIGHT FOOT 5 |---     ---| LEFT FOOT 4    
*/ 
#define DIN_PIN A3
#define CS_PIN A2
#define CLK_PIN A1
#define LED_DIRECTION 1
#define PIN_YL 2 // left leg, servo[0]
#define PIN_YR 3 // right leg, servo[1]
#define PIN_RL 4 // left foot, servo[2]
#define PIN_RR 5 // right foot, servo[3]
#define PIN_Trigger 8 // ultrasound
#define PIN_Echo 9 // ultrasound
#define PIN_Buzzer  13 //buzzer
int statePin = 10;
int msToDefineConnection = 1500; // some time longer than the pulse
long highStartTimestamp;
int lastState = LOW;
bool confirmedConnected = false;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(9600);
  BTserial.begin(9600);   
  pinMode(statePin, INPUT);  
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo);
  ematrix.begin(0x70);  // pass in the address
  Otto.matrixIntensity(1);// set up Matrix display intensity
  //Setup callbacks for SerialCommand commands 
  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  SCmd.addCommand("L", receiveLED);       //  sendAck & sendFinalAck
  SCmd.addCommand("T", recieveBuzzer);    //  sendAck & sendFinalAck
  SCmd.addCommand("M", receiveMovement);  //  sendAck & sendFinalAck
  SCmd.addCommand("H", receiveGesture);   //  sendAck & sendFinalAck
  SCmd.addCommand("K", receiveSing);      //  sendAck & sendFinalAck
  SCmd.addCommand("C", receiveTrims);     //  sendAck & sendFinalAck
  SCmd.addCommand("G", receiveServo);     //  sendAck & sendFinalAck
  SCmd.addCommand("B", requestBattery);   // 
  SCmd.addCommand("I", requestProgramId);
  SCmd.addDefaultHandler(receiveStop);
  //Otto wake up!
  Otto.sing(S_connection);
  Otto.home();
  Otto.putMouth(smile);
  ematrix.drawBitmap(0, 0, + logo_bmp , 8, 16, LED_ON);
  ematrix.writeDisplay();
  delay(200);
  Otto.sing(S_happy);
  delay(200);
  Otto.putMouth(happyOpen);
  ematrix.clear();
  ematrix.drawBitmap(0, 0, + eyes_bmp , 8, 16, LED_ON);
  ematrix.writeDisplay();
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  checkIfConnected();
        SCmd.readSerial();    //If Otto is moving yet
        if (Otto.getRestState()==false){  
          move(moveId);
        }  
    }
///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////
//-- Function to receive Stop command.
void receiveStop(){
    sendAck();
    Otto.home();
    sendFinalAck();
}

//-- Function to receive LED commands
void receiveLED(){  
    sendAck();    //sendAck & stop if necessary
    Otto.home();  //Examples of receiveLED Bluetooth commands
    //L 000000001000010100100011000000000
    //L 001111111111111111111111111111111 (All LEDs)
    unsigned long int matrix;
    char *arg;
    char *endstr;
    arg=SCmd.next();
    //Serial.println (arg);
    if (arg != NULL) {
      matrix=strtoul(arg,&endstr,2);    // Converts a char string to unsigned long integer
      Otto.putMouth(matrix,false);
    }else{
      Otto.putMouth(xMouth);
      delay(2000);
      Otto.clearMouth();
    }
    sendFinalAck();
}

//-- Function to receive buzzer commands
void recieveBuzzer(){
  
    //sendAck & stop if necessary
    sendAck();
    Otto.home(); 

    bool error = false; 
    int frec;
    int duration; 
    char *arg; 
    
    arg = SCmd.next(); 
    if (arg != NULL) { frec=atoi(arg); }    // Converts a char string to an integer   
    else {error=true;}
    
    arg = SCmd.next(); 
    if (arg != NULL) { duration=atoi(arg); } // Converts a char string to an integer  
    else {error=true;}

    if(error==true){

      Otto.putMouth(xMouth);
      delay(2000);
      Otto.clearMouth();

    }else{ 

      Otto._tone(frec, duration, 1);   
    }

    sendFinalAck();

}

//-- Function to receive TRims commands
void receiveTrims(){  

    //sendAck & stop if necessary
    sendAck();
    Otto.home(); 

    int trim_YL,trim_YR,trim_RL,trim_RR;

    //Definition of Servo Bluetooth command
    //C trim_YL trim_YR trim_RL trim_RR
    //Examples of receiveTrims Bluetooth commands
    //C 20 0 -8 3
    bool error = false;
    char *arg;
    arg=SCmd.next();
    if (arg != NULL) { trim_YL=atoi(arg); }    // Converts a char string to an integer   
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { trim_YR=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { trim_RL=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { trim_RR=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}
    
    if(error==true){

      Otto.putMouth(xMouth);
      delay(2000);
      Otto.clearMouth();

    }else{ //Save it on EEPROM
      Otto.setTrims(trim_YL, trim_YR, trim_RL, trim_RR);
      Otto.saveTrimsOnEEPROM(); //Uncomment this only for one upload when you finaly set the trims.
    } 

    sendFinalAck();

}

//-- Function to receive Servo commands
void receiveServo(){  

    sendAck(); 
    moveId = 30;

    //Definition of Servo Bluetooth command
    //G  servo_YL servo_YR servo_RL servo_RR 
    //Example of receiveServo Bluetooth commands
    //G 90 85 96 78 
    bool error = false;
    char *arg;
    int servo_YL,servo_YR,servo_RL,servo_RR;

    arg=SCmd.next();
    if (arg != NULL) { servo_YL=atoi(arg); }    // Converts a char string to an integer   
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { servo_YR=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { servo_RL=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { servo_RR=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}
    
    if(error==true){

      Otto.putMouth(xMouth);
      delay(2000);
      Otto.clearMouth();

    }else{ //Update Servo:

      int servoPos[4]={servo_YL, servo_YR, servo_RL, servo_RR}; 
      Otto._moveServos(200, servoPos);   //Move 200ms
      
    }

    sendFinalAck();

}

//-- Function to receive movement commands
void receiveMovement(){

    sendAck();

    if (Otto.getRestState()==true){
        Otto.setRestState(false);
    }

    //Definition of Movement Bluetooth commands
    //M  MoveID  T   MoveSize  
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {moveId=atoi(arg);}
    else{
      Otto.putMouth(xMouth);
      delay(2000);
      Otto.clearMouth();
      moveId=0; //stop
    }
    
    arg = SCmd.next(); 
    if (arg != NULL) {T=atoi(arg);}
    else{
      T=1000;
    }

    arg = SCmd.next(); 
    if (arg != NULL) {moveSize=atoi(arg);}
    else{
      moveSize =15;
    }
}

//-- Function to execute the right movement according the movement command received.
void move(int moveId){

  bool manualMode = false;

  switch (moveId) {
    case 0:
      Otto.home();
      break;
    case 1: //M 1 1000 
      Otto.walk(1,T,1);
      break;
    case 2: //M 2 1000 
      Otto.walk(1,T,-1);
      break;
    case 3: //M 3 1000 
      Otto.turn(1,T,1);
      break;
    case 4: //M 4 1000 
      Otto.turn(1,T,-1);
      break;
    case 5: //M 5 1000 30 
      Otto.updown(1,T,moveSize);
      break;
    case 6: //M 6 1000 30
      Otto.moonwalker(1,T,moveSize,1);
      break;
    case 7: //M 7 1000 30
      Otto.moonwalker(1,T,moveSize,-1);
      break;
    case 8: //M 8 1000 30
      Otto.swing(1,T,moveSize);
      break;
    case 9: //M 9 1000 30 
      Otto.crusaito(1,T,moveSize,1);
      break;
    case 10: //M 10 1000 30 
      Otto.crusaito(1,T,moveSize,-1);
      break;
    case 11: //M 11 1000 
      Otto.jump(1,T);
      break;
    case 12: //M 12 1000 30 
      Otto.flapping(1,T,moveSize,1);
      break;
    case 13: //M 13 1000 30
      Otto.flapping(1,T,moveSize,-1);
      break;
    case 14: //M 14 1000 20
      Otto.tiptoeSwing(1,T,moveSize);
      break;
    case 15: //M 15 500 
      Otto.bend(1,T,1);
      break;
    case 16: //M 16 500 
      Otto.bend(1,T,-1);
      break;
    case 17: //M 17 500 
      Otto.shakeLeg(1,T,1);
      break;
    case 18: //M 18 500 
      Otto.shakeLeg(1,T,-1);
      break;
    case 19: //M 19 500 20
      Otto.jitter(1,T,moveSize);
      break;
    case 20: //M 20 500 15
      Otto.ascendingTurn(1,T,moveSize);
      break;
    default:
        manualMode = true;
      break;
  }

  if(!manualMode){
    sendFinalAck();
  }
        BTserial.println("M:END");
}

//-- Function to receive gesture commands
void receiveGesture(){

    //sendAck & stop if necessary
    sendAck();
    Otto.home(); 

    //Definition of Gesture Bluetooth commands
    //H  GestureID  
    int gesture = 0;
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {gesture=atoi(arg);}
    else 
    {
      //Otto.putMouth(xMouth);
      delay(2000);
      //Otto.clearMouth();
    }

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
        ematrix.clear(); ematrix.drawBitmap(0, 0, + angry_bmp , 8, 16, LED_ON);  ematrix.writeDisplay();
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
    BTserial.println("H:END");
}

//-- Function to receive sing commands
void receiveSing(){

    //sendAck & stop if necessary
    sendAck();
    Otto.home(); 

    //Definition of Sing Bluetooth commands
    //K  SingID    
    int sing = 0;
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {sing=atoi(arg);}
    else 
    {
     // Otto.putMouth(xMouth);
      delay(2000);
      //Otto.clearMouth();
    }

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

//-- Function to send battery voltage percent
void requestBattery(){

    Otto.home();  //stop if necessary

    //The first read of the batery is often a wrong reading, so we will discard this value. 
    double batteryLevel = Otto.getBatteryLevel();

    Serial.print(F("&&"));
    Serial.print(F("B "));
    Serial.print(batteryLevel);
    Serial.println(F("%%"));
    Serial.flush();
}

//-- Function to send program ID
void requestProgramId(){

    Otto.home();   //stop if necessary

    Serial.print(F("&&"));
    Serial.print(F("I "));
    Serial.print(programID);
    Serial.println(F("%%"));
    Serial.flush();
}

//-- Function to send Ack comand (A)
void sendAck(){

  delay(30);

  Serial.print(F("&&"));
  Serial.print(F("A"));
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send final Ack comand (F)
void sendFinalAck(){

  delay(30);

  Serial.print(F("&&"));
  Serial.print(F("F"));
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Functions with animatics
//--------------------------------------------------------
void OttoLowBatteryAlarm(){

    double batteryLevel = Otto.getBatteryLevel();

    if(batteryLevel<45){
         
          Otto.putMouth(thunder);
          Otto.bendTones (880, 2000, 1.04, 8, 3);  //A5 = 880
          
          delay(30);

          Otto.bendTones (2000, 880, 1.02, 8, 3);  //A5 = 880
          Otto.clearMouth();
          delay(500);    
    }
}

bool checkIfConnected(){
  int state = digitalRead(statePin);
  long now = millis();

  if(state == HIGH){
    
    if(confirmedConnected == false){

      if(lastState == LOW){
        // start the timer for HIGH
        highStartTimestamp = now;
        lastState = HIGH;
      }else{
        if(now - highStartTimestamp >= msToDefineConnection){
          Serial.println("C");
          confirmedConnected = true;
          // do stuff like control an LED
          Otto.sing(S_connection);
          // Empty the buffer
          while(BTserial.available() > 0) {
            char t = BTserial.read();
            Serial.write(t);
          }
        }
      }
    }
    
  }else { // state is LOW
    if(lastState == HIGH && confirmedConnected == true){
      Otto.sing(S_disconnection);
      Serial.println("D");
      confirmedConnected = false;

    }
    lastState = LOW;
  }
  
  return confirmedConnected;
}
