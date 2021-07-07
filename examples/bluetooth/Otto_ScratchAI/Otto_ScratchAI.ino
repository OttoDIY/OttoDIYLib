/**
 * Otto DIY Scratch AI firmware with standard baudrate of 9600 for BLE Bluetooth modules.
 * Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https:// www.ottodiy.com)
 * If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be
 * included in any redistribution in accordance with the GPL Version 2 when your application is distributed. See http:// www.gnu.org/copyleft/gpl.html
 *
 * ADDED Progmem for MOUTHS and GESTURES: Paul Van De Veen October 2018
 * ADDED PIN definitions for ease of use: Jason Snow November 2018
 * ADDED Battery meassurementin in mode 3 Jason Snow August 2019
 * ADDED changed to original Software serial library Camilo Parra May 2020
 * DELETED interrupts and modes to use BTserial Camilo Parra May 2020
 * ADDED BLE communication for Scratch AI Jorge Gonzalez August 2020 https:// ottoschool.com/scratch/
 * ADDED LEDs Neo pixel control for fun by Jorge Gonzalez August 2020
 * ADDED LEDs RF 433Mhz to control remote 4 Relay module by Jorge Gonzalez November 2020
 * CHANGED name of SerialCommand libraries so that this code needs dont interfeer with App code by Camilo Parra March 2021
 *
 *
 *                     Otto Biped                                          Otto Humanoid  
 *                     ╭──────╮                                            ╭──────╮
 *                     │  O   O  │                                           │  O   O  │
 *                     ├─────┤                       RIGHT ARM 7  ━┭───┤  ╭───╮  ├───┮━  LEFT ARM 6
 *                     │         │                                  ━└───┼──╯     ╰──┼───┘━
 *     RIGHT LEG  3    ├─┬─┬─┤    LEFT LEG  2                             ├─┬─┬─┤
 *                     ├─┤  ├─┤                      RIGHT LEG 3           ├─┤  ├─┤     LEFT LEG 2
 *     RIGHT FOOT 5  ━━┷━━┙    ┕━━┷━━  LEFT FOOT 4                              ━━┷━━┙   ┕━━┷━━
 *                                                                 RIGHT FOOT 5       LEFT FOOT 4                
 *
 * Otto Modules
 *
 * To include code to the module installed in your Otto, uncomment the corresponding line.
 * Remember that the more components you add, the larger the compiled code will be.
 * Each Otto model has different components, for example:
 * Otto Humanoid has two additional servos and an 8x8 LED matrix for his mouth,
 * while Otto Eyes has a 16x8 matrix for his eyes.
 */

#define BLUETOOTH          // Uncomment this line to include code to Bluetooth module
// #define EYES_MATRIX        // Uncomment this line to include code to 16x8 LED matrix
#define MOUTH_MATRIX       // Uncomment this line to include code to 8x8 LED matrix
#define TOUCH_SENSOR       // Uncomment this line to include code to touch sensor
#define NOISE_SENSOR       // Uncomment this line to include code to noise sensor
#define ULTRASONIC_SENSOR  // Uncomment this line to include code to ultrasonic sensor
#define BUZZER             // Uncomment this line to include code to buzzer
#define SERVOS             // Uncomment this line to include code to servos
// #define FUNNY_LED          // Uncomment this line to include code to Funny led
// #define RADIO_FREQUENCY    // Uncomment this line to include code to legs servos
// #define RADIO_HEAD     // Uncomment this line if using RadioHead library
// #define VIRTUAL_WIRE   // Uncomment this line if using VirtualWire library
// #define RC_SWITCH      // Uncomment this line if using rc-switch library
// #define RF_TRANSMITTER // Uncomment this line if using RFTransmitter

#include <EEPROM.h>
#include <string.h>
#include <SerialCommandAI.h> // Library to manage serial commands
#include <Otto.h>
#ifdef EYES_MATRIX
    #include "Adafruit_LEDBackpack.h"
#endif
#ifdef RADIO_FREQUENCY
    #ifdef RF_TRANSMITTER
        #include <RFTransmitter.h>
    #endif
    #ifdef RADIO_HEAD
        #include <RH_ASK.h>
        #ifdef RH_HAVE_HARDWARE_SPI
            #include <SPI.h> // Not actually used but needed to compile
        #endif
    #endif
    #ifdef VIRTUAL_WIRE
        #include <VirtualWire.h>
    #endif
    #ifdef RC_SWITCH
        #include <RCSwitch.h>
    #endif
#endif

Otto Otto;  // This is Otto!

/**
 * Reserved PINs
 */
#define PIN_RX          0
#define PIN_TX          1

/**
 * Servo PINs
 */
#define LeftLeg 2 
#define RightLeg 3
#define LeftFoot 4 
#define RightFoot 5 
#define PIN_ARM_LEFT    6 // servo[4]  Left arm
#define PIN_ARM_RIGHT   7 // servo[5]  Right arm

/**
 * RF 433Mhz Module
 */
