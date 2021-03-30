//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Otto_Matrix_smile   sample sketch
// Otto libraries version 9 contain the basic MAXMATRIX 7219 library, so we will use these for this example
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY PLUS APP Firmware version 9 (V9)
//-- Otto DIY invests time and resources providing open source code and hardware,  please support by purchasing kits from (https://www.ottodiy.com)
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL Version 2 when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include <Otto9.h> //-- Otto Library version 9
Otto9 Otto;  //This is Otto!
//---------------------------------------------------------
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////
unsigned long int matrix; // variable to store MATRIX display before we send it to the MATRIX
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup() {
  Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up library Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
  Otto.matrixIntensity(1);// set up Matrix display intensity

Serial.begin (9600);
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////  delay(1000);// wait for 1 second

void loop() {
  // we use the middle section of the display, a 6 x 5 matrix, this means you can only use part of the display
  // to set an LED ON we write a 1 to is postion in a variable of the 30 (6 x 5 = 30) bits 
  // to set an LED OFF we write a 0 to is postion in a variable of the 30 (6 x 5 = 30) bits 
  // the variable starts from the RHS 
  // so to set the bottom RHS LED on we need 0b000000000000000000000000000001
  Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
  matrix = 0b000000000000000000000000000001;// show the first LED bottom RHS 
  Otto.putMouth(matrix, false);  
    delay(1000);
   Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
   matrix = 0b001100001100111111011110001100;
  Otto.putMouth(matrix, false);      
  delay(1000);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second
  Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
  matrix = 0b001100010010010010010010001100; // show ZERO digit
  Otto.putMouth(matrix, false);  
  delay(1000);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second
  Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
  matrix = 0b000100001100000100000100001110; // show ONE digit
  Otto.putMouth(matrix, false); 
  delay(1000);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second
  Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
  matrix = 0b001100010010000100001000011110; // show TWO digit
  Otto.putMouth(matrix, false); 
  delay(1000);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second
  Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
  matrix = 0b001100010010000100010010001100; // show THREE digit
  Otto.putMouth(matrix, false); 
  delay(1000);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second
  Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
  matrix = 0b000000100001010010001100000000; // show SMILE 
  Otto.putMouth(matrix, false); 
  delay(1000);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second
  Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
  // start again at the RHS 
  matrix = 0b000000000000000000000000000001;// set the variable to the first LED bottom RHS again
  for (int i = 0; i < 30; i++) { // this FOR NEXT LOOP repeats the code following it 30 times
      Otto.putMouth(matrix, false); // display the single LED
      matrix = matrix << 1 ;// shift the single LED one to the left (to the next LED)
      delay(250);// wait for 1/4 second, this is so that you can see the image on the Matrix 
      Otto.clearMouth();// clear the Matrix display so that it is ready for the next image
    }
  delay(1000);// wait for 1 second, this is so that you can see the image on the Matrix for 1 second before repeating

}  
