//Led Display

#include "LedControl.h"


#define	MAX_DEVICES	1
#define	CLK_PIN		15
#define	DATA_PIN	17
#define	CS_PIN		16

LedControl LM = LedControl(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

String inString = ""; // String para lectura desde Protocoder

void readFromAndroid() {
  char inChar;
  while (Serial.available() > 0) {
    inChar = (char) Serial.read();
    Serial.flush();
    if (inChar == '='){
      inString = "";
      LM.clearDisplay(0);
    }else if (inChar != '+'){
      inString += inChar;
    }
  }
}

void writeData(){
  
  if (inString == "up") {
    lookUp();
  }
  if (inString == "down") {
    lookDown();
  }
  if (inString == "center") {
    lookCenter();
  }
  if (inString == "left") {
    lookLeft();
  }
  if (inString == "right") {
    lookRight();
  }
}
void setup() {

  Serial.begin(19200);
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   // Function to go to power-save mode: LM.shutdown(0,true);
   */
  LM.shutdown(0, false);
  /* Set the brightness to max values value={0,15} */
  LM.setIntensity(0, 15);
  /* and clear the display */
  LM.clearDisplay(0);
  lookCenter();
  
  // test rutines

  //testPos(250);
  //test();
  
}

int delayTime = 750;
long nextTime=1000;
long nextTimeDouble = 10000;
void loop() {

  if (millis()>nextTimeDouble){
    fullBlink(60);
    delay(100);
    fullBlink(60);
    nextTimeDouble = millis() + random(30000, 60000);
    writeData();
  }else if (millis() > nextTime){
    fullBlink(60);
    nextTime = millis() + random(1500, 4000);
    writeData();
  }


  if (Serial.available() > 0) {
    readFromAndroid();
  }
  writeData();
}

void testPos(int delayTime){
  lookCenter();
  delay(delayTime);
  lookUp();
  delay(delayTime);
  lookCenter();
  delay(delayTime);
  lookDown();
  delay(delayTime);
  lookCenter();
  delay(delayTime);
  lookRight();
  delay(delayTime);
  lookCenter();
  delay(delayTime);
  lookLeft();
  delay(delayTime);
}

void test() {
  LM.shutdown(0, false);
  LM.clearDisplay(0);

  for (int row = 0; row < 8; row++) {
    for ( int col = 0; col < 8; col++) {
      LM.setLed(0, row, col, true);
      delay(100);
    }
  }
}

void lookDown() {

  LM.setLed(0, 3, 3, false);
  LM.setLed(0, 3, 4, false);
  LM.setLed(0, 5, 3, false);
  LM.setLed(0, 5, 4, false);

  LM.setLed(0, 1, 3, true);
  LM.setLed(0, 1, 4, true);
  LM.setLed(0, 2, 2, true);
  LM.setLed(0, 2, 3, true);
  LM.setLed(0, 2, 4, true);
  LM.setLed(0, 2, 5, true);
  LM.setLed(0, 3, 1, true);
  LM.setLed(0, 3, 2, true);
  LM.setLed(0, 3, 3, true);
  LM.setLed(0, 3, 4, true);
  LM.setLed(0, 3, 5, true);
  LM.setLed(0, 3, 6, true);
  LM.setLed(0, 4, 1, true);
  LM.setLed(0, 4, 2, true);
  LM.setLed(0, 4, 5, true);
  LM.setLed(0, 4, 6, true);
  LM.setLed(0, 5, 2, true);
  LM.setLed(0, 5, 5, true);
  LM.setLed(0, 6, 3, true);
  LM.setLed(0, 6, 4, true);

}
void lookUp() {

  LM.setLed(0, 2, 3, false);
  LM.setLed(0, 2, 4, false);
  LM.setLed(0, 3, 3, false);
  LM.setLed(0, 3, 4, false);

  LM.setLed(0, 1, 3, true);
  LM.setLed(0, 1, 4, true);
  LM.setLed(0, 2, 2, true);
  LM.setLed(0, 2, 5, true);
  LM.setLed(0, 3, 1, true);
  LM.setLed(0, 3, 2, true);
  LM.setLed(0, 3, 5, true);
  LM.setLed(0, 3, 6, true);
  LM.setLed(0, 4, 1, true);
  LM.setLed(0, 4, 2, true);
  LM.setLed(0, 4, 3, true);
  LM.setLed(0, 4, 4, true);
  LM.setLed(0, 4, 5, true);
  LM.setLed(0, 4, 6, true);
  LM.setLed(0, 5, 2, true);
  LM.setLed(0, 5, 3, true);
  LM.setLed(0, 5, 4, true);
  LM.setLed(0, 5, 5, true);
  LM.setLed(0, 6, 3, true);
  LM.setLed(0, 6, 4, true);

}

void lookCenter() {

  LM.setLed(0, 3, 3, false);
  LM.setLed(0, 3, 4, false);
  LM.setLed(0, 4, 3, false);
  LM.setLed(0, 4, 4, false);

  LM.setLed(0, 1, 3, true);
  LM.setLed(0, 1, 4, true);
  LM.setLed(0, 2, 2, true);
  LM.setLed(0, 2, 3, true);
  LM.setLed(0, 2, 4, true);
  LM.setLed(0, 2, 5, true);
  LM.setLed(0, 3, 1, true);
  LM.setLed(0, 3, 2, true);
  LM.setLed(0, 3, 5, true);
  LM.setLed(0, 3, 6, true);
  LM.setLed(0, 4, 1, true);
  LM.setLed(0, 4, 2, true);
  LM.setLed(0, 4, 5, true);
  LM.setLed(0, 4, 6, true);
  LM.setLed(0, 5, 2, true);
  LM.setLed(0, 5, 3, true);
  LM.setLed(0, 5, 4, true);
  LM.setLed(0, 5, 5, true);
  LM.setLed(0, 6, 3, true);
  LM.setLed(0, 6, 4, true);

}

void lookLeft() {

  LM.setLed(0, 3, 4, false);
  LM.setLed(0, 3, 5, false);
  LM.setLed(0, 4, 4, false);
  LM.setLed(0, 4, 5, false);

  LM.setLed(0, 1, 3, true);
  LM.setLed(0, 1, 4, true);
  LM.setLed(0, 2, 2, true);
  LM.setLed(0, 2, 3, true);
  LM.setLed(0, 2, 4, true);
  LM.setLed(0, 2, 5, true);
  LM.setLed(0, 3, 1, true);
  LM.setLed(0, 3, 2, true);
  LM.setLed(0, 3, 3, true);
  LM.setLed(0, 3, 6, true);
  LM.setLed(0, 4, 1, true);
  LM.setLed(0, 4, 2, true);
  LM.setLed(0, 4, 3, true);
  LM.setLed(0, 4, 6, true);
  LM.setLed(0, 5, 2, true);
  LM.setLed(0, 5, 3, true);
  LM.setLed(0, 5, 4, true);
  LM.setLed(0, 5, 5, true);
  LM.setLed(0, 6, 3, true);
  LM.setLed(0, 6, 4, true);
}

void lookRight(){

  LM.setLed(0, 3, 2, false);
  LM.setLed(0, 3, 3, false);
  LM.setLed(0, 4, 2, false);
  LM.setLed(0, 4, 3, false);

  LM.setLed(0, 1, 3, true);
  LM.setLed(0, 1, 4, true);
  LM.setLed(0, 2, 2, true);
  LM.setLed(0, 2, 3, true);
  LM.setLed(0, 2, 4, true);
  LM.setLed(0, 2, 5, true);
  LM.setLed(0, 3, 1, true);
  LM.setLed(0, 3, 4, true);
  LM.setLed(0, 3, 5, true);
  LM.setLed(0, 3, 6, true);
  LM.setLed(0, 4, 1, true);
  LM.setLed(0, 4, 4, true);
  LM.setLed(0, 4, 5, true);
  LM.setLed(0, 4, 6, true);
  LM.setLed(0, 5, 2, true);
  LM.setLed(0, 5, 3, true);
  LM.setLed(0, 5, 4, true);
  LM.setLed(0, 5, 5, true);
  LM.setLed(0, 6, 3, true);
  LM.setLed(0, 6, 4, true);
}

int delayBlink = 10;

void fullBlink(int timeClosed){
    blinkClose();
    delay(timeClosed);
    blinkOpen(); 
}
void blinkClose(){


  LM.setLed(0,1,3,false);
  LM.setLed(0,1,4,false);
  LM.setLed(0,6,3,false);
  LM.setLed(0,6,4,false);
  
  //delay(delayBlink);

  LM.setLed(0,2,2,false);
  LM.setLed(0,2,3,false);
  LM.setLed(0,2,4,false);
  LM.setLed(0,2,5,false);
  
  LM.setLed(0,5,2,false);
  LM.setLed(0,5,3,false);
  LM.setLed(0,5,4,false);
  LM.setLed(0,5,5,false);

  //delay(delayBlink);

  LM.setLed(0,3,1,false);
  LM.setLed(0,3,2,false);
  LM.setLed(0,3,5,false);
  LM.setLed(0,3,6,false);

  LM.setLed(0,4,3,true);
  LM.setLed(0,4,4,true);

  delay(delayBlink*3);
}

void blinkOpen(){

  LM.setLed(0,4,3,false);
  LM.setLed(0,4,4,false);

  LM.setLed(0,3,1,true);
  LM.setLed(0,3,2,true);
  LM.setLed(0,3,5,true);
  LM.setLed(0,3,6,true);


  //delay(delayBlink);

  LM.setLed(0,2,2,true);
  LM.setLed(0,2,3,true);
  LM.setLed(0,2,4,true);
  LM.setLed(0,2,5,true);
  
  LM.setLed(0,5,2,true);
  LM.setLed(0,5,3,true);
  LM.setLed(0,5,4,true);
  LM.setLed(0,5,5,true);

  //delay(delayBlink);

  LM.setLed(0,1,3,true);
  LM.setLed(0,1,4,true);
  LM.setLed(0,6,3,true);
  LM.setLed(0,6,4,true);
  delay(delayBlink);
}