#define RF_SPEED     1000
#define PIN_RF_TX       6 // Check conflict with arms in Otto Humanoid
#define PIN_RF_RX       7 // Check conflict with arms in Otto Humanoid

/**
 * Buzzer PIN
 */
#define Buzzer  13 

/**
 * Ultrasonic PINs
 */
#define PIN_US_TRIGGER  8 // TRIGGER pin (8)
#define PIN_US_ECHO     9 // ECHO pin (9)

/**
 * BT/BLE Module PINs
 */
#define PIN_BLE_STATE  10 // optional
#define PIN_BLE_TX     11
#define PIN_BLE_RX     12
#define PIN_BLE_ENABLE 13 // optional

/**
 * Touch Sensor or Push Button
 */
#define PIN_BUTTON     A0 // TOUCH SENSOR Pin (A0) PULL DOWN RESISTOR MAYBE REQUIRED to stop false interrupts (interrupt PIN)

/**
 * Mouth LED Matrix PINs
 */
#define PIN_MOUTH_CLK  A1 // CLK pin (A1)
#define PIN_MOUTH_CS   A2 // CS  pin (A2)
#define PIN_MOUTH_DIN  A3 // DIN pin (A3)
#define LED_DIRECTION   1 // LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1

/**
 * Sound Sensor PIN
 */
#define PIN_NOISE      A6  // SOUND SENSOR   ANALOG pin (A6)

/**
 * Eyes LED Matrix PINs
 */
#define SCL // CLOCK
#define SDA // DATA

/**
 * Global Variable1
 */

SoftwareSerial BTserial = SoftwareSerial(PIN_BLE_TX, PIN_BLE_RX); //  TX  RX of the Bluetooth
SerialCommandAI SCmd(BTserial);  // The SerialCommand object

const char programID[] = "OttoScratchAI-v1"; // Each program will have a ID
const char factory_name = '$'; // Factory name
const char first_name = '#'; // First name

/**
 * Eyes LED Matrix
 */
#ifdef EYES_MATRIX
    Adafruit_8x16matrix eyesMatrix = Adafruit_8x16matrix();
#endif

/**
 * RF 433Mhz Module
 */
#ifdef RADIO_FREQUENCY
    #ifdef RF_TRANSMITTER
        RFTransmitter transmitter(PIN_RF_TX, 1);
    #endif
    #ifdef RADIO_HEAD
        RH_ASK driver(RF_SPEED, PIN_RF_RX, PIN_RF_TX, 0);
    #endif
    #ifdef RC_SWITCH
        RCSwitch rcSwitch = RCSwitch();
    #endif
#endif

/**
 * Movement parameters
 */
int duration = 1000;     // Initial duration of movement
int moveID = 0;          // Number of movement
int moveSize = 15;       // Associated with the height of some movements
int steps = 1;           // Number of steps

/**
 * Button
 */
volatile bool buttonPushed = false;  // Variable to remember when a button has been pushed
bool obstacleDetected = false;

/**
 * LEDs Matrix 8x8 MAX7219ENG
 */
unsigned long int mouthMatrix;

/**
 * LED Matrix images 8x16
 */
#ifdef EYES_MATRIX
static const uint8_t PROGMEM
logo_bmp[]     = { B01111110,B10000001,B10111001,B10101001,B10111001,B10010001,B10111001,B10010001,B10010001,B10111001,B10010001,B10111001,B10101001,B10111001,B10000001,B01111110},
happy_bmp[]    = { B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000,B00000000,B00111100,B00000010,B00000010,B00000010,B00000010,B00111100,B00000000},
eyes_bmp[]     = { B00000000,B00111100,B01111110,B01100110,B01100110,B01111110,B00111100,B00000000,B00000000,B00111100,B01111110,B01100110,B01100110,B01111110,B00111100,B00000000},
touch_bmp[]    = { B00000000,B00111100,B01110010,B01110010,B01111110,B01111110,B00111100,B00000000,B00000000,B00111100,B01110010,B01110010,B01111110,B01111110,B00111100,B00000000},
sad_bmp[]      = { B00000000,B00110000,B01111000,B01001100,B01001110,B01111110,B00111100,B00000000,B00000000,B00111100,B01111110,B01001110,B01001100,B01111000,B00110000,B00000000},
xx_bmp[]       = { B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00010100,B00001000,B00010100,B00100010,B00000000},
XX_bmp[]       = { B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001,B00000000,B00000000,B01000001,B00100010,B00010100,B00001000,B00010100,B00100010,B01000001},
angry_bmp[]    = { B00000000,B00111100,B01111110,B01100110,B01100100,B01111000,B00110000,B00000000,B00000000,B00110000,B01111000,B01100100,B01100110,B01111110,B00111100,B00000000},
angry2_bmp[]   = { B00000000,B00000010,B00000100,B00001000,B00010000,B00100000,B00000000,B00000000,B00000000,B00000000,B00100000,B00010000,B00001000,B00000100,B00000010,B00000000},
sleep_bmp[]    = { B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000,B00000000,B00000000,B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00000000},
freetful_bmp[] = { B00000000,B00111100,B01100110,B01100110,B01111100,B01111000,B00110000,B00000000,B00000000,B00111100,B01100110,B01100110,B01111100,B01111000,B00110000,B00000000},
love_bmp[]     = { B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000,B00000000,B00001100,B00011110,B00111100,B00111100,B00011110,B00001100,B00000000},
confused_bmp[] = { B00000000,B01111100,B10000010,B10111010,B10101010,B10001010,B01111000,B00000000,B00000000,B01111100,B10000010,B10111010,B10101010,B10001010,B01111000,B00000000},
wave_bmp[]     = { B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000,B00000000,B00100000,B00010000,B00001000,B00010000,B00100000,B00010000,B00000000},
magic_bmp[]    = { B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111110,B11111111,B01111110,B00000000,B00000000},
fail_bmp[]     = { B00000000,B00110000,B01111000,B01111000,B01111100,B00111100,B00001000,B00000000,B00000000,B00001000,B00111100,B01111100,B01111000,B01111000,B00110000,B00000000},
full_bmp[]     = { B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};
#endif

