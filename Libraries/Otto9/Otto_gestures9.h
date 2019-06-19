//----------------------------------------------------------------
//-- OTTO version 9 
//-----------------------------------------------------------------
#ifndef Otto_gestures9_h
#define Otto_gestures9_h

//***********************************************************************************
//*********************************GESTURE DEFINES***********************************
//***********************************************************************************           

#define OttoHappy 		0
#define OttoSuperHappy 	1
#define OttoSad 		2
#define OttoSleeping 	3
#define OttoFart 		4
#define OttoConfused 	5
#define OttoLove 		6
#define OttoAngry 		7
#define OttoFretful 	8
#define OttoMagic 		9
#define OttoWave 		10
#define OttoVictory 	11
#define OttoFail 		12

//*** MOUTH ANIMATIONS***
#define littleUuh		0
#define dreamMouth		1 	
#define adivinawi		2
#define wave 			3


 //*** MOUTH ANIMATIONS***
#define littleUuh   0
#define dreamMouth    1   
#define adivinawi   2
#define wave      3
#define otto      4 // Wordt niet gebruikt.

 

const unsigned long int Gesturetable[4][10] PROGMEM = {
  {
    0b00000000000000001100001100000000, // littleUuh_code1
    0b00000000000000000110000110000000, // littleUuh_code2
    0b00000000000000000011000011000000, // littleUuh_code3
    0b00000000000000000110000110000000, // littleUuh_code4
    0b00000000000000001100001100000000, // littleUuh_code5
    0b00000000000000011000011000000000, // littleUuh_code6
    0b00000000000000110000110000000000, // littleUuh_code7
    0b00000000000000011000011000000000
  } // littleUuh_code8
  ,
  {
    0b00000000000000000000110000110000, // dreamMouth_code1
    0b00000000000000010000101000010000, // dreamMouth_code2
    0b00000000011000100100100100011000, // dreamMouth_code3
    0b00000000000000010000101000010000  // dreamMouth_code4
  } 
  ,
  {
    0b00100001000000000000000000100001, // adivinawi_code1
    0b00010010100001000000100001010010, // adivinawi_code2
    0b00001100010010100001010010001100, // adivinawi_code3
    0b00000000001100010010001100000000, // adivinawi_code4
    0b00000000000000001100000000000000, // adivinawi_code5
    0b00000000000000000000000000000000  // adivinawi_code6
  } 
  ,
  {
    0b00001100010010100001000000000000, // wave_code1
    0b00000110001001010000100000000000, // wave_code2
    0b00000011000100001000010000100000, // wave_code3
    0b00000001000010000100001000110000, // wave_code4
    0b00000000000001000010100100011000, // wave_code5
    0b00000000000000100001010010001100, // wave_code6
    0b00000000100000010000001001000110, // wave_code7
    0b00100000010000001000000100000011, // wave_code8
    0b00110000001000000100000010000001, // wave_code9
    0b00011000100100000010000001000000  // wave_code10
  } 
};

//"PROGMEM_readAnything.h"   FROM http://www.gammon.com.au/progmem
// modified for OTTO use by Paul Van De Veen along with all PROGMEM mouths and gestures
#include <Arduino.h>  // for type definitions

template <typename T> void PROGMEM_readAnything (const T * sce, T& dest)
  {
  memcpy_P (&dest, sce, sizeof (T));
  }

template <typename T> T PROGMEM_getAnything (const T * sce)
  {
  static T temp;
  memcpy_P (&temp, sce, sizeof (T));
  return temp;
  }

#endif
