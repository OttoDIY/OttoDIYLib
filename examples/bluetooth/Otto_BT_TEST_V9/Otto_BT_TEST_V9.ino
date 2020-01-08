//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Otto_BT_TEST   sample sketch
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY PLUS APP Firmware version 9 (V9)
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//BLUETOOTH SOFTWARE SERIAL    /////////////////////////////////////////////////////////////////////
#include <SoftwareSerial.h>
// SoftwareSerial (RX, TX)
#define PIN_TX 3//connect BLUETOOTH RX pin here - pin 3
#define PIN_RX 2 //connect BLUETOOTH TX pin here - pin 2
SoftwareSerial BTSerial ( PIN_TX , PIN_RX ); // set up software serial library with the name BTserial and the pins used for RX and TX
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
    BTSerial.begin (9600); // start the software serial  communications
    Serial.begin (9600); // start the standard serial communications

}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  // Receive data via Bluetooth and the software serial library
    if  (BTSerial.available ()) {   // if we have received something over Bluetooth then we will display 
                                    //it back if you use the Arduino IDE serial monitor
        // get the received data
        byte  data =  BTSerial.read (); 
        // Output the received data to the serial port
        Serial.print("Data recieved = ");
        Serial.write(data);  
        Serial.println("");       
    }
}  