/**
 * BLE Connection status
 */
int msToDefineConnection = 1500; // some time longer than the pulse
long highStartTimestamp;
int lastConnState = LOW;
bool confirmedConnected = false;

/**
 * Touch Sensor
 */
int lastTouchState = LOW;
int currentTouchState;

/**
 * Setup
 */
void setup() {
    // Serial communication initialization
    Serial.begin(9600);

    // Bluetooth communication initialization
    BTserial.begin(9600);
    pinMode(PIN_BLE_STATE, INPUT);

    // Otto initialization
  Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); //Set the servo pins and Buzzer pin

    // Mouth LED Matrix initialization
#ifdef MOUTH_MATRIX
    Otto.initMATRIX(PIN_MOUTH_DIN, PIN_MOUTH_CS, PIN_MOUTH_CLK, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation
    Otto.matrixIntensity(5); // set up Matrix display intensity
#endif

    // Eyes LED Matrix initialization
#ifdef EYES_MATRIX
    eyesMatrix.begin(0x70); // pass in the address
    eyesMatrix.setBrightness(0);
    //eyesMatrix.setRotation(1);
#endif

    // Touch Sensor initialization
#ifdef TOUCH_SENSOR
    pinMode(PIN_BUTTON, INPUT); // - ensure pull-down resistors are used
#endif

    // RF 433Mhx Module initialization
#ifdef RADIO_FREQUENCY
    #ifdef VIRTUAL_WIRE
        vw_set_tx_pin(PIN_RF_TX);
        vw_set_rx_pin(PIN_RF_RX);
        vw_setup(RF_SPEED);	 // Bits per sec
        if (!driver.init())
            Serial.println("init failed");
    #endif
    #ifdef RC_SWITCH
        // Transmitter is connected to Arduino Pin #10  
        rcSwitch.enableTransmit(PIN_RF_TX);
        // Optional set protocol (default is 1, will work for most outlets)
        rcSwitch.setProtocol(1);
        // Optional set pulse length.
        //rcSwitch.setPulseLength(350);  // Default 350 us
        // Optional set number of transmission repetitions.
        //rcSwitch.setRepeatTransmit(4); // Default 4
    #endif
#endif


    // Setup callbacks for SerialCommand to Actuators
    SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
    SCmd.addCommand("L", receiveMouth);     //  sendAck & sendFinalAck
    SCmd.addCommand("O", receiveEyes);      //  sendAck & sendFinalAck
    SCmd.addCommand("T", receiveBuzzer);    //  sendAck & sendFinalAck
    SCmd.addCommand("K", receiveSing);      //  sendAck & sendFinalAck
    SCmd.addCommand("H", receiveGesture);   //  sendAck & sendFinalAck
    SCmd.addCommand("M", receiveMovement);  //  sendAck & sendFinalAck
    SCmd.addCommand("C", receiveTrims);     //  sendAck & sendFinalAck
    SCmd.addCommand("G", receiveServo);     //  sendAck & sendFinalAck
    SCmd.addCommand("R", receiveName);      //  sendAck & sendFinalAck
    SCmd.addCommand("F", receiveRelay);     //  sendAck & sendFinalAck
    SCmd.addCommand("X", receiveText);     //  sendAck & sendFinalAck

    // Setup callbacks for SerialCommand to Sensors
    //SCmd.addCommand("E", requestName);
    //SCmd.addCommand("D", requestDistance);
    //SCmd.addCommand("N", requestNoise);
    //SCmd.addCommand("I", requestProgramId);

    SCmd.addDefaultHandler(receiveStop);

    // Otto wake up!
    Otto.sing(S_connection);
    Otto.home();
    // Animation Uuuuuh - A little moment of initial surprise
    for (int i = 0; i < 2; i++) {
        for (int i = 0; i < 8; i++) {
#ifdef MOUTH_MATRIX
            Otto.putAnimationMouth(littleUuh, i);
#endif
            delay(150);
        }
    }
    // Smile for a happy Otto :)
#ifdef MOUTH_MATRIX
    Otto.putMouth(smile);
#endif
    Otto.sing(S_happy);
    delay(200);

    // If Otto's name is '#' means that Otto hasn't been baptized
    // In this case, Otto does a longer greeting
    // 5 = EEPROM address that contains first name character
    if (EEPROM.read(5) == first_name) {
        Otto.jump(1, 700);
        delay(200);
        Otto.shakeLeg(1, duration, 1);
#ifdef MOUTH_MATRIX
        Otto.putMouth(smallSurprise);
#endif
        Otto.swing(2, 800, 20);
        Otto.home();
    }

#ifdef MOUTH_MATRIX
    Otto.putMouth(happyOpen);
#endif

    Otto.home();
#ifdef EYES_MATRIX
    eyesMatrix.setTextSize(1);
    eyesMatrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
    eyesMatrix.setTextColor(LED_ON);
    eyesMatrix.setRotation(1);
    for (int8_t x=7; x>=-90; x--) {
      eyesMatrix.clear();
      eyesMatrix.setCursor(x,0);
      eyesMatrix.print("Otto Scratch AI");
      eyesMatrix.writeDisplay();
      delay(50);
    }
    eyesMatrix.setRotation(0);

    eyesMatrix.clear();
    eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
    eyesMatrix.writeDisplay();
#endif
}

