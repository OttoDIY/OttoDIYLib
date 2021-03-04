/*************************************************************************************
  //-- Otto Test Serial
  //
  //-- With this code you can test the beeper, the eyes and/or the mouth matrices
  //-- you can also test and calibrate the ultrasonic range and the microfon addon.
  //
  //-- Written for the OttoDIY Builders Community
  //-- February 2021: (Birger T)
  //
  //-- It is recommended to arrange the servo's and sensor's pin connection as
  //   described in the Otto9_config.h file!!
  //-- This sketch needs the following files included
  //    --  Otto9_FuncNames.h - containing the names of Lib functions to display
  //    --  Otto9_LibAddOn.h  - may be new functions to be included in the next 
  //                            Library Version, Pay Attention to dot vs underline: 
  //                            - Original Library function Otto.doSomething()
  //                            - My AddOn Library uses     Otto_doSomething()
  //    --  Otto9_config.h - Tell me please if you love or mislike it.. OPEN & READ
  //    --  Otto_eyes5.h   - in the past this file was missing in the library
  //                         No 5 - because many improvements until No 9 is reached
  /***********************************************************************************

     Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
     Otto DIY invests time and resources providing open source code and hardware,
     please support by purchasing kits from (https://www.ottodiy.com)

     BSD license, all text above must be included in any redistribution

 ************************************************************************************/
