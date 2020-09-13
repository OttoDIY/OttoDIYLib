// NewPing - Version: Latest 
#include <NewPing.h>
#include <Servo.h>
Servo left;
Servo right;
NewPing Ping(8,9,30);
const int leftSensor = 4;
const int rightSensor = 5;
int mode = 3;
void setup() {
  // put your setup code here, to run once:
  left.attach(2);
  right.attach(3);
  pinMode(leftSensor, INPUT_PULLUP);
  pinMode(rightSensor,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mode == 0){
    test();
  }
  if(mode == 1){
    Sense();
  }
  if(mode == 2){
    Avoid();
  }
  if(mode == 3){
    Sumo();
  }
}
void test(){
  left.write(90);
  right.write(90);
  delay(1000);
  left.write(45);
  right.write(45);
  delay(1000);
  left.write(0);
  right.write(0);
  delay(1000);
}
void Sense(){
  int sensorStateLeft = digitalRead(leftSensor);
  int sensorStateRight = digitalRead(rightSensor);
  Serial.println("Left Edge: ");
  Serial.print( digitalRead(sensorStateLeft));
  Serial.println("Right Edge: ");
  Serial.print(digitalRead(sensorStateRight));

  Serial.println();

  Serial.println("Distance: ");
  Serial.print(Ping.ping_cm());

  Serial.println();

  delay(500);
}
void Avoid(){
  int sensorStateLeft = digitalRead(leftSensor);
  int sensorStateRight = digitalRead(rightSensor);
  delay(50);
  if (Ping.ping_cm() >= 15 && sensorStateLeft == 0 && sensorStateRight == 0){
    left.write(0);
    right.write(90);
  }
  if(Ping.ping_cm() <= 15 && Ping.ping_cm() != 0 || sensorStateLeft == 1 || sensorStateRight == 1)
  {
    left.write(90);
    right.write(0);
  }
}
void Sumo(){
  int sensorStateLeft = digitalRead(leftSensor);
  int sensorStateRight = digitalRead(rightSensor);
  if (sensorStateLeft == 1 && sensorStateRight == 1){
    left.write(0);
    right.write(90);
  }
  if(Ping.ping_cm() <= 15 && Ping.ping_cm() != 0 || sensorStateLeft == 0 || sensorStateRight == 0)
  {
    left.write(90);
    right.write(0);
    delay(1500);
  }
}