/**
 * Main Loop
 */
void loop() {
    checkIfTouched();
    checkIfConnected();
    SCmd.readSerial();    // If Otto is moving yet
    if (Otto.getRestState() == false) {
        move(moveID);
    }
}

/**
 * Functions
 */

/**
 * Function to read distance sensor & to actualize obstacleDetected variable
 */

/**
 * Function to receive Stop command.
 */
void receiveStop() {
    sendAck();
    Otto.home();
    sendFinalAck();
}

/**
 * Function to receive LED commands
 */
void receiveMouth() {
#ifdef MOUTH_MATRIX
    // sendAck & stop if necessary
    sendAck();
    Otto.home();
    // Examples of receiveMouth Bluetooth commands
    // L 000000001000010100100011000000000
    char *arg;
    char *endstr;
    arg = SCmd.next();
    Serial.println(arg);
    if (arg != NULL) {
        mouthMatrix = strtoul(arg, &endstr, 2); // Converts a char string to unsigned long integer
#ifdef MOUTH_MATRIX
        Otto.putMouth(mouthMatrix, false);
#endif
    } else {
#ifdef MOUTH_MATRIX
        Otto.putMouth(xMouth);
#endif
        delay(2000);
#ifdef MOUTH_MATRIX
        Otto.clearMouth();
#endif
    }
    sendFinalAck();
#endif
}

/**
 * Function to receive Eyes Matrix LED commands
 */
void receiveEyes() {
#ifdef EYES_MATRIX
    Serial.println("receiveEyes");
    // sendAck & stop if necessary
    sendAck();
    Otto.home();
    // Examples of receiveMouth Bluetooth commands
    // O FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    char *arg;
    uint8_t image_bmp[16];
    arg = SCmd.next();
    Serial.print("arg=");
    Serial.println(arg);
    if (arg != NULL) {
        hex2dec(arg, image_bmp);
        eyesMatrix.clear();
        eyesMatrix.drawBitmap(0, 0, +image_bmp, 8, 16, LED_ON);
        eyesMatrix.writeDisplay();
    } else {
        //Otto.
        delay(2000);
        //Otto.
    }
    sendFinalAck();
    BTserial.println("O:ACK");
#endif
}

/**
 * Function to receive Eyes Matrix LED commands
 */
void receiveText() {
#ifdef EYES_MATRIX
    Serial.println("receiveText");
    // sendAck & stop if necessary
    sendAck();
    Otto.home();
    // Examples of receiveMouth Bluetooth commands
    // X|Otto Scratch AI
    char *arg;
    arg = SCmd.next();
    uint8_t  msgLength = strlen(arg) * 6;
    Serial.print("msgLenght: ");
    Serial.println(msgLength);

    if (arg != NULL) {
/*
        eyesMatrix.setTextSize(1);
        eyesMatrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
        eyesMatrix.setTextColor(LED_ON);
        eyesMatrix.setRotation(1);
        for (int8_t x=16; x>=-msgLength; x--) {
          eyesMatrix.clear();
          eyesMatrix.setCursor(x,0);
          eyesMatrix.print(arg);
          eyesMatrix.writeDisplay();
          delay(50);
        }
        eyesMatrix.setRotation(0);
        Otto.home();
*/

        eyesMatrix.setTextSize(1);
        eyesMatrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
        eyesMatrix.setTextColor(LED_ON);
        eyesMatrix.setRotation(1);

        char subbuff[17];
        int i = 0;
        do {
           memcpy( subbuff, &arg[i], 17);
           subbuff[16] = '\0';

           for (int8_t x=0; x>=-4; x--) {
             eyesMatrix.clear();
             eyesMatrix.setCursor(x,0);
             eyesMatrix.print(subbuff);
             eyesMatrix.writeDisplay();
             delay(50);
           }
        } while (arg[i++] != '\0');
           eyesMatrix.setRotation(0);
    } else {
        //Otto.
        delay(2000);
        //Otto.
    }
    sendFinalAck();
    BTserial.println("X:ACK");
#endif
}

