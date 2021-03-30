//------------------------------------------------------------------
// Otto Bluetooth Testing sketch
//
// With this sketch you can test the communication between the Arduino
// and the Bluetooth modul. There are predefined AT command strings
// that are send when entering a digit 0..9 in the Serial Monitor.
//
// When a connection to the OttoDIY App ist established you can watch
// the received commands when tapping the buttons inside the app.
//------------------------------------------------------------------
//-- Otto DIY invests time and resources providing open source code and hardware,
//-- please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, 
//-- you must contribute all your source code to the community and 
//-- all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. 
//-- See http://www.gnu.org/copyleft/gpl.html
//------------------------------------------------------------------
// additional functionality by Birger Töpelmann Jan.2021
// extended for the Otto Builder Community by Birger T February 2021
//
//BLUETOOTH SOFTWARE SERIAL    ////////////////////////////////////
#include <SoftwareSerial.h>
// SoftwareSerial (RX, TX)
#define PIN_TX 11 //connect BLUETOOTH RX pin here
#define PIN_RX 12 //connect BLUETOOTH TX pin here
// set up software serial library with the name 
// BTserial and the pins used for RX and TX
SoftwareSerial BTSerial ( PIN_TX , PIN_RX ); 
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
// "Predefined BLE AT Commandstrings send by Digit 0..9:";
//
const char PROGMEM ATcmd0[] = "AT+HELP";
const char PROGMEM ATcmd1[] = "AT+VERSION";
const char PROGMEM ATcmd2[] = "AT+TYPE";
const char PROGMEM ATcmd3[] = "AT+PIN";
const char PROGMEM ATcmd4[] = "AT+ROLE0";
const char PROGMEM ATcmd5[] = "AT+ROLE1";
const char PROGMEM ATcmd6[] = "AT+DISC";
const char PROGMEM ATcmd7[] = "AT+DEFAULT";
const char PROGMEM ATcmd8[] = "AT+NAME";
// this renames your Bluetooth Modul, so it's more comfortable to
// find the Otto connection among all other connections you may have 
const char PROGMEM ATcmd9[] = "AT+NAMEOttoDIY"; 
// table of cmd stringadresses in flash memory
const char* const ATcmds[] PROGMEM = {
    ATcmd0, ATcmd1, ATcmd2, ATcmd3, ATcmd4, 
    ATcmd5, ATcmd6, ATcmd7, ATcmd8, ATcmd9 };
    

///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////

void testing () {
// Here you can write your own additional code and call it by 
// sending a 't' from the Serial Monitor.

}
//
void help1() {
    
    Serial.print(F( "Otto 9 Bluetooth Communication Test\n"
                    "Attention: \n"
                    "- Arduino monitor has to send the inputstring with NL+CR at the End\n"
                    "- All Bluetooth commands are in CAPITAL / UPPERCASE letters\n\n"
                    "Enter ""AT+HELP"" [Send] to get an overview of available BLE commands\n"
                    "or enter an ""h"" [Send] to get an overview of predefined BLE commands\n\n"
                    ));
} // help1()
//
void help2() {
    
    Serial.print(F( "\n\nWhen the communication with the Bluetooth modul is working now,\n"
                    "you can connect to the OttoDIY App.\n"
                    "Inside the App select a page and tap the command buttons..\n"
                    "..and watch the received command strings in the Serial Monitor\n\n"));
} // help2()

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
    help1();
    cmdsToMonitor();
    help2();
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
//////////////////////////////////////////////// Birger T 20.Jan.2021
/*  I am wondering what messages are sent from the OttoDIY app to
 *   the arduino. This sketch comes me also in handy to display
 *   these messages in serial monitor - but..
 *  The commands are sent without an new line char at the end, 
 *  so all commands are printed in a single line.
 *  I added some lines, if an incoming BLE string ends without any 
 *  control character as new line or carriage return, then an 
 *  additional println() to the serial monitor is executed.
 * 
 */
#define BTWAITNL 30   // the amount of millis to wait before a println()
 
    // don't mess around with long or uint32_t timing variables
    // when only short delays up to 255ms are needed
    static uint8_t msOld;               // to remember last read millis
    uint8_t msNow = (uint8_t) millis(); // get lower byte of current millis
            
    static char    BTdata = 0x20;       // remember last BT/BLE char

    // if we have received something over Bluetooth then we will display 
    // it back if you use the Arduino IDE serial monitor
    while (BTSerial.available ()) {
        // get the received data from Bluetooth port
        BTdata =  BTSerial.read (); 
        // Output the received data to the serial monitor port
        Serial.write(BTdata);
        // make unprintable characters visible
//        Serial.print((uint8_t)BTdata,HEX);
        msOld = msNow;      // reset time for a '\n' (New Line)
    }

    if (BTdata == 0x0d) {       // if the last BT/BLE char was a '\r' Return..
        if (msNow - msOld > BTWAITNL) {  // .. if it is time for a NewLine..
            Serial.write('\n'); // ..send the NewLin Character to Monitor
            BTdata = 0x20;      // set the last BT/BLE char to an control char
        }
    } // if (BTdata >= ' ')

    // send input from the Arduino IDE serial monitor to Bluetooth device
    // or show or send predefined cmd strings
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
            if (data == 't') {  // 't' for testing?
                testing(); // 
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
        // Output the received data to the Bluetooth serial port
        BTSerial.write(data);  
    } // while (Serial.available ())
}  // loop()
