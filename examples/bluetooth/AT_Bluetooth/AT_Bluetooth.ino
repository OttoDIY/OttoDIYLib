//  Basic serial communication sketch using AltSoftSerial (ASS). 
//  Uses hardware serial to talk to the host computer and ASS for communication with the Bluetooth module
//
//  When a command is entered in the serial monitor on the computer 
//  the Arduino will relay it to the Bluetooth module and display the result in the serial monitor.
//
//  Pins
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  Arduino D8 ASS RX - BT TX no need voltage divider 
//  Arduino D9 ASS TX - BT RX through a voltage divider
//
 
#include <AltSoftSerial.h>
AltSoftSerial BTSerial; 
 
char c=' ';
boolean NL = true;
 
void setup() 
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");

    BTSerial.begin(9600);  
    //BTSerial.begin(19200); 
    //BTSerial.begin(38400); 
    //BTSerial.begin(57600); 
    //BTSerial.begin(115200);
        Serial.println("BTserial started at 9600"); 
    // If using an HC-05 in AT command mode the baud rate is likely to be 38400
    // Comment out the above 2 lines and uncomment the following 2 lines. 
    // Serial.println("BTserial started at 38400");

    Serial.println("");
 
}
 
void loop()
{
 
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (BTSerial.available())
    {
        c = BTSerial.read();
        Serial.write(c);
    }
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();
        BTSerial.write(c);   
 
        // Echo the user input to the main window. The ">" character indicates the user entered text.
        if (NL) { Serial.print(">");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }
}