/**
 *Function to receive buzzer commands
 */
void receiveBuzzer() {
#ifdef BUZZER
    // sendAck & stop if necessary
    sendAck();
    Otto.home();

    bool error = false;
    int frec;
    int duration;
    char *arg;

    arg = SCmd.next();
    if (arg != NULL) frec = atoi(arg);  // Converts a char string to an integer
    else error = true;

    arg = SCmd.next();
    if (arg != NULL) duration = atoi(arg);  // Converts a char string to an integer
    else error = true;
    if (error == true) {
#ifdef MOUTH_MATRIX
        Otto.putMouth(xMouth);
#endif
        delay(2000);
#ifdef MOUTH_MATRIX
        Otto.clearMouth();
#endif
    } else Otto._tone(frec, duration, 1);
    sendFinalAck();
    BTserial.print("T:ACK");
#endif
}

/**
 * Function to receive TRims commands
 */
void receiveTrims() {
#ifdef SERVOS
    // sendAck & stop if necessary
    sendAck();
    Otto.home();
    int trimLegLeft, trimLegRIGHT, trimFootLeft, trimFootRight;

    // Definition of Servo Bluetooth command
    // C trimLegLeft trimLegRIGHT trimFootLeft trimFootRight
    // Examples of receiveTrims Bluetooth commands
    // C 20 0 -8 3
    bool error = false;
    char *arg;
    arg = SCmd.next();
    if (arg != NULL) trimLegLeft = atoi(arg);  // Converts a char string to an integer
    else error = true;

    arg = SCmd.next();
    if (arg != NULL) trimLegRIGHT = atoi(arg);  // Converts a char string to an integer
    else error = true;

    arg = SCmd.next();
    if (arg != NULL) trimFootLeft = atoi(arg);  // Converts a char string to an integer
    else error = true;

    arg = SCmd.next();
    if (arg != NULL) trimFootRight = atoi(arg);  // Converts a char string to an integer
    else error = true;
    if (error == true) {
#ifdef MOUTH_MATRIX
        Otto.putMouth(xMouth);
#endif
        delay(2000);
#ifdef MOUTH_MATRIX
        Otto.clearMouth();
#endif

    } else { // Save it on EEPROM
        Otto.setTrims(trimLegLeft, trimLegRIGHT, trimFootLeft, trimFootRight);
        Otto.saveTrimsOnEEPROM(); // Uncomment this only for one upload when you finaly set the trims.
    }
    sendFinalAck();
#endif
}

/**
 * Function to receive Servo commands
 */
void receiveServo() {
#ifdef SERVOS
    sendAck();
    moveID = 30;

    // Definition of Servo Bluetooth command
    // G  servoLegLeft servoLegRight servoFootLeft servoFootRight
    // Example of receiveServo Bluetooth commands
    // G 90 85 96 78
    bool error = false;
    char *arg;
    int servoLegLeft, servoLegRight, servoFootLeft, servoFootRight;

    arg = SCmd.next();
    if (arg != NULL) servoLegLeft = atoi(arg);  // Converts a char string to an integer
    else error = true;

    arg = SCmd.next();
    if (arg != NULL) servoLegRight = atoi(arg);  // Converts a char string to an integer
    else error = true;

    arg = SCmd.next();
    if (arg != NULL) servoFootLeft = atoi(arg);  // Converts a char string to an integer
    else error = true;

    arg = SCmd.next();
    if (arg != NULL) {
        servoFootRight = atoi(arg);  // Converts a char string to an integer
    } else error = true;
    if (error == true) {
#ifdef MOUTH_MATRIX
        Otto.putMouth(xMouth);
#endif
        delay(2000);
#ifdef MOUTH_MATRIX
        Otto.clearMouth();
#endif
    } else { // Update Servo:
        int servoPos[4] = {servoLegLeft, servoLegRight, servoFootLeft, servoFootRight};
        Otto._moveServos(200, servoPos);   // Move 200ms
    }
    sendFinalAck();
#endif
}

/**
 * Function to receive movement commands
 */
void receiveMovement() {
#ifdef SERVOS
    sendAck();
    if (Otto.getRestState() == true) Otto.setRestState(false);

    // Definition of Movement Bluetooth commands
    // M  moveID duration  moveSize
    // M       1     1000        20
    char *arg;

    arg = SCmd.next(); // Move ID
    if (arg != NULL) moveID = atoi(arg);
    else {
#ifdef MOUTH_MATRIX
        Otto.putMouth(xMouth);
#endif
        delay(2000);
#ifdef MOUTH_MATRIX
        Otto.clearMouth();
#endif
        moveID = 0; // stop
    }

    arg = SCmd.next(); // Duration - Speed
    if (arg != NULL) duration = atoi(arg);
    else duration = 1000;

    /*
    arg = SCmd.next(); // Steps
    if (arg != NULL) steps = atoi(arg);
    else steps = 1;
    */

    arg = SCmd.next(); // Move Size
    if (arg != NULL) moveSize = atoi(arg);
    else moveSize = 15;
#endif
}

