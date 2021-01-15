/*------ Arduino Line Follower Code----- */
/*-------definning Inputs------*/
#define LS A6      // left sensor
#define RS A5      // right sensor

/*-------definning Outputs------*/
#define LM1 9       // left motor
#define LM2 10       // left motor
#define RM1 5       // right motor
#define RM2 6       // right motor

void setup()
{
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
}

void loop()
{
  if(!(digitalRead(LS)) && !(digitalRead(RS)))     // Move Forward
  {
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
  }
  
  if(!(digitalRead(LS)) && digitalRead(RS))     // Turn right
  {
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
  }
  
  if(digitalRead(LS) && !(digitalRead(RS)))     // turn left
  {
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
  }
  
  if(digitalRead(LS) && digitalRead(RS))     // stop
  {
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
  }
  // Move Backward
   // digitalWrite(LM1, LOW);
    //digitalWrite(LM2, HIGH);
    //digitalWrite(RM1, LOW);
    //digitalWrite(RM2, HIGH);
      //  delay(10000);
}
