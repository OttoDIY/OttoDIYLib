//----------------------------------------------------------------
// Otto mBlock Software in Scratch Mode via USBSerial Cable
//-- With this code Otto can be control via serial with your computer by connecting Otto with cable USB using mblock or scratch, choose right SerialPort and Arduino Nano 328 as board
//-- April 2019: Designed to work with the basic Otto but could be compatible with PLUS or Humanoid or other biped robots
/******************************************************************************************************
  Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
  Otto DIY invests time and resources providing open source code and hardware, 
  please support by purchasing kits from (https://www.ottodiy.com)
  
  BSD license, all text above must be included in any redistribution
 *******************************************************************/
//-----------------------------------------------------------------
#include "Otto9.h"
#include "OttoSerialCommand.h"
OttoSerialCommand SCmd; //The SerialCommand object
//---------------------------------------------------------
//-- Make sure the servos are in the right pin
/*             -------- 
              |  O  O  |
              |--------|
  RIGHT LEG 3 |        | LEFT LEG 2
               -------- 
               ||     ||
RIGHT FOOT 5 |---     ---| LEFT FOOT 4 
*/
Otto9 Otto;  
//---------------------------------------------------------
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)
//---------------------------------------------------------
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
const char programID[]="Otto_mBlock";
const char name_fac='$'; //Factory name
const char name_fir='#'; 

//-- Movement parameters
int T=1000;              //Initial duration of movement, show the speed
int moveId=0;            //Type of movement
int moveSize=15;         //Size of movement
unsigned long previousMillis=0;
int randomDance=0;
int randomSteps=0;
bool obstacleDetected = false;
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){

  //BT.begin(9600);  
  Serial.begin(115200);
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin
  randomSeed(analogRead(A0));
  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  SCmd.addCommand("L", receiveLED);       //  sendAck & sendFinalAck
  SCmd.addCommand("T", recieveBuzzer);    //  sendAck & sendFinalAck
  SCmd.addCommand("M", receiveMovement);  //  sendAck & sendFinalAck
  SCmd.addCommand("H", receiveGesture);   //  sendAck & sendFinalAck
  SCmd.addCommand("K", receiveSing);      //  sendAck & sendFinalAck
  SCmd.addCommand("C", receiveTrims);     //  sendAck & sendFinalAck
  SCmd.addCommand("G", receiveServo);     //  sendAck & sendFinalAck
  SCmd.addCommand("R", receiveName);      //  sendAck & sendFinalAck
  SCmd.addCommand("E", requestName);
  SCmd.addCommand("D", requestDistance);
  SCmd.addCommand("N", requestNoise);
  SCmd.addCommand("B", requestBattery);
  SCmd.addCommand("I", requestProgramId);
  SCmd.addDefaultHandler(receiveStop);
  
  Otto.sing(S_connection);
  Otto.home();
  delay(50);
  requestName();
  delay(50);
  requestProgramId();
  delay(50);
  requestBattery();

  LowBatteryAlarm();
  Otto.sing(S_happy);
  delay(200);
}

///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {

  if (Serial.available() > 0) {

    SCmd.readSerial();
  
    //If is moving yet
    if (Otto.getRestState()==false) {
      move(moveId);
    }
  } 
}

///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////
void obstacleDetector(){

   int distance = Otto.getDistance();

        if(distance<15){
          obstacleDetected = true;
        }else{
          obstacleDetected = false;
        }
}
//-- Function to receive Stop command.
void receiveStop(){
    sendAck();
    Otto.home();
    sendFinalAck();
}
void receiveLED(){  

    //sendAck & stop if necessary
    sendAck();
    Otto.home();
     //put some code for deny (not available)
   
    sendFinalAck();

}
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
      delay(2000);
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
    Otto.sing(S_confused);
    //Otto.playGesture(RobotConfused);// Indicate that Function not availabe for this version
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

//      Otto.putMouth(xMouth);
      delay(2000);
  //    Otto.clearMouth();

    }else{ //Update Servo:

      int servoPos[4]={servo_YL, servo_YR, servo_RL, servo_RR}; 
      Otto._moveServos(200, servoPos);   //Move 200ms
      
    }
    sendFinalAck();
}