/**
 * Function to execute the right movement according the movement command received.
 * @param moveID
 */
void move(int moveID) {
#ifdef SERVOS
    bool manualMode = false;
    switch (moveID) {
        case 0:
            Otto.home();
            break;
        case 1: // M 1 1000
            Otto.walk(1, duration, 1);
            break;
        case 2: // M 2 1000
            Otto.walk(1, duration, -1);
            break;
        case 3: // M 3 1000
            Otto.turn(1, duration, 1);
            break;
        case 4: // M 4 1000
            Otto.turn(1, duration, -1);
            break;
        case 5: // M 5 1000 30
            Otto.updown(1, duration, moveSize);
            break;
        case 6: // M 6 1000 30
            Otto.moonwalker(1, duration, moveSize, 1);
            break;
        case 7: // M 7 1000 30
            Otto.moonwalker(1, duration, moveSize, -1);
            break;
        case 8: // M 8 1000 30
            Otto.swing(1, duration, moveSize);
            break;
        case 9: // M 9 1000 30
            Otto.crusaito(1, duration, moveSize, 1);
            break;
        case 10: // M 10 1000 30
            Otto.crusaito(1, duration, moveSize, -1);
            break;
        case 11: // M 11 1000
            Otto.jump(1, duration);
            break;
        case 12: // M 12 1000 30
            Otto.flapping(1, duration, moveSize, 1);
            break;
        case 13: // M 13 1000 30
            Otto.flapping(1, duration, moveSize, -1);
            break;
        case 14: // M 14 1000 20
            Otto.tiptoeSwing(1, duration, moveSize);
            break;
        case 15: // M 15 500
            Otto.bend(1, duration, 1);
            break;
        case 16: // M 16 500
            Otto.bend(1, duration, -1);
            break;
        case 17: // M 17 500
            Otto.shakeLeg(1, duration, 1);
            break;
        case 18: // M 18 500
            Otto.shakeLeg(1, duration, -1);
            break;
        case 19: // M 19 500 20
            Otto.jitter(1, duration, moveSize);
            break;
        case 20: // M 20 500 15
            Otto.ascendingTurn(1, duration, moveSize);
            break;
        default:
            manualMode = true;
            break;
    }
    if (!manualMode) sendFinalAck();
    BTserial.println("M:ACK");
#endif
}

void receiveGesture() {
    // sendAck & stop if necessary
    sendAck();
    Otto.home();

    // Definition of Gesture Bluetooth commands
    // H gestureID
    int gesture = 0;
    char *arg;
    arg = SCmd.next();
    if (arg != NULL) gesture = atoi(arg);
    else delay(2000); // Otto.putMouth(xMouth); // Otto.clearMouth();

#ifdef EYES_MATRIX
    switch (gesture) {
        case 1: //H 1
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +happy_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoHappy);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 2: //H 2
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +happy_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoSuperHappy);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 3: //H 3
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +sad_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoSad);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 4: //H 4
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +sleep_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoSleeping);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 5: //H 5
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +xx_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoFart);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 6: //H 6
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +confused_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoConfused);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 7: //H 7
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +love_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoLove);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 8: //H 8
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +angry_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoAngry);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 9: //H 9
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +freetful_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoFretful);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 10: //H 10
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +magic_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoMagic);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 11: //H 11
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +wave_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoWave);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 12: //H 12
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +magic_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoVictory);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +eyes_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        case 13: //H 13
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +fail_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            Otto.playGesture(OttoFail);
            eyesMatrix.clear();
            eyesMatrix.drawBitmap(0, 0, +XX_bmp, 8, 16, LED_ON);
            eyesMatrix.writeDisplay();
            break;
        default:
            break;
    }
#else
        switch (gesture) {
            case 1: // H 1
                Otto.playGesture(OttoHappy);
                break;
            case 2: // H 2
                Otto.playGesture(OttoSuperHappy);
                break;
            case 3: // H 3
                Otto.playGesture(OttoSad);
                break;
            case 4: // H 4
                Otto.playGesture(OttoSleeping);
                break;
            case 5: // H 5
                Otto.playGesture(OttoFart);
                break;
            case 6: // H 6
                Otto.playGesture(OttoConfused);
                break;
            case 7: // H 7
                Otto.playGesture(OttoLove);
                break;
            case 8: // H 8
                Otto.playGesture(OttoAngry);
                break;
            case 9: // H 9
                Otto.playGesture(OttoFretful);
                break;
            case 10: // H 10
                Otto.playGesture(OttoMagic);
                break;
            case 11: // H 11
                Otto.playGesture(OttoWave);
                break;
            case 12: // H 12
                Otto.playGesture(OttoVictory);
                break;
            case 13: // H 13
                Otto.playGesture(OttoFail);
                break;
            default:
                break;
        }