#include "Otto9_config.h"     // Otto Sketch Setup
//       "Otto_eyes5.h"       // included by Otto9_config.h
#include "Otto9_FuncNames.h"  // Display the names of selected Eyes, Mouths and/or Beeps
#include "Otto9_LibAddOn.h"   // Library Add ons for the Sensors
//
//----------------------------------------------------------------- //
#define TIMER_CNT 3
uint16_t
millisNow,              // holds the last read millis()
millisOld[TIMER_CNT],   // millis value at last action
msInterval[TIMER_CNT];  // millis to wait until next action
// Global variables used in all Testfunctions (always 5 chars long - my favorites)
uint8_t state = 0;  // Statemachine
uint8_t count = 0;  // Counter, Helper
uint8_t index = 0;  // Index for Imagedisplay
uint8_t level = 0;  // when 3 Variables aren't enough ;-)
//----------------------------------------------------------------- //
// because I have troubles with uploading sketches - there are Errors
// that could be or even not - I mark the upload with the last Build
// time to verify if the last sketch is uploaded (or still the prev.)
const char buildtime[] PROGMEM = {__TIME__};
//----------------------------------------------------------------- //
//
void fnHelp() {
  if (state) return;
  state = 1;
  Serial.print(F("\n\n\nOtto Test Sketch for Serial Monitor by Birger T \n"));
  print_PSTR(buildtime);  // see comment above
  Serial.print(F("\n With this sketch you can test the beeper, the eyes and/or the mouth matrices\n"
                 "you can also test and calibrate the ultrasonic range and the noise sensor addon.\n\n"
                 "\t\t-- to trim / test the Servos use the 'Otto Calibrate' Sketch first --\n\n"
                 "The interface uses single character menu controls\n\n"
                 "*!!* -> After sending a new command, you may have to wait (some seconds)\n"
                 "                               until Otto has finished his present action..*!!*\n\n"
                 "'H'/'h' as in Help or Home: this Message & Reset all to Home Positions\n"
                 "'1' - Select the Beeper ('+'&'-' to alter sounds)\n"
                 "'2' - Display states of Touch Sensor / Button\n"
                 "'3' - Display the US Distance Sensor's Values\n"
                 "'4' - Check the Noise Sensor\n"

                 "'6' - select the Mouth Matrix ('+'&'-' to alter display)\n"
                 "'7' - select the Eyes  Matrix ('+'&'-' to alter display)\n"
                 "'8' - Measure MCU Temperature & Pin A7 (Battery) Voltage\n"
                 "'9' - Playing some Gestures and going to 8 (did Temperature change?)\n"
                 "'0' - Switch the loop()'s duration watch ON/OFF\n"
                ));
} // void help()
//
//-----------------------------------------------  BUZZER --------- //
//
void fnBeeper() {

  switch (state) {
    case 0:
      // doing the "Blink Without Delay" sketch
      if (millisNow - millisOld[0] < 300) break;
      millisOld[0] = millisNow;
      if (count == LOW) count = HIGH;
      else              count = LOW;
      digitalWrite(PIN_Buzzer, count);
      break;

    case 1: // '+' increments 'state'
      // 'index' holds the index of sound to play
      // 'state' == 2 plays the sound
      index = 0;
      state = 2;
      break;
    // when sound was played wait in 'state' = 10
    case 2:
      state = 10;
      break;
    // user entered a '-' so 'state' is 10--
    case 9:
      if (index > 0) index--;
      state = 2;
      count = 0;
      break;
    case 10: break;
    // user entered a '+' so 'state' is 10++
    case 11:
      if (index < NUMBER_OF_SOUNDNAMES - 1) {
        index++;
        state = 2;
      }
      else {  // rollover to first sound
        state = 1;
      }
      break;
    default:
      state = 1;
      break;
  } // case()
  // Sound is played only once after selection changed
  if (state == 2) {
    Otto.sing(index);
    // show index and name in Serial Monitor
    Serial.print(index);
    Serial.print(F(" = "));
    print_PSTR(pgm_read_word(&singnames[index]));
  } // if (state == 2)
} // fnBeeper()
//--------------------------------------------  TOUCH SENSOR ------ //
//
// only when Touch sensor is connected to the interrupt pin 2
#if (PIN_Button==2)
// display ISR results when function is called from loop()
// Triggering the touchsensor lets the buzzer beep 
//                              (when no other sound is played)
void  fnButton() {
  if (millisNow - millisOld[0] >= 200) { // 5 times per second
    millisOld[0] = millisNow;
    level = Otto_getButtonPresses();
    if (level) {
      Serial.print(F("Touch Sensor is now "));
      if (Otto_getButton()) {
        Serial.print(F("ON "));
      }
      else {
        Serial.print(F("OFF "));
      }
      count = level;
      // Can you trigger the button more then 5 within 200ms?!
      if (count > 20) count = 20;
      while (count) {
        Serial.write('#');
        count--;
      }
      Serial.write(' ');
      Serial.println(level, DEC);
    } // if (level)
  } // if(millis)
} // fnTouch()
#else
// use Arduino function to get touch sensor's state
void  fnButton() {
  if (millisNow - millisOld[0] >= 100) { // 10 times per second
    millisOld[0] = millisNow;
    state = digitalRead(PIN_Button);
    if (state != count) { // edge detection (change of pin state)
      count = state;
      Serial.print(F("Touch Sensor is now "));
      if (state) {
        Serial.println(F("ON"));
      }
      else {
        Serial.println(F("OFF"));
      }
    } // if (edge)
  } // if(millis)
} // fnTouch()
#endif
//--------------------------------------  DISTANCE SENSOR  -------- //
//
#if (PIN_UStrigg == PIN_USecho)
// if US distance sensor's echo and trigger pins are connected
void  fnDistance() {
  if (millisNow - millisOld[0] >= 200) { // 5 times per second
    millisOld[0] = millisNow;
    uint8_t dist = Otto_getDistance();
    // for displaying a 'bargraph'..
    level = map(dist, 0, 80, 0, 40);
    Serial.print(F("Distance "));
    while (level--) {
      Serial.write('#');
    }
    Serial.write(' ');
    // ..and the measured distance
    Serial.println(dist, DEC);
  } // if(millis)
} // fnDistance()
#else
// use Otto lib function with two separate pins for echo and trigger
void  fnDistance() {
  if (millisNow - millisOld[0] >= 200) {
    millisOld[0] = millisNow;
    uint16_t dist = Otto.getDistance();
    level = map(dist, 0, 150, 0, 40);
    Serial.print(F("Distance "));
    while (level--) {
      Serial.write('#');
    }
    Serial.write(' ');
    Serial.println(dist, DEC);
  } // if(millis)
} // fnDistance()
#endif
//---------------------------------------------  NOISE SENSOR ---- //
//
#if (PIN_Noise==3)
// if the noise sensor is connected to pin 3 an interrupt service
// routine is 'attached' that counts the rising edges of the signal
void  fnNoise() {
  if (millisNow - millisOld[0] >= 200) { // 5 times per second
    millisOld[0] = millisNow;
    level = Otto_getNoiseDetected();
    if (level) {
      Serial.print(F("Noise is now "));
      if (digitalRead(PIN_Noise)) {
        Serial.print(F("quiet"));
      }
      else {
        Serial.print(F("loud"));
      }
      Serial.print(F("; Noises detected "));
      count = level;
      // constrain bargraph lenght
      if (count > 50) count = 50;
      while (count) {
        Serial.write('#');
        count--;
      }
      Serial.write(' ');
      Serial.println(level, DEC);
    } // if (level)
  } // if(millis)
} // fnNoise()
#else
// if the noise sensor isn't connected to the interrupt pin 3
// use the Otto library function to read the noise state
void  fnNoise() {
  if (millisNow - millisOld[0] >= 50) { // 20 x per Second
    millisOld[0] = millisNow;
    uint16_t noise = Otto.getNoise();
    //    uint16_t noise = analogRead(A6);
    level = map(noise, 0, 962, 0, 40);
    Serial.print(F("Noise "));
    while (level--) {
      Serial.write('#');
    }
    Serial.write(' ');
    Serial.println(noise, DEC);
  } // if(millis)
} // fnNoise()
#endif
//-----------------------------------------  MOUTH MATRIX --------- //
//  Displaying the predefined 6x5 pixel symbols and some gestures
void fnMouth() {

  switch (state) {
    case 0:
      // Some Matrix playing while waiting for user input
      // using millis() to be 'nonblocking'
      if (millisNow - millisOld[0] < 10) break;
      millisOld[0] = millisNow;
      // count is used as counter and state variable
      if (count < 64) {     // count = 0..63
        Otto.setLed(count / 8, count % 8, 1);
        count++;
      }
      else if (count < 128) { // count = 64..127
        count -= 64;          // count = 0..63
        Otto.setLed(count / 8, count % 8, 0);
        count += 64;          // count = 64..127
        count++;
      }
      else if (count < 192) {
        count -= 128;
        Otto.setLed(count % 8, count / 8, 1);
        count += 128;
        count++;
      }
      else {
        count -= 192;
        Otto.setLed(count % 8, count / 8, 0);
        count += 192;
        count++;
      }
      break;

    case 1: // '+' increments 'state'
      // 'index' holds the index of mouth to display
      // 'state' == 2 displays the mouth
      index = 0;
      state = 2;
      break;
    // when mouth was displayed wait in 'state' = 10
    case 2:
      state = 10;
      break;
    // playing the mouth animations
    // TIPP: if sending '++' from the serial monitor
    // it'll go directly to step 3..
    case 3:
      Serial.println(F("Otto is sleeping"));
      Otto.playGesture(OttoSleeping);
      break;
    case 4:
      Serial.println(F("Otto the magician.."));
      Otto.playGesture(OttoMagic);
      break;
    case 5:
      Serial.println(F("Otto is waving"));
      Otto.playGesture(OttoWave);
      break;
    case 6:
      // rollover to first mouth
      state = 2;
      index = 0;
      break;
    // user entered a '-' so 'state' is 10--
    case 9:
      if (index > 0) index--;
      state = 2;
      break;
    case 10: break;
    // user entered a '+' so 'state' is 10++
    case 11:
      if (index < NUMBER_OF_MOUTHNAMES - 1) {
        index++;
        state = 2;
      }
      else { // made all static mouths - go to animations
        state = 3;
      }
      break;
    default:
      state = 1;
      break;
  } // case()
  // Mouth is displayed only once after selection changed
  if (state == 2) {
    Otto.clearMouth();
    Otto.putMouth(index);
    Serial.print(index);
    Serial.print(F(" = "));
    print_PSTR(pgm_read_word(&mouthnames[index]));
  }
} // fnMouth()
//------------------------------------------  EYES MATRIX --------- //
//  Displaying predefined 16x8 pixel symbols found in the examples
void fnEyes() {

  switch (state) {

    case 0:
      // Some Matrix playing while waiting for user input
      // using millis() to be 'nonblocking'
      if (millisNow - millisOld[0] < 10) break;
      millisOld[0] = millisNow;
      // count is used as counter and state variable
      if (count < 128) {     // count = 0..127
        Eyes.drawPixel(count / 16, count % 16, LED_ON);
        Eyes.writeDisplay();
        count++;
      }
      else {
        count -= 128;
        Eyes.drawPixel(count % 8, count / 8, LED_OFF);
        Eyes.writeDisplay();
        count += 128;
        count++;
      }
      break;

    case 1: // '+' increments 'state'
      // 'index' holds the index of eyes to display
      // 'state' == 2 displays the eyes
      index = 0;
      state = 2;
      break;
    // when eyes were displayed wait in 'state' = 10
    case 2:
      state = 10;
      break;
    // user entered a '-' so 'state' is 10--
    case 9:
      if (index > 0) index--;
      state = 2;
      break;
    case 10: break;
    // user entered a '+' so 'state' is 10++
    case 11:
      if (index < NUMBER_OF_EYEPAIRS - 1) {
        index++;
      }
      else {
        index = 1;
      }
      state = 2;
      break;
    default:  // something may have gone wrong or not
      state = 1;
      break;
  } // case()
  
  // Eyes are updated only once after the selection changed
  if (state == 2) {
    // Similiar to 'Otto.putMouth(index)'
    Otto_putEyepair(index);
    // Printing index and name to Serial Monitor
    Serial.print(index);
    Serial.print(F(" = "));
    print_PSTR(pgm_read_word(&eyesnames[index]));
    // Sorry for the delays.. but I Love this lil joke
    if (index == 10) {
      for (int i = 0; i < 3; i++) {
        delay(400);
        Eyes.setBrightness(8);
        delay(400);
        Eyes.setBrightness(3);
      }
    } // if(index == 10)
  } // if (state == 2)
} // fnEyes()
//
//------------------------------- MCU Temperature ----------------- //
// See ATmega48,88,168,328 Datasheet chapter "21.8 Temperature Measurement"
// and Application Note "AVR 122" (download from MicroChip Site)
void showTemperature() {  // Read & show the MCU's buildin Temperature
  uint32_t TmpVal;
  ADMUX   = 0b11001000;     // Ref = 1,1V, select ADC #8
  // because a capacitor should be connected to AREF..wait for it
  delay(10);
  ADCSRA |= 0b11000000;     // enable & start ADC
  while (ADCSRA & 0b01000000) {
    ; // Wait for Data
  }
  TmpVal = ADCW;    //(ADCH << 8) | ADCL;
  ADCSRA |= 0b01000000;     // start ADC
  while (ADCSRA & 0b01000000) {
    ; // Wait for Data
  }
  TmpVal = ADCW;    //(ADCH << 8) | ADCL;
  TmpVal *= 1100;   // VREF [mV]
  TmpVal /= 1024;   // ADC resolution
  TmpVal -= (314 - 25); // typical mV @ 25°C - 25K
  Serial.print(F("Otto's MCU Core Temperatur = roundabout "));
  Serial.print(TmpVal, DEC);
  Serial.println(F(" °C"));
} // showTemperature()
//------------------------------- MCU Voltage ----------------- //
// See MicroChip Application Note "AN2447 Measuring Voltage.."
uint16_t MCU_VCC_mv; // hold the MCU Voltage for further calculations
//
void showVoltage() {
  uint32_t VccVal;
  ADMUX   = 0b01001110;     // Ref = Ub, ADC = 1,1Vbg
  delay(10);
  // wait for input from Serial monitor - time to measure the
  // changing voltage at AREF pin with a multimeter
  //  while (Serial.available()) Serial.read();
  //  while (!Serial.available()); Serial.read();
  ADCSRA |= 0b11000000;     // enable & start ADC
  while (ADCSRA & 0b01000000) {
    ; // Wait for Data
  }
  VccVal = ADCW;    //(ADCH << 8) | ADCL;
  ADCSRA |= 0b01000000;     // start ADC
  VccVal = 1024UL * 1100UL;     // Prepare Calculation
  while (ADCSRA & 0b01000000) {
    ; // Wait for Data
  }
  MCU_VCC_mv = ADCW;   //(ADCH << 8) | ADCL;
  if (MCU_VCC_mv) { // to prevent DIV0!
    VccVal /= MCU_VCC_mv;
    MCU_VCC_mv = VccVal;
  }
  Serial.print(F("Otto's real supply voltage = "));
  Serial.print(MCU_VCC_mv, DEC);
  Serial.println(F(" mV"));
} // showVoltage()
//------------------------------- Battery Pin ---------------- //

