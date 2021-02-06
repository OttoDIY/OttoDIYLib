//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Otto_BT_TEST   sample sketch
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// additional functionality by Birger Töpelmann Jan.2021 toepelmann(at)web.de
//
//BLUETOOTH SOFTWARE SERIAL    /////////////////////////////////////////////////////////////////////
#include <SoftwareSerial.h>
// SoftwareSerial (RX, TX)
#define PIN_TX 11 //connect BLUETOOTH RX pin here - pin 3
#define PIN_RX 12 //connect BLUETOOTH TX pin here - pin 2
// set up software serial library with the name BTserial and the pins used for RX and TX
SoftwareSerial BTSerial ( PIN_TX , PIN_RX ); 
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
const char PROGMEM Headline[] = "Predefined BLE AT Commandstrings send by Digit 0..9:";
//
const char PROGMEM ATcmd0[] = "AT+HELP";
const char PROGMEM ATcmd1[] = "AT+VERSION";
const char PROGMEM ATcmd2[] = "AT+ROLE";
const char PROGMEM ATcmd3[] = "AT+TYPE";
const char PROGMEM ATcmd4[] = "AT+PIN";
const char PROGMEM ATcmd5[] = "AT+HELP";
const char PROGMEM ATcmd6[] = "AT+DISC";
const char PROGMEM ATcmd7[] = "AT+DEFAULT";
const char PROGMEM ATcmd8[] = "AT+NAME";
const char PROGMEM ATcmd9[] = "AT+NAMEOttoDIY";
// table of cmd stringadresses in flash memory
const char* const ATcmds[] PROGMEM = {
    ATcmd0, ATcmd1, ATcmd2, ATcmd3, ATcmd4, 
    ATcmd5, ATcmd6, ATcmd7, ATcmd8, ATcmd9 };

///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////

// list the predefined cmd strings (little helper)
void cmdsToMonitor() {

    Serial.println(F("Predefined BLE AT Commandstrings send by Digit 0..9:"));
    for (uint8_t idx = 0; idx <= 9; idx++) {
        Serial.write('\t');
        Serial.write('0' + idx);
        Serial.print(F(" = "));
        // load charpointer with stringadress
        char* chptr = (char*)pgm_read_word(&(ATcmds[idx]));
        // read & print char from string
        while(char chr = pgm_read_byte(chptr++)) {
            Serial.write(chr);
        }
        Serial.println(""); // new line
    }
    Serial.println(F("Enter Digit 0..9 or AT-command"));
} // cmdsToMonitor()

// send a predefined cmd to the BLE device
void cmdsToBLE(char data) {

   // load charpointer with stringadress
   char* chptr = (char*)pgm_read_word(&(ATcmds[data-'0']));
        // read & send char from string
        while(char chr = pgm_read_byte(chptr++)) {
            BTSerial.write(chr);
        }
        BTSerial.println(""); // new line
} // cmdsToBLE()

///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
    BTSerial.begin (9600); // start the software serial communications
    Serial.begin (9600); // start the standard serial communications
    delay(200);
    Serial.println(F("Bluetooth Communication Test"));
    Serial.println(F("Attention: "));
    Serial.println(F("- Arduino monitor has to send the inputstring with NL+CR at the End"));
    Serial.println(F("- All BLE cmds are in CAPITAL / UPPERCASE letters"));
    Serial.println(F("Enter ""AT+HELP"" [Send] to get an overview of available BLE commands"));
    Serial.println(F("or enter an ""h"" [Send] to get an overview of predefined BLE commands"));
//    BTSerial.println("AT+HELP");
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  // Receive data via Bluetooth and the software serial library
//    if  (BTSerial.available ()) {   // if we have received something over Bluetooth then we will display 
//                                    //it back if you use the Arduino IDE serial monitor
//        // get the received data
//        byte  data =  BTSerial.read (); 
//        // Output the received data to the serial port
//        Serial.print("Data received = ");
//        Serial.write(data);  
//        Serial.println("");       
//    }
//
    // if we have received something over Bluetooth then we will display 
    // it back if you use the Arduino IDE serial monitor
    while (BTSerial.available ()) {
        // get the received data
        byte  data =  BTSerial.read (); 
        // Output the received data to the serial port
        Serial.write(data);  
    }
    
    // send input from the Arduino IDE serial monitor to Bluetooth device
    // or show or send prefined cmd strings
    while (Serial.available ()) {
        // remember last cmd char to determine if 
        // it is the first char of a new cmd line
        static char lastdata = 0;
        // get the received data
        byte  data =  Serial.read ();
        if (lastdata < ' ') {  // first char of a new cmd?
            if (data == 'h') {  // 'h' for helper?
                cmdsToMonitor(); // print predefined cmds
                // flush the rest of cmd line into Write Only Memory 
                while (Serial.available ()) { 
                    data =  Serial.read ();
                }
                return;
            }
            if (isdigit(data)) { // '0'..'9' ?
                cmdsToBLE(data); // send cmd to BLE
                while (Serial.available ()) {
                    data =  Serial.read ();
                }
                return;
            }
        } // if (lastdata < ' ')
        lastdata = data;   
        // Output the received data to the serial port
        BTSerial.write(data);  
    }
}  
