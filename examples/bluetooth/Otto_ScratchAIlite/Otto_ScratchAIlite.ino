#define BLUETOOTH      
#define MOUTH_MATRIX     
#define TOUCH_SENSOR     
#define NOISE_SENSOR  
#define ULTRASONIC_SENSOR 
#define BUZZER            
#define SERVOS       

#include <EEPROM.h>
#include <string.h>
#include <SerialCommandAI.h> 
#include <Otto.h>
Otto Otto; 

#define PIN_RX          0
#define PIN_TX          1
#define LeftLeg 2 
#define RightLeg 3
#define LeftFoot 4 
#define RightFoot 5 
#define PIN_ARM_LEFT    6 
#define PIN_ARM_RIGHT   7 
#define Buzzer  13 
#define PIN_US_TRIGGER  8 
#define PIN_US_ECHO     9 
#define PIN_BLE_STATE  10 
#define PIN_BLE_TX     11
#define PIN_BLE_RX     12
#define PIN_BLE_ENABLE 13 
#define PIN_BUTTON     A0 
#define PIN_MOUTH_CLK  A1 
#define PIN_MOUTH_CS   A2 
#define PIN_MOUTH_DIN  A3 
#define LED_DIRECTION   1 
#define PIN_NOISE      A6  

SoftwareSerial BTserial = SoftwareSerial(PIN_BLE_TX, PIN_BLE_RX); 
SerialCommandAI SCmd(BTserial); 

const char programID[] = "OttoScratchAI-v1"; 
const char factory_name = '$'; 
const char first_name = '#';

int duration = 1000;    
int moveID = 0;        
int moveSize = 15;     
int steps = 1;         
bool buttonPushed = false; 
bool obstacleDetected = false;
unsigned long int mouthMatrix;
int msToDefineConnection = 1500; 
long highStartTimestamp;
int lastConnState = LOW;
bool confirmedConnected = false;
int lastTouchState = LOW;
int currentTouchState;

void setup() {
    Serial.begin(9600);
    BTserial.begin(9600);
    pinMode(PIN_BLE_STATE, INPUT);
    Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer);
#ifdef MOUTH_MATRIX
    Otto.initMATRIX(PIN_MOUTH_DIN, PIN_MOUTH_CS, PIN_MOUTH_CLK, LED_DIRECTION);  
    Otto.matrixIntensity(5); 
#endif

#ifdef TOUCH_SENSOR
    pinMode(PIN_BUTTON, INPUT); // - ensure pull-down resistors are used
#endif
  
    SCmd.addCommand("S", receiveStop);      
    SCmd.addCommand("L", receiveMouth);  
    SCmd.addCommand("T", receiveBuzzer);   
    SCmd.addCommand("K", receiveSing);    
    SCmd.addCommand("H", receiveGesture);  
    SCmd.addCommand("M", receiveMovement);  
    SCmd.addCommand("C", receiveTrims);    
    SCmd.addCommand("G", receiveServo);    
    SCmd.addCommand("R", receiveName);     
    SCmd.addCommand("F", receiveRelay); 
    SCmd.addDefaultHandler(receiveStop);
    
    Otto.sing(S_connection);
    Otto.home();
    Otto.sing(S_happy);
    delay(200);

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
}

void loop() {
    checkIfTouched();
    checkIfConnected();
    SCmd.readSerial();    // If Otto is moving yet
    if (Otto.getRestState() == false) {
        move(moveID);
    }
}

void receiveStop() {
    sendAck();
    Otto.home();
    sendFinalAck();
}

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

void receiveServo() {
#ifdef SERVOS
    sendAck();
    moveID = 30;
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
