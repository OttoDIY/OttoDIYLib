//----------------------------------------------------------------
//-- Otto Guess game
//-- (c) ottodiy.com Released under a GPL licencse
//-----------------------------------------------------------------
#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
//#include <BatReader.h>
#include <US.h>
#include <EnableInterrupt.h>
#include <OttoSerialCommand.h>
OttoSerialCommand SCmd;  //The SerialCommand object
//-- Otto Library
#include <Otto9.h>
Otto9 Otto;  //This is Otto!!
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1

//---------------------------------------------------------
//-- Configuration of pins where the servos are attached
/*
         --------------- 
        |     O   O     |
        |---------------|
YR 3==> |               | <== YL 2
         --------------- 
            ||     ||
RR 5==>   -----   ------  <== RL 4
         |-----   ------|
*/
#define PIN_YL 2 //servo[0]  left leg
#define PIN_YR 3 //servo[1]  right leg
#define PIN_RL 4 //servo[2]  left foot
#define PIN_RR 5 //servo[3]  right foot
#define PIN_Trigger  8  //TRIGGER pin (8)
#define PIN_Echo     9  //ECHO pin (9)
#define PIN_Buzzer  13 //BUZZER pin (13)
//---------------------------------------------------------
//---Otto Buttons
#define PIN_SecondButton A0
#define PIN_ThirdButton A6
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
#define BAT_MAX  4.2
#define BAT_MIN 3.2
#define SLOPE 100/(BAT_MAX - BAT_MIN)
#define OFFSET  (100*BAT_MIN)/(BAT_MAX - BAT_MIN)

const char programID[] = "Otto_Adivinawi_v3"; //Each program will have a ID
//-- Movement parameters
int T = 1000;            //Initial duration of movement
int moveId = 0;          //Number of movement
int moveSize = 15;       //Asociated with the height of some movements
//---------------------------------------------------------
//-- Adivinawi has 5 modes:
//--    * MODE = 0: Otto is awaiting
//--    * MODE = 1: Otto Adivino Adivinawi
//--    * MODE = 2: Otto Dado Dice
//--    * MODE = 3: Rock Paper Scissors
//--    * MODE = 4: OttoPAD or any Teleoperation mode (listening SerialPort).
//--
volatile int MODE = 0; //State of Otto in the principal state machine.
volatile int currentMode = 1; //To remember current mode when changing modes
//---------------------------------------------------------
volatile bool buttonPushed = false; //Variable to remember when a button has been pushed
volatile bool buttonAPushed = false; //Variable to remember when A button has been pushed
volatile bool buttonBPushed = false; //Variable to remember when B button has been pushed

unsigned long previousMillis = 0;

int randomDance = 0;
int randomSteps = 0;

bool obstacleDetected = false;

unsigned long int rock_symbol =    0b00000000001100011110011110001100;
unsigned long int paper_symbol =   0b00011110010010010010010010011110;
unsigned long int scissors_symbol = 0b00000010010100001000010100000010;

int angryPos2[4] =    {90, 90, 70, 110};
int headLeft2[4] =    {110, 110, 90, 90};
int headRight2[4] =   {70, 70, 90, 90};

