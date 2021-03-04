/*************************************************************************************
//-- Otto FuncNames needed by the Test Serial sketch
//
// Test Serial sketch:
//-- With this code you can test the beeper, the eyes and/or the mouth matrices
//-- you can also test and calibrate the ultrasonic range and the microfon addon.
//
// -- To Serial.print() the names use the "printName(pgm_read_word(&xxxnames[index]));"
// -- statement in your sketch, where "xxxnames" is the array with the stringadresses
// -- and "index" the index/number of the selected matrix's display or sound.

//
//-- Written for the OttoDIY Builders Community
//-- February 2021: (Birger T)
/*************************************************************************************

 Make sure to have installed all libraries: https://wikifactory.com/+OttoDIY/otto-diy
 Otto DIY invests time and resources providing open source code and hardware,
 please support by purchasing kits from (https://www.ottodiy.com)

 BSD license, all text above must be included in any redistribution

 ************************************************************************************/
#ifndef Otto_FuncNames_h
#define Otto_FuncNames_h//
// A list of the predefined sounds inside the Otto Library:
// the sounds are 'hard coded' inside 'Otto(Humanoid)9.cpp'
//
const char singname00[] PROGMEM = {"S_connection"}; //    0
const char singname01[] PROGMEM = {"S_disconnection"}; // 1
const char singname02[] PROGMEM = {"S_buttonPushed"}; //  2
const char singname03[] PROGMEM = {"S_mode1"}; //         3
const char singname04[] PROGMEM = {"S_mode2"}; //         4
const char singname05[] PROGMEM = {"S_mode3"}; //         5
const char singname06[] PROGMEM = {"S_surprise"}; //      6
const char singname07[] PROGMEM = {"S_OhOoh"}; //         7
const char singname08[] PROGMEM = {"S_OhOoh2"}; //        8
const char singname09[] PROGMEM = {"S_cuddly"}; //        9
const char singname10[] PROGMEM = {"S_sleeping"}; //     10
const char singname11[] PROGMEM = {"S_happy"}; //        11
const char singname12[] PROGMEM = {"S_superHappy"}; //   12
const char singname13[] PROGMEM = {"S_happy_short"}; //  13
const char singname14[] PROGMEM = {"S_sad"}; //          14
const char singname15[] PROGMEM = {"S_confused"}; //     15
const char singname16[] PROGMEM = {"S_fart1"}; //        16
const char singname17[] PROGMEM = {"S_fart2"}; //        17
const char singname18[] PROGMEM = {"S_fart3"}; //        18
// this array grants easy access to the strings above by an index
const char* const singnames[] PROGMEM = {
  singname00, singname01, singname02, singname03, singname04,
  singname05, singname06, singname07, singname08, singname09,
  singname10, singname11, singname12, singname13, singname14,
  singname15, singname16, singname17, singname18,
};
//
#define NUMBER_OF_SOUNDNAMES (uint8_t)sizeof(singnames)/sizeof(singnames[0])
//
// A list of the predefined mouths inside the Otto Library:
// the glyphs itself are of 6x5 size in Otto_mouth9.h
//
const char mouthname00[] PROGMEM = {"zero"};
const char mouthname01[] PROGMEM = {"one"};
const char mouthname02[] PROGMEM = {"two"};
const char mouthname03[] PROGMEM = {"three"};
const char mouthname04[] PROGMEM = {"four"};
const char mouthname05[] PROGMEM = {"five"};
const char mouthname06[] PROGMEM = {"six"};
const char mouthname07[] PROGMEM = {"seven"};
const char mouthname08[] PROGMEM = {"eight"};
const char mouthname09[] PROGMEM = {"nine"};
const char mouthname10[] PROGMEM = {"smile"};
const char mouthname11[] PROGMEM = {"happyOpen"};
const char mouthname12[] PROGMEM = {"happyClosed"};
const char mouthname13[] PROGMEM = {"heart"};
const char mouthname14[] PROGMEM = {"bigSurprise"};
const char mouthname15[] PROGMEM = {"smallSurprise"};
const char mouthname16[] PROGMEM = {"tongueOut"};
const char mouthname17[] PROGMEM = {"vamp1"};
const char mouthname18[] PROGMEM = {"vamp2"};
const char mouthname19[] PROGMEM = {"lineMouth"};
const char mouthname20[] PROGMEM = {"confused"};
const char mouthname21[] PROGMEM = {"diagonal"};
const char mouthname22[] PROGMEM = {"sad"};
const char mouthname23[] PROGMEM = {"sadOpen"};
const char mouthname24[] PROGMEM = {"sadClosed"};
const char mouthname25[] PROGMEM = {"okMouth"};
const char mouthname26[] PROGMEM = {"xMouth"};
const char mouthname27[] PROGMEM = {"interrogation"};
const char mouthname28[] PROGMEM = {"thunder"};
const char mouthname29[] PROGMEM = {"culito"};
const char mouthname30[] PROGMEM = {"angry"};

const char* const mouthnames[] PROGMEM = {
  mouthname00, mouthname01, mouthname02, mouthname03, mouthname04,
  mouthname05, mouthname06, mouthname07, mouthname08, mouthname09,
  mouthname10, mouthname11, mouthname12, mouthname13, mouthname14,
  mouthname15, mouthname16, mouthname17, mouthname18, mouthname19,
  mouthname20, mouthname21, mouthname22, mouthname23, mouthname24,
  mouthname25, mouthname26, mouthname27, mouthname28, mouthname29,
  mouthname30
};
//
#define NUMBER_OF_MOUTHNAMES (uint8_t)sizeof(mouthnames)/sizeof(mouthnames[0])
//
// A list of the predefined eyes from some example sketches inside
// the Otto Library: the Bitmaps 16x8 should be in an 'Otto_Eyes9.h'
//
const char eyesname00[] PROGMEM = {"E_logo"};
const char eyesname01[] PROGMEM = {"E_happy"};
const char eyesname02[] PROGMEM = {"E_eyes"};
const char eyesname03[] PROGMEM = {"E_sad"};
const char eyesname04[] PROGMEM = {"E_xx"};
const char eyesname05[] PROGMEM = {"E_XX"};
const char eyesname06[] PROGMEM = {"E_angry"};
const char eyesname07[] PROGMEM = {"E_angry2"};
const char eyesname08[] PROGMEM = {"E_sleep"};
const char eyesname09[] PROGMEM = {"E_freetful"};
const char eyesname10[] PROGMEM = {"E_love"};
const char eyesname11[] PROGMEM = {"E_confused"};
const char eyesname12[] PROGMEM = {"E_wave"};
const char eyesname13[] PROGMEM = {"E_magic"};
const char eyesname14[] PROGMEM = {"E_fail"};

const char* const eyesnames[] PROGMEM = {
  eyesname00, eyesname01, eyesname02, eyesname03, eyesname04,
  eyesname05, eyesname06, eyesname07, eyesname08, eyesname09,
  eyesname10, eyesname11, eyesname12, eyesname13, eyesname14,
};

#define NUMBER_OF_EYENAMES  (uint8_t)sizeof(eyesnames)/sizeof(eyesnames[0])

// --------------------------------------------------------- print_PSTR()
void print_PSTR(char* stradr) {
  char chr;
  while (chr = pgm_read_byte(stradr++)) {
    Serial.write(chr);
  }
  Serial.write('\n');
} // printName()

#endif // Otto_FuncNames_h
