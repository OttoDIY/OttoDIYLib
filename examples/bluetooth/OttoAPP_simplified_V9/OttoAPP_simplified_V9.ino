#include <Otto9.h>
Otto9 Otto;
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
/*             -------- 
              |  O  O  |
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
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(9600);  
  BTserial.begin(9600);  
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo);
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
  Otto.sing(S_happy);
    delay(200);
  Otto.putMouth(happyOpen);
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
