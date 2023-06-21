//----------------------------------------------------------------
//-- Otto All moves test
//-- Otto DIY invests time and resources providing open source code and hardware, 
//-- please support by purchasing kits from https://www.ottodiy.com/
//-- Make sure to have installed all libraries: https://github.com/OttoDIY/OttoDIYLib
//-----------------------------------------------------------------


///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);

  pinMode( D5 , OUTPUT);    
  pinMode(2, OUTPUT);
    tone(  D5,523 ,125);
   delay(125);
  tone(  D5, 587 ,125);
   delay(125);
  tone(  D5, 659 ,125);
   delay(125);
  tone(  D5, 698 ,125);
   delay(125);
  tone(  D5, 784 ,125);
   delay(125);
  tone(  D5,880,125);
   delay(125);
  tone(  D5,261,125);
   delay(125);

}

///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////

void loop() {
    digitalWrite(2, LOW);
    Serial.println("LIGHT:");
    Serial.print(analogRead(A0));
    if (1==1) {
      digitalWrite(2, HIGH);
      tone(  D5,440,125);
       delay(125);
    }

}