//Function that reads voltage of battery
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  long result = (high << 8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {

  //Serial communication initialization
  Serial.begin(57600);

  pinMode(PIN_SecondButton, INPUT);
  pinMode(PIN_ThirdButton, INPUT);

  //Set the servo pins
Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, A6, PIN_Buzzer, PIN_Trigger, PIN_Echo); //Set the servo pins and ultrasonic pins and Buzzer pin

  //Uncomment this to set the servo trims manually and save on EEPROM
  //Otto.setTrims(TRIM_YL, TRIM_YR, TRIM_RL, TRIM_RR);
  //Otto.saveTrimsOnEEPROM(); //Uncomment this only for one upload when you finaly set the trims.

  randomSeed(digitalRead(A5));   //Set a random seed

  //Interrumptions
  enableInterrupt(PIN_SecondButton, secondButtonPushed, RISING);
  enableInterrupt(PIN_ThirdButton, thirdButtonPushed, RISING);

  //Setup callbacks for SerialCommand commands
  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  SCmd.addCommand("R", receiveName);      //  sendAck & sendFinalAck
  SCmd.addCommand("E", requestName);
  SCmd.addCommand("B", requestBattery);
  SCmd.addCommand("I", requestProgramId);
  SCmd.addDefaultHandler(receiveStop);

  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
  Otto.matrixIntensity(1);// set up Matrix display intensity
  Otto.sing(S_connection);   //Otto wake up!
  delay(500);
  Otto.home();
  //Send Otto name, programID & battery level.
  requestName();
  delay(50);
  requestProgramId();
  delay(50);
  requestBattery();
  OttoLowBatteryAlarm();
  // Animation Uuuuuh - A little moment of initial surprise
  for (int i = 0; i < 2; i++) {
    for (int i = 0; i < 8; i++) {
      if (buttonPushed) {
        break;
      }
      Otto.putAnimationMouth(littleUuh, i);
      delay(150);
    }
  }
  //Smile for a happy Otto :)
  if (!buttonPushed) {
    Otto.playGesture(OttoHappy);
    Otto.sing(S_happy);
    delay(200);
  }

  if (!buttonPushed) {
    Otto.putMouth(happyOpen);
  }
  previousMillis = millis();
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {

  if (Serial.available() > 0 && MODE != 4) {
    MODE = 4;
    Otto.putMouth(happyOpen);
    /*
        //Disable Pin Interruptions
        disableInterrupt(PIN_SecondButton);
        disableInterrupt(PIN_ThirdButton);
        buttonPushed=false;*/
  }
  //First attemp to initial software
  if (buttonPushed) {
    Otto.home();
    // delay(100); //Wait for all buttons
    Otto.sing(S_buttonPushed);
    //  delay(200); //Wait for all buttons
    /*
          if      ( buttonAPushed && !buttonBPushed) {
            MODE = 1;
            Otto.sing(S_mode1);
          }
          else if (!buttonAPushed && buttonBPushed) {
            MODE = 2;
            Otto.sing(S_mode2);
          }
          else if ( buttonAPushed && buttonBPushed) {
            MODE = 3;  //else
            Otto.sing(S_mode3);
          }
    */
    MODE = currentMode;
    Otto.putMouth(MODE);
    if (MODE == 1) {
      Otto.sing(S_mode1);
    }

    else if (MODE == 2) {
      Otto.sing(S_mode2);
    }

    else if (MODE == 3) {
      Otto.sing(S_mode3);
    }

    int showTime = 1500;
    while ((showTime > 0)) { //Wait to show the MODE number

      showTime -= 10;
      delay(10);
    }

    Otto.putMouth(interrogation);
    Otto.sing(S_happy_short);
    delay(200);

    buttonPushed = false;
    buttonAPushed = false;
    buttonBPushed = false;

  } else {

    switch (MODE) {

      //-- MODE 0 - Otto is awaiting
      //---------------------------------------------------------
      case 0:

        //Every 80 seconds in this mode, Otto falls asleep
        if (millis() - previousMillis >= 80000) {
          OttoSleeping_withInterrupts(); //ZZzzzzz...
          previousMillis = millis();
        }
        break;


      //-- MODE 1 - Otto Adivino Adivinawi
      //---------------------------------------------------------
      case 1:

        delay(50);
        if (Otto.getNoise() >= 600) { //740
          delay(50);
          OttoMagics(2);

          int randomNum = random(1, 3); //1,2  - YES, NO

          if (randomNum == 1) { //1 = YES

            if (!buttonPushed) {
              Otto._tone(note_E5, 50, 30);
              Otto.putMouth(okMouth);
            }

            if (!buttonPushed) {
              Otto.sing(S_happy);
              Otto.swing(1, 800, 20);
              Otto.sing(S_superHappy);
            }


          } else { //2 = NO

            if (!buttonPushed) {
              Otto._moveServos(300, angryPos2);
              Otto.putMouth(xMouth);
            }

            if (!buttonPushed) {
              Otto._tone(note_A5, 100, 30);
              Otto.bendTones(note_A5, note_D6, 1.02, 7, 4);
              Otto.bendTones(note_D6, note_G6, 1.02, 10, 1);
              Otto.bendTones(note_G6, note_A5, 1.02, 10, 1);
              delay(15);
            }

            if (!buttonPushed) {
              Otto.bendTones(note_A5, note_E5, 1.02, 20, 4);
              delay(400);
              Otto._moveServos(200, headLeft2);
            }

            if (!buttonPushed) {
              Otto.bendTones(note_A5, note_D6, 1.02, 20, 4);
              Otto._moveServos(200, headRight2);
              Otto.bendTones(note_A5, note_E5, 1.02, 20, 4);
            }
          }

          Otto.home();

          if (!buttonPushed) {
            delay(2000);
          }

          if (!buttonPushed) {
            Otto.clearMouth();
            delay(500);
          }

          if (!buttonPushed) {
            Otto.putMouth(interrogation);
            Otto.sing(S_happy_short);
          }

          if (!buttonPushed) {
            Otto.clearMouth();
            delay(200);
            Otto.putMouth(interrogation);
          }

        }
        break;


      //-- MODE 2 - Otto Dado Dice
      //---------------------------------------------------------
      case 2:


        if (Otto.getNoise() >= 600) { //740

          delay(50);
          OttoMagics(1);

          int randomNum = random(1, 7); //1-6

          if (!buttonPushed) {

            Otto.putMouth(randomNum);

            if (randomNum == 1) {
              Otto.sing(S_sad);
            }
            else if (randomNum == 6) {
              Otto.sing(S_superHappy);
            }
            else {
              Otto.sing(S_connection);
            }
          }

          for (int i = 0; i < 4; i++) {
            if (!buttonPushed) {
              Otto.clearMouth();
              delay(200);
            }
            if (!buttonPushed) {
              Otto.putMouth(randomNum);
              delay(200);
            }
          }

          if (!buttonPushed) {
            delay(2300);
          }

          if (!buttonPushed) {
            Otto.clearMouth();
            delay(100);
          }

          if (!buttonPushed) {
            Otto.putMouth(interrogation);
            Otto.sing(S_happy_short);
          }

          if (!buttonPushed) {
            Otto.clearMouth();
            delay(200);
            Otto.putMouth(interrogation);
          }

        }


        break;

      //-- MODE 3 - Rock Paper Scissors
      //---------------------------------------------------------
      case 3:

        delay(50);

       if (Otto.getNoise() >= 600) { //740

          switch (random(0, 3)) {

            case 0:
              if (buttonPushed) {
                break;
              }
              Otto.putMouth(rock_symbol, 0);
              Otto.sing(S_fart1);
              break;

            case 1:
              if (buttonPushed) {
                break;
              }
              Otto.putMouth(paper_symbol, 0);
              Otto.sing(S_OhOoh2);
              break;

            case 2:
              if (buttonPushed) {
                break;
              }
              Otto.putMouth(scissors_symbol, 0);
              Otto.sing(S_cuddly);
              break;
          }

          if (!buttonPushed) {
            delay(2500);
          }

          if (!buttonPushed) {
            Otto.clearMouth();
            delay(100);
          }

          if (!buttonPushed) {
            Otto.putMouth(interrogation);
            Otto.sing(S_happy_short);
          }

          if (!buttonPushed) {
            Otto.clearMouth();
            delay(200);
            Otto.putMouth(interrogation);
          }
        }
        break;

      case 4:

        SCmd.readSerial();
        // //If Otto is moving yet
        // if (Otto.getRestState()==false){
        //   move(moveId);
        // }

        break;
        
      default:
        MODE = 4;
        break;
    }
  }
}
///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////

//Adivinawi gesture with interruptions
void OttoMagics(int repetitions) {

  //Initial note frecuency = 400
  //Final note frecuency = 1000

  // Reproduce the animation four times
  for (int i = 0; i < repetitions; i++) {

    int noteM = 400;
    if (buttonPushed) {
      break;
    }

    for (int index = 0; index < 6; index++) {
      if (buttonPushed) {
        break;
      }
      Otto.putAnimationMouth(adivinawi, index);
      Otto.bendTones(noteM, noteM + 100, 1.04, 10, 10);  //400 -> 1000
      noteM += 100;
    }

    if (!buttonPushed) {
      Otto.clearMouth();
      Otto.bendTones(noteM - 100, noteM + 100, 1.04, 10, 10); //900 -> 1100
    }

    for (int index = 0; index < 6; index++) {
      if (buttonPushed) {
        break;
      }
      Otto.putAnimationMouth(adivinawi, index);
      Otto.bendTones(noteM, noteM + 100, 1.04, 10, 10);  //1000 -> 400
      noteM -= 100;
    }
  }
  if (!buttonPushed) {
    delay(300);
    Otto.putMouth(happyOpen);
  }
}
//-- Function executed when second button is pushed
void secondButtonPushed() {

  buttonAPushed = true;

  if (!buttonPushed) {
    buttonPushed = true;
    Otto.putMouth(smallSurprise);
  }
  currentMode = MODE + 1;
  if (currentMode > 3) {
    currentMode = 0;
  }
}

  //-- Function executed when third button is pushed
  void thirdButtonPushed() {

  buttonBPushed = true;

  if (!buttonPushed) {
    buttonPushed = true;
    Otto.putMouth(smallSurprise);
  }
  }

//-- Function to receive Stop command.
void receiveStop() {
  sendAck();
  Otto.home();
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
    Otto.putMouth(xMouth);
    delay(2000);
    Otto.clearMouth();
  }
  sendFinalAck();
}