void fnVolt() {
  if (state) return;
  state = 1;
  uint16_t mVhold;
  uint32_t mVolts;
  uint8_t saveRef = ADMUX;  // save Arduino core's ADC settings
  showTemperature();
  showVoltage();
  ADMUX = saveRef;      // restore Arduino core's ADC settings
  delay(10);            // adc regeneration pause
  // Reading value at battery pin A7..
  mVhold = analogRead(A7);
  // ..calculating with AREF == Vcc assuming 5,0V
  mVolts = mVhold;
  mVolts *= 5000UL;
  mVolts /= 1024UL;
  Serial.print(F("voltage @pin A7 (Vcc 5,0V) = "));
  Serial.print(mVolts, DEC);
  Serial.print(F(" mV & w. real Vcc = "));
  // ..calculating with real measured Vcc @ AREF
  mVolts = mVhold;
  mVolts *= MCU_VCC_mv;
  mVolts /= 1024UL;
  Serial.print(mVolts, DEC);
  Serial.println(F(" mV "));
  // Reading Battery Voltage & Level through Otto Lib will no longer be supported
//  Serial.print(F("Otto Battery Voltage = "));
//  Serial.print(Otto.getBatteryVoltage(), DEC);
//  Serial.println(F(" V"));
//  Serial.print(F("Otto Battery Level = "));
//  Serial.print(Otto.getBatteryLevel(), DEC);
//  Serial.println(F(" %"));
} // fnVolt()
//------------------------------------------- loop() durations  --- //
// Keep track of the loop() durations .. or not
uint8_t loopWatchSwitch = LOW;