void receiveMovement(){

    sendAck();
  //  Serial.print("Move Command: ");
    if (Otto.getRestState()==true){
        Otto.setRestState(false);
    }

    //Definition of Movement Bluetooth commands
    //M  MoveID  T   MoveSize  
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {moveId=atoi(arg);}// Serial.println(moveId); Serial.print(" ");}
    else{
//      Otto.putMouth(xMouth);
      delay(2000);
  //    Otto.clearMouth();
      moveId=0; //stop
    }
    
    arg = SCmd.next(); 
    if (arg != NULL) {T=atoi(arg);} //Serial.println(T); Serial.print(" ");}
    else{
      T=1000;
    }

    arg = SCmd.next(); 
    if (arg != NULL) {moveSize=atoi(arg);}// Serial.println(moveSize); Serial.print(" ");}
    else{
      moveSize =30;
    }
}

//-- Function to execute the right movement according the movement command received.///
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
      Otto.updown(1,T,30);
      break;
    case 6: //M 6 1000 30
      moveSize = 30;
      Otto.moonwalker(1,T,moveSize,1);
      break;
    case 7: //M 7 1000 30
      moveSize = 30;
      Otto.moonwalker(1,T,moveSize,-1);
      break;
    case 8: //M 8 1000 30
	  moveSize =30;
      Otto.swing(1,T,moveSize);
      break;
    case 9: //M 9 1000 30 
      moveSize =30;
	  Otto.crusaito(1,T,moveSize,1);
      break;
    case 10: //M 10 1000 30 
      moveSize =30;
	  Otto.crusaito(1,T,moveSize,-1);
      break;
    case 11: //M 11 1000 
      Otto.jump(1,T);
      break;
    case 12: //M 12 1000 30 
      moveSize =30;
	  Otto.flapping(1,T,moveSize,1);
      break;
    case 13: //M 13 1000 30
      moveSize =30;
	  Otto.flapping(1,T,moveSize,-1);
      break;
    case 14: //M 14 1000 20
      moveSize =30;
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
      moveSize =30;
	  Otto.jitter(1,T,moveSize);
      break;
    case 20: //M 20 500 15
      Otto.ascendingTurn(1,T,moveSize);
      break;
    default:
        manualMode = true;
      break;
  }

  if (!manualMode){
    sendFinalAck();
  }
       
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
     // Otto.clearMouth();
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

//-- Function to receive Name command
void receiveName(){

    Otto.home(); 
    Otto.sing(S_confused); //deny to receive command changing the name. 

}

//-- Function to send Otto's name
void requestName(){

    Otto.home(); //stop if necessary

    char actualOttoName[11]= "Zowi";  //Variable to store data read from EEPROM.

    Serial.print(F("&&"));
    Serial.print(F("E "));
    Serial.print(actualOttoName);
    Serial.println(F("%%"));
    Serial.flush();
}

//-- Function to send ultrasonic sensor measure (distance in "cm")
void requestDistance(){

    Otto.home();  //stop if necessary  

    int distance = Otto.getDistance();
    Serial.print(F("&&"));
    Serial.print(F("D "));
    Serial.print(distance);
    Serial.println(F("%%"));
    Serial.flush();
}

void requestNoise(){

    Otto.home();  //stop if necessary

    int microphone= Otto.getNoise(); //analogRead(PIN_NoiseSensor);
    Serial.print(F("&&"));
    Serial.print(F("N "));
    Serial.print(microphone);
    Serial.println(F("%%"));
    Serial.flush();
}

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

void requestProgramId(){

    Otto.home();   //stop if necessary

    Serial.print(F("&&"));
    Serial.print(F("I "));
    Serial.print(programID);
    Serial.println(F("%%"));
    Serial.flush();
}

void sendAck(){

  delay(30);

  Serial.print(F("&&"));
  Serial.print(F("A"));
  Serial.println(F("%%"));
  Serial.flush();
}

void sendFinalAck(){

  delay(30);

  Serial.print(F("&&"));
  Serial.print(F("F"));
  Serial.println(F("%%"));
  Serial.flush();
}

void LowBatteryAlarm(){

    double batteryLevel = Otto.getBatteryLevel();

    if(batteryLevel<45){
      Otto.bendTones (880, 2000, 1.04, 8, 3);  //A5 = 880
      
      delay(30);
      
      Otto.bendTones (2000, 880, 1.02, 8, 3);  //A5 = 880
      delay(500);     
    }
}
