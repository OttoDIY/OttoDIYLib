#include "MaxMatrix.h"
#define NumMatrix 1       // Cuantas matrices vamos a usar
byte Buf7219[7]; // "width,height,data[5]" single character buffer.

MaxMatrix lc=MaxMatrix(A3,A2,A1, NumMatrix);  // Creamos una instancia de LedControl
#define zero_code                  0b00001100010010010010010010001100
#define one_code                   0b00000100001100000100000100001110
#define two_code                   0b00001100010010000100001000011110
#define three_code                 0b00001100010010000100010010001100
#define smile_code                 0b00000000100001010010001100000000

void setup()
   {
      for (int i=0; i< NumMatrix ; i++)
        {
           lc.init();
           lc.setIntensity(1);    // Poner el brillo a un valor intermedio
         //  lc.clearDisplay(i);      // Y borrar todo
        }
         for (int i=0;i<8;i++){
     lc.setColumn(i,B11111111);}
     delay(100);
    // lc.clear(); 
   }

void loop()
   {
  
    lc.writeFull(smile_code);
    delay(1000);
    lc.writeFull(three_code);
 delay(1000);
    lc.writeFull(two_code);
  delay(1000);
    lc.writeFull(one_code);
  delay(1000);
    lc.writeFull(zero_code);  
  delay(1000);  
 }