#endif
    sendFinalAck();
    BTserial.println("H:ACK");
}

/**
 * Function to receive sing commands
 */
void receiveSing() {
#ifdef BUZZER
    // sendAck & stop if necessary
    sendAck();
    Otto.home();
    // Definition of Sing Bluetooth commands
    // K  SingID
    int sing = 0;
    char *arg;
    arg = SCmd.next();
    if (arg != NULL) sing = atoi(arg);
    else delay(2000); // Otto.putMouth(xMouth); // Otto.clearMouth();
    switch (sing) {
        case 1: // K 1
            Otto.sing(S_connection);
            break;
        case 2: // K 2
            Otto.sing(S_disconnection);
            break;
        case 3: // K 3
            Otto.sing(S_surprise);
            break;
        case 4: // K 4
            Otto.sing(S_OhOoh);
            break;
        case 5: // K 5
            Otto.sing(S_OhOoh2);
            break;
        case 6: // K 6
            Otto.sing(S_cuddly);
            break;
        case 7: // K 7
            Otto.sing(S_sleeping);
            break;
        case 8: // K 8
            Otto.sing(S_happy);
            break;
        case 9: // K 9
            Otto.sing(S_superHappy);
            break;
        case 10: // K 10
            Otto.sing(S_happy_short);
            break;
        case 11: // K 11
            Otto.sing(S_sad);
            break;
        case 12: // K 12
            Otto.sing(S_confused);
            break;
        case 13: // K 13
            Otto.sing(S_fart1);
            break;
        case 14: // K 14
            Otto.sing(S_fart2);
            break;
        case 15: // K 15
            Otto.sing(S_fart3);
            break;
        case 16: // K 16
            Otto.sing(S_mode1);
            break;
        case 17: // K 17
            Otto.sing(S_mode2);
            break;
        case 18: // K 18
            Otto.sing(S_mode3);
            break;
        case 19: // K 19
            Otto.sing(S_buttonPushed);
            break;
        default:
            break;
    }
    sendFinalAck();
#endif
}

/**
 * Function to receive RF command
 */
void receiveRelay() {
#ifdef RADIO_FREQUENCY
    int relayNumber = 0;
    int onoff = 0;

    sendAck();
    // F relayNumber
    // F  1
    char *arg;

    arg = SCmd.next(); // Relay Number
    if (arg != NULL) relayNumber = atoi(arg);
    else {
    #ifdef MOUTH_MATRIX
        Otto.putMouth(xMouth);
    #endif
        delay(2000);
    #ifdef MOUTH_MATRIX
        Otto.clearMouth();
    #endif
        moveID = 0; // stop
    }

    arg = SCmd.next(); // ON-OFF
    if (arg != NULL) onoff = atoi(arg);
    else onoff = 0;

    Serial.print("RF: ");
    Serial.print(relayNumber);
    Serial.print(" ");
    Serial.println(onoff);


    //char *msg = "F Hola Otto!";
    char msg[6] = "F 0 0";
    msg[2] = relayNumber + '0';
    msg[4] = onoff + '0';

    #ifdef RF_TRANSMITTER
        int i;
        for (i = 0; i < 5; i++) {
            transmitter.send((byte *)msg, strlen(msg) + 1);
            delay(300);
        }
    #endif

    #ifdef VIRTUAL_WIRE
        vw_send((uint8_t *)msg, 7);
        vw_wait_tx(); // Wait until the whole message is gone
        delay(3000);
    #endif

    #ifdef RADIO_HEAD
        driver.send((uint8_t *)msg, strlen(msg));
        driver.waitPacketSent();
    #endif
    #ifdef RC_SWITCH
        Serial.println("Sending Message...");
        /**
         * Switch a remote switch on/off (Type B with two rotary/sliding switches)
         *
         * @param nAddressCode  Number of the switch group (1..4)
         * @param nChannelCode  Number of the switch itself (1..4)
         */
        onoff = 1;
        if (onoff == 1) {
            rcSwitch.switchOn(1, relayNumber);                    
        } else {
            rcSwitch.switchOff(1, relayNumber);                    
        }
    #endif

    BTserial.println("F:ACK");

#endif
}

/**
 * Function to receive Name command
 */
