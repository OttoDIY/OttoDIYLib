#include <SoftwareSerial.h>
/////// SoftwareSerial (RX, TX)
SoftwareSerial BTSerial ( 2 ,  3 ); 
// buffer to receive data
byte  buffer [ 1024 ];
String myString= "" ; // String to receive
void  setup () {
    BTSerial. begin ( 9600 ); 
    Serial. begin ( 9600 ); 
}
void  loop () {
    // Receive data via Bluetooth
    if  (BTSerial.available ()) { 
        // Save received data
        byte  data =  BTSerial.read (); 
        // Output the received data to the serial monitor
        //Serial.write(data);     
        do_the_cmd (data);      
    }
}
void  do_the_cmd ( byte  cmd)
{
    switch (cmd)
    {
       
        case  '0' :
            Serial.write ( "CMD 0 \ n" );
            break ;
        case  '1' :
            Serial.write ( "CMD 1 \ n" );
            break ;
            
        case  '2' : 
            Serial.write ( "CMD 2 \ n" );
            break ;
            
        case  '3' : 
            Serial.write ( "CMD 3 \ n" );
            break ;
         
        default :
            break ;        
    }  
}
