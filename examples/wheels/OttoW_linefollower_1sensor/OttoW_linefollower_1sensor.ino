#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

Servo rightServo;  
Servo leftServo; 

int desiredMouth;
int rightSpeed = 0;
int leftSpeed = 0;
int command;
int ready = 0;

//+++++++++++++++FUNCTION DECLARATIONS+++++++++++++++++++++++++++
void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int Sensor = A7; 
int sensorValue = 0; 

unsigned long SetPoint;
unsigned long programTime;
const unsigned long period = 4000; //sets period equal to 4250 milliseconds 

void setup () {
  Serial.begin(9600);
  rightServo.attach(3); //pin s9 on Meped
  leftServo.attach(2);

  Serial.setTimeout(50);      //ensures the the arduino does not read serial for too long
  Serial.println("started");
}
void loop () {
  int count = 0;
  sensorValue = analogRead (Sensor);

  Serial.print("Initial Read: ");
  Serial.print(sensorValue);

  if(sensorValue > 520){ //takes values that are greater than 520 and converts them to 845
    sensorValue = 845; 
    Serial.print("first if: ");
    Serial.print(sensorValue);
  }
  if(sensorValue <= 520){ //takes values that are less than or equal to 520 and converts them to 145
    sensorValue = 145;
    Serial.print("Second if: ");
    Serial.print(sensorValue); 
  }

  Serial.print("sensor 520: ");
  Serial.print(sensorValue);

  if (sensorValue == 845) //if the sensor value is equal to 845 the right servo starts moving
  { 
    rightSpeed = -25;    
    leftSpeed  = 0;
    motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
    Serial.print("SensorValue1: ");
    Serial.println(sensorValue);
    SetPoint = programTime; //sets the current time equal to the program time using millis()
  }

  if (sensorValue == 145){ //if the sensorValue is equal to 145 the program starts this set

    Serial.print("SensorValue2: ");
    Serial.println(sensorValue);

    programTime = millis(); //declares programTime equal to the millis() time clock.

    count++; //takes the count integer and adds 1 to it

    if(count==1){ //if count is equal to one the program enters this for statement
      for(count=0; count==1; count++){ //makes count equal to zero, if count is equal to 1 exit the for loop, add one to count after every iteration
        SetPoint = millis(); //Sets a certain point in the millis() timeline known as currentTime
        Serial.print("Count: ");
        Serial.println(count);
      }
    }

    if (programTime - SetPoint <= period){ //subtracts the SetPoint or currenTime from the program time and if the is less than the period or 4250 milliseconds; it goes through the if statement
      rightSpeed = 0;    
      leftSpeed = -17; //left servo moves forward
      motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
    }

    if (programTime - SetPoint > period){ //if the set point subtracted from the program time is greater than the period enter the if statement
      rightSpeed = -25;  //right servo moves forward 
      leftSpeed = 17;
      motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
      count= 0;
    }

    Serial.print("Program Time: ");
    Serial.print(programTime);
    Serial.print("SetPoint: ");
    Serial.print(SetPoint);
    Serial.print("Right Speed: ");
    Serial.print(rightSpeed);
    Serial.print("left Speed: ");
    Serial.print(leftSpeed);
  }
}


//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo){


  //Set all of the servo positions
  theRightServo.write(90 - rightSpeed);  //compensate for the 0-180, -90-90 relationship
  theLeftServo.write(90 + leftSpeed);
  //mouthServo.write(desiredMouth);

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