void receiveName() {
    // sendAck & stop if necessary
    sendAck();
    Otto.home();
    char newOttoName[11] = "";  // Variable to store data read from Serial.
    int eeAddress = 5;          // Location we want the data to be in EEPROM.
    char *arg;
    arg = SCmd.next();
    if (arg != NULL) {
        // Complete newOttoName char string
        int k = 0;
        while ((*arg) && (k < 11)) {
            newOttoName[k] = *arg++;
            k++;
        }
        EEPROM.put(eeAddress, newOttoName);

        pinMode(PIN_BLE_ENABLE, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
        digitalWrite(PIN_BLE_ENABLE, HIGH);
        delay(300);
        BTserial.print("AT+NAME=");
        BTserial.println(newOttoName);
        delay(300); 
        digitalWrite(PIN_BLE_ENABLE, LOW);
        delay(300);
    } else {
        // Otto.putMouth(xMouth);
        delay(2000);
        // Otto.clearMouth();
    }
    sendFinalAck();
}

/**
 * Function to send Otto's name
 */
void requestName() {
    Otto.home(); // stop if necessary
    char actualOttoName[11] = ""; // Variable to store data read from EEPROM.
    int eeAddress = 5;            // EEPROM address to start reading from
    // Get the float data from the EEPROM at position 'eeAddress'
    EEPROM.get(eeAddress, actualOttoName);
    Serial.print(F("&&"));
    Serial.print(F("E "));
    Serial.print(actualOttoName);
    BTserial.print("E:");
    BTserial.println(actualOttoName);
    Serial.println(F("%%"));
    Serial.flush();
}



/**
 * Function to send program ID
 */
void requestProgramId() {
    Otto.home();   // stop if necessary
    Serial.print(F("&&"));
    Serial.print(F("I "));
    Serial.print(programID);
    BTserial.print("I:");
    BTserial.println(programID);
    Serial.println(F("%%"));
    Serial.flush();
}

/**
 * Function to send Ack comand (A)
 */
void sendAck() {
    delay(30);
    Serial.print(F("&&"));
    Serial.print(F("A"));
    Serial.println(F("%%"));
    Serial.flush();
}

/**
 * Function to send final Ack comand (F)
 */
void sendFinalAck() {
    delay(30);
    Serial.print(F("&&"));
    Serial.print(F("F"));
    Serial.println(F("%%"));
    Serial.flush();
}

/**
 * Function executed when  button is pushed / Touch sensor VIA INTERRUPT routine
 */
void ButtonPushed() {
    if (!buttonPushed) {
        buttonPushed = true;
#ifdef MOUTH_MATRIX
        Otto.putMouth(smallSurprise);
#endif
    }
}

/**
 * Function to Read Touch Sensor
 */
void checkIfTouched() {
#ifdef TOUCH_SENSOR
    currentTouchState = digitalRead(PIN_BUTTON);
    if (lastTouchState == LOW && currentTouchState == HIGH) {
    #ifdef EYES_MATRIX
        eyesMatrix.clear(); eyesMatrix.drawBitmap(0, 0, + touch_bmp , 8, 16, LED_ON);  eyesMatrix.writeDisplay();
    #endif
        //Otto.sing(S_buttonPushed);
        delay(500);
    #ifdef EYES_MATRIX
        eyesMatrix.clear(); eyesMatrix.drawBitmap(0, 0, + eyes_bmp , 8, 16, LED_ON);  eyesMatrix.writeDisplay();
    #endif
        BTserial.println(F("W:ACK"));
    }
    lastTouchState = currentTouchState;
#endif
}

/**
 * Function to check BLE connection status
 * @return TRUE if connected
 */
bool checkIfConnected() {
#ifdef BLUETOOTH
    int state = digitalRead(PIN_BLE_STATE);
    long now = millis();
    if (state == HIGH) {
        if (confirmedConnected == false) {
            if (lastConnState == LOW) {
                // start the timer for HIGH
                highStartTimestamp = now;
                lastConnState = HIGH;
            } else {
                if (now - highStartTimestamp >= msToDefineConnection) {
                    Serial.println("C");
                    confirmedConnected = true;
                    // do stuff like control an LED
                    Otto.sing(S_connection);
                    // Empty the buffer
                    while (BTserial.available() > 0) {
                        char t = BTserial.read();
                        Serial.write(t);
                    }
                }
            }
        }
    } else { // state is LOW
        if (lastConnState == HIGH && confirmedConnected == true) {
            Otto.sing(S_disconnection);
            Serial.println("D");
            confirmedConnected = false;
            
            // Set the Otto Name in BLE
            int eeAddress = 5;          // Location we want the data to be in EEPROM.
            char c;
            char ottoName[11];
            for (int8_t i = eeAddress; i < eeAddress + 11; i++) {
                EEPROM.get(i, c);
                ottoName[i - eeAddress] = c;
            }
            delay(1000);
            Serial.println(ottoName);
            BTserial.print("AT+NAME");
            BTserial.println(ottoName);
        }
        lastConnState = LOW;
    }
    return confirmedConnected;
#endif
}

/**
 *
 */
void hex2dec(char *hex, uint8_t *image_bmp) {
#ifdef EYES_MATRIX
    char sub[3];
    uint8_t dec;
    int i = 0;
    int j = 0;
    for (i = 0 ; i < 32; i = i + 4) {
        sub[0] = hex[i];
        sub[1] = hex[i + 1];
        sub[2] = '\0';
        dec = strtoul(sub, NULL, 16);
        image_bmp[j++] = dec;
        printf("Number: %s -  %d\n", sub, dec);
    };
    for (i = 2 ; i < 32; i = i + 4) {
        sub[0] = hex[i];
        sub[1] = hex[i + 1];
        sub[2] = '\0';
        dec = strtoul(sub, NULL, 16);
        image_bmp[j++] = dec;
    };
#endif
}
