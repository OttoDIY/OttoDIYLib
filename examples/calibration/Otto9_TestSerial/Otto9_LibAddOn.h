/*************************************************************************************
//-- Otto 9 Library Add Ons 
//
//-- Additional functions that aren't included in the recent Otto V9 Library 
//-- but needed to make the Otto_TestSerial sketch run properly with the
//-- alternated PINs configuration for the Touch-, Noise- and Distance- Sensors.
//
//
//-- Written for the OttoDIY Builders Community
//-- February 2021: (Birger T)
//   Sorry..But i'm not that CPP-Programmer, and I don't want to mess around with two
//   files (.h & .cpp) when it could be done in one .inc file (but .inc isn't allowed
//   in the Arduino IDE).
/*************************************************************************************

 Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
 Otto DIY invests time and resources providing open source code and hardware,
 please support by purchasing kits from (https://www.ottodiy.com)

 BSD license, all text above must be included in any redistribution

 ************************************************************************************/
#ifndef Otto_LibAddOn.h
#define Otto_LibAddOn.h
//--------------------------------------------------------- TEMPORARILY HOME FUNCTION 
// if you prefer the Humanoid's Arms Home positions to be not straight away as moving
// them downwards or upwards or what ever else, you can define the servo's pos inside
// "OttoX_config" and use "Otto_home();" instead of "Otto.home();"
void Otto_home() {
  if (Otto.getRestState() == false) { // Go to rest position only if necessary
    // All the servos at rest position
    int homes[6] = {90, 90, 90, 90, OTTO_LAPOS, OTTO_RAPOS};
    Otto._moveServos(500, homes);  // Move the servos in half a second

    Otto.detachServos();
    Otto.setRestState(true);
  }
} // Otto_home()

//------------------------------------------------------------ TOUCH SENSOR @ PIN 2 
//
// Don't ever miss a Touch Sensor's touch even when Otto is sleeping..! YEAH
// The AVR 328 has two Pins ( 2 & 3) that can generate an interrupt, so reconnect
// the Servos from these pin to other free pins and alter the PIN configuration 
// in your sketch (or Ottox_config.h) accordingly.
//
#if (PIN_Button==2) // only with Touch Sensor connected to the interrupt pin 2..
          // ..establish interrupt handling that counts edges of touch detection
volatile uint8_t touchedges = 0;
void  ISR_Button() { // touch sensor interrupt service routine
  if(touchedges < 254) touchedges++; // prevent OVF to 0 again and keep 1 bit in spare
  digitalWrite(PIN_Buzzer,digitalRead(PIN_Button)); // Touch feedback by the buzzer
}
// -------------------------------------------------- Start the Button's ISR
void  Otto_setButtonActiv() {
  attachInterrupt(digitalPinToInterrupt(PIN_Button), ISR_Button, CHANGE);
  touchedges = 0;
}
// -------------------------------------------------- Stopp the Button's ISR
void  Otto_setButtonInactiv() {
  detachInterrupt(digitalPinToInterrupt(PIN_Button));
}
// -------------------------------------------------- Get the Button Presses
uint8_t Otto_getButtonPresses() {
    uint8_t presses = (touchedges +1) / 2; // Roundabout
    touchedges = 0;
    return presses;
}
#else
void  Otto_setButtonActiv() { }
void  Otto_setButtonInactiv() { }
uint8_t Otto_getButtonPresses() {return 0;}
#endif // (PIN_Button==2)
// -- a missing library function..? ;-) --------------- Get the Button's State
#define Otto_getButton() digitalRead(PIN_Button)
//
//------------------------------------------------------------ NOISE SENSOR @ PIN 3 
//
// The noise sensor switches his signal output from high to low whenever a noise
// is detected that's level is higher as the levellimit ?? with the trimpot
// Because the level changes very fast and often, it is really impossible to catch
// an active noise state by a digitalRead(). So when the noise sensor is connected
// at the Arduino's pin 3 the state changes can be counted by an interrupt service.
// Within the sketch the amount of noise events can be read when it's time for it.
//
#if (PIN_Noise==3)// only with Noise Sensor connected to the interrupt pin 3..
          // ..establish interrupt handling that counts edges of noise detection
volatile uint8_t noiseedges = 0;
void  ISR_Noise() { // noise sensor interrupt service routine
  if(noiseedges < 254) noiseedges++; // prevent OVF to 0, keep 1 digit in spare
}
// -------------------------------------------------- Start the Noise detection ISR
// attaching the noise sensor's pin to the noise sensor ISR
// the ISR is called whenever the pin goes from low to high
void  Otto_setNoiseActiv() {
  attachInterrupt(digitalPinToInterrupt(PIN_Noise), ISR_Noise, RISING);
  noiseedges = 0;
}
// -------------------------------------------------- Stopp the Noise detection ISR
void  Otto_setNoiseInactiv() {
  detachInterrupt(digitalPinToInterrupt(PIN_Noise));
}
// ------------------------------------------------------- Get the Noises detected
uint8_t Otto_getNoiseDetected() {
    uint8_t noises = noiseedges; 
    noiseedges = 0;
    return noises;
}
#else
void  Otto_setNoiseActiv() { }
void  Otto_setNoiseInactiv() { }
uint8_t Otto_getNoiseDetected() {return 0;}
#endif // (PIN_Button==2)
// -- a missing library function..? ;-) ----------------- Get the Noise Sensor State
#define Otto_getNoise() digitalRead(PIN_Noise)
//
//
//--------------------------------------------------------------- ULTRA SOUND SENSOR 
//
// The ultrasound sensor has a trigger pin to start the measuring and an echo pin
// that's high as long the sensor waits for the echo. The two pins of the HC_SR04
// can be connected together, so that only one Arduino pin is needed for triggering
// and getting the pulseIn() result (PIN_Trigger == PIN_Echo).
// To save runtime the max distance that will be returned is limited to 86cm. 
//
uint8_t Otto_getDistance() {
  
  // 65535ms = 112,99m = far far away
  unsigned int dist = 0;
  // Trigger
  pinMode(PIN_UStrigg,OUTPUT);
  digitalWrite(PIN_UStrigg, HIGH);  // Trigger Impuls 10 us
  delayMicroseconds(10);    // Default 10
  digitalWrite(PIN_UStrigg, LOW);
  // Pingen
  pinMode(PIN_USecho,INPUT);
  // Echo Timeout = 5000ms = max. dist. 86cm
  // to get up to 255cm range set timeout <= 14790
  dist = pulseIn(PIN_USecho, HIGH, 5000); // Echo-Zeit messen
  //  pulseIn() returns 0, if echo time > timeout 
  if (!dist) dist = 5000;   // no echo - assume max distance
  dist = (dist+29) / 29 / 2;     // ms to cm distance
  //  Serial.print(dist);
  //  Serial.println("cm");
  return (byte)dist;
  
} // Otto_getDistance()


// display the predefined bitmap #idx of an eyepair 
// idx == 0..NUMBER_OF_EYEPAIRS - 1
void Otto_putEyepair(uint8_t idx) {
    Eyes.clear(); // I see clearly now.. Needed before new bmp
    Eyes.drawBitmap(0, 0,Eyepairstable[0] + idx *16,  8, 16, LED_ON);
    Eyes.writeDisplay();
} // Otto_putEyepair()

// Just to remember:
// display a predefined 8x8 bitmap #idx to the 'dir' side of the eyes
// #define LEFT        1
// #define RIGHT       -1
void Otto_putEye(uint8_t idx, int8_t dir) {
    // still to program
}

#endif // Otto_LibAddOn.h