//-- Function to send Otto's name
void requestName() {

  Otto.home(); //stop if necessary
  char actualOttoName[11] = ""; //Variable to store data read from EEPROM.
  int eeAddress = 5;            //EEPROM address to start reading from
  //Get the float data from the EEPROM at position 'eeAddress'
  EEPROM.get(eeAddress, actualOttoName);
  Serial.print(F("&&"));
  Serial.print(F("E "));
  Serial.print(actualOttoName);
  Serial.println(F("%%"));
  Serial.flush();
}
//-- Function to send battery voltage percent
void requestBattery() {

  Otto.home();  //stop if necessary

  int batteryLevel = ((readVcc() / 1000.00) * SLOPE) - OFFSET;

  //double batteryLevel = Otto.getBatteryLevel(); //uncomment this one and comment out above one when using step up converter

  if (batteryLevel > 100) {
    batteryLevel = 100;
  };

  Serial.print(F("&&"));
  Serial.print(F("B "));
  Serial.print(batteryLevel);
  Serial.println(F("%%"));
  Serial.flush();
}

//-- Function to send program ID
void requestProgramId() {

  Otto.home();   //stop if necessary

  Serial.print(F("&&"));
  Serial.print(F("I "));
  Serial.print(programID);
  Serial.println(F("%%"));
  Serial.flush();
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

//-- Functions with animatics
//--------------------------------------------------------

void OttoLowBatteryAlarm() {

  int batteryLevel = ((readVcc() / 1000.00) * SLOPE) - OFFSET;

  //double batteryLevel = Otto.getBatteryLevel(); //uncomment this one and comment out above one when using step up converter

  if (batteryLevel < 20) {

    while (!buttonPushed) {

      Otto.putMouth(thunder);
      Otto.bendTones (880, 2000, 1.04, 8, 3);  //A5 = 880

      delay(30);

      Otto.bendTones (2000, 880, 1.02, 8, 3);  //A5 = 880
      Otto.clearMouth();
      delay(500);
    }
  }
}

void OttoSleeping_withInterrupts() {

  int bedPos_0[4] = {100, 80, 60, 120}; //{100, 80, 40, 140}

  if (!buttonPushed) {
    Otto._moveServos(700, bedPos_0);  //800
  }
  for (int i = 0; i < 4; i++) {

    if (buttonPushed) {
      break;
    }
//    digitalWrite(ledR, HIGH);
//    digitalWrite(ledG, HIGH);
  //  digitalWrite(ledB, HIGH);
    Otto.putAnimationMouth(dreamMouth, 0);
    Otto.bendTones (100, 200, 1.04, 10, 10);

    if (buttonPushed) {
      break;
    }
    Otto.putAnimationMouth(dreamMouth, 1);
    Otto.bendTones (200, 300, 1.04, 10, 10);

    if (buttonPushed) {
      break;
    }
    Otto.putAnimationMouth(dreamMouth, 2);
    Otto.bendTones (300, 500, 1.04, 10, 10);

    delay(500);
//    digitalWrite(ledR, LOW);
  //  digitalWrite(ledG, LOW);
    //digitalWrite(ledB, LOW);
    if (buttonPushed) {
      break;
    }
    Otto.putAnimationMouth(dreamMouth, 1);
    Otto.bendTones (400, 250, 1.04, 10, 1);

    if (buttonPushed) {
      break;
    }
    Otto.putAnimationMouth(dreamMouth, 0);
    Otto.bendTones (250, 100, 1.04, 10, 1);

    delay(500);
  }

  if (!buttonPushed) {
    Otto.putMouth(lineMouth);
    Otto.sing(S_cuddly);
  }

  Otto.home();
  if (!buttonPushed) {
    Otto.putMouth(happyOpen);
  }
}