uint16_t loopWatch(uint16_t Now, uint16_t Inv) {

  static uint16_t Prev, Act = 0;
  static uint16_t loopMax = 0;
  static uint16_t loopMid = 0;
  static uint16_t loopMin = 0xFFFF;

  uint16_t loopduration = Now - Prev;
  Prev = Now;
  // store min- and max values
  if (loopduration > loopMax) loopMax = loopduration;
  if (loopduration < loopMin) loopMin = loopduration;
  loopMid += loopduration;
  loopMid += 1;     // to round the /2
  loopMid /= 2;
  if (loopWatchSwitch) {
    // Show results every Inv/1000 seconds
    if ((Now - Act) >= Inv) { // every 2 seconds
      Act = Now;
      Serial.print(F("Loopdurations min "));
      Serial.print(loopMin, DEC);
      Serial.print(F(" ..average "));
      Serial.print(loopMid, DEC);
      Serial.print(F(" ..max "));
      Serial.print(loopMax, DEC);
      Serial.println(F(" ms"));
      loopMax = 0;
      loopMid = 0;
      loopMin = 0xFFFF;
    } // if (Now - Act)
  } // if(loopWatchSwitch)
  return loopduration;
} // loopWatch()

//----------------------------------------------------------------- //
//  'processFunction' is a pointer variable, it contains
//  the adress of the entry to the function selected
void (*processFunction)() = &fnHelp;
//----------------------------------------------------------------- //
void setFunction(void (*function)()) { //     INITIALIZE FUNCTIONS
  //----------------------------------------------------------------- //
  processFunction = function;  // copy the function's adress to pointer
  state = 0;                   // and reset all global variables
  count = 0;
  index = 0;
  millisOld[0] = millisNow;    // set current millis
}
//----------------------------------------------------------------- //
void processChar(char c) { //                 PROCESS INPUT CHAR
  //----------------------------------------------------------------- //

  switch (c) {
    case 'h':
    case 'H':
      setFunction(&fnHelp);
      Otto.clearMouth();
      Eyes.clear();
      Eyes.writeDisplay();
      Otto_home();
      break;
    case '1':
      Serial.println(F("Selected Beeper\n"
                       "Index = Name"));
      setFunction(&fnBeeper);
      break;
    case '2':
      Serial.println(F("Testing Touch Sensor"));
      setFunction(&fnButton);
      count = 1;  // to 'detect an edge' on first call
      break;
    case '3':
      Serial.println(F("Selected US Distance Sensor"));
      setFunction(&fnDistance);
      break;
    case '4':
      Serial.println(F("Testing Noise Sensor"));
      setFunction(&fnNoise);
      break;
    case '6':
      Serial.println(F("Selected Mouth Matrix\n"
                       "Index = Name"));
      setFunction(&fnMouth);
      break;
    case '7':
      Serial.println(F("Selected Eye Matrix\n"
                       "Index = Name"));
      setFunction(&fnEyes);
      break;
    case '9':
      Serial.println(F("Playing Gesture 'OttoFail'.."));
      Otto.playGesture(OttoFail);
      Serial.println(F("..and now 'OttoAngry'.."));
      Otto.playGesture(OttoAngry);
      Serial.println(F("..let's go on to.."));
    case '8':
      Serial.println(F("Selected Voltages & Temperature"));
      setFunction(&fnVolt);
      break;
    case '0':
      Serial.print(F("Watching Loop() Duration = "));
      if (!loopWatchSwitch) {
        loopWatchSwitch = HIGH;
        Serial.println(F("ON"));
      }
      else {
        loopWatchSwitch = LOW;
        Serial.println(F("OFF"));
      }
      break;
    case '+':
      state++;
      break;
    case '-':
      if (state > 1) state--;
      break;
    case ' ':
      delay(100);
      break;
    case '\n':
    case '\r':
      break;
    default:
      Serial.print(F("Unknown command ("));
      Serial.print(c);
      Serial.println(F(")"));
      break;
  }
} // processChar()

//----------------------------------------------------------------- //
void setup(void) { //                                       SETUP
  //----------------------------------------------------------------- //

  // Contains all functioncalls needed to start up the Otto
  Otto_Start();

  // Dimm the Eyes Matrix
  Eyes.setBrightness(3);

  // Start with sleeping Otto until user inputs from Serial Monitor
  //  setFunction(&fnMouth);
  //  state = 3;

  // attaching the external interrupts to PIN 2 and PIN 3 when needed
#if (PIN_Button==2)
  Otto_setButtonActiv();
#endif
#if (PIN_Noise==3)
  Otto_setNoiseActiv();
#endif

} // setup()

//----------------------------------------------------------------- //
void loop() {  //                                            LOOP
  //----------------------------------------------------------------- //
  // we won't mess around with unsigned long or uint32_t
  millisNow = (uint16_t)millis();
  // Watching the loop duration all 5000ms
  loopWatch(millisNow, 5000);

  // get user input from Serial port
  while (Serial.available())  {
    processChar(Serial.read());
  }
  //  processFunction is a pointer variable, it contains
  //  the adress of the function the user selected
  processFunction();

} // loop()
//----------------------------------------------------------------- //
//----------------------------------------------------------------- //
