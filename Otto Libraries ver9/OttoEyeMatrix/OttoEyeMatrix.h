/**
 * \par Copyright (C), 2012-2015, MakeBlock
 * \class   MeLEDMatrix
 * @author  MakeBlock
 * @version V1.0.3
 * @date    2016/01/29
 * @brief   Driver for Me LED Matrix module.
 * Mark Yan        2016/01/29     1.0.3            Fix issue when show integer number
 * MODIFIED FOR OTTO USE  J Snow   NOV 2018 - renamed to keep original library seperate
 * Adapted for OTTO version 9 use
 */
 
#ifndef _OttoEyeMATRIX_H_
#define _OttoEyeMATRIX_H_

#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "OTTOeyeMatrixData.h"

#define PointOn   1
#define PointOff  0


#define LED_BUFFER_SIZE   16
//#define STRING_DISPLAY_BUFFER_SIZE 20


//Define Data Command Parameters
#define Mode_Address_Auto_Add_1  0x40     //0100 0000 B
#define Mode_Permanent_Address   0x44     //0100 0100 B


//Define Address Command Parameters
#define ADDRESS(addr)  (0xC0 | addr)


typedef enum
{
  Brightness_0 = 0,
  Brightness_1,
  Brightness_2,
  Brightness_3,
  Brightness_4,
  Brightness_5,
  Brightness_6,
  Brightness_7,
  Brightness_8
}LED_Matrix_Brightness_TypeDef;


class OTTOeyeMatrix
{
public:

  OTTOeyeMatrix();
  
/**
 * Alternate Constructor which can call your own function to map the Buzzer to arduino port,
 * you can set any slot for the buzzer device. 
 * \param[in]
 *   SCK_Pin - The SCk of LED Matrix.
 * \param[in]
 *   DIN_Pin - Put in parameter.
 */
  OTTOeyeMatrix(uint8_t SCK_Pin, uint8_t DIN_Pin);

  void clearScreen();

  void setBrightness(uint8_t Bright);
  
  void setColorIndex(bool Color_Number);
  
/**
 * \par Function
 *    drawBitmap
 * \par Description
 *    Draw a bitmap.
 * \param[in]
 *    x - The x coordinate of bitmap.
 * \param[in]
 *    y - The y coordinate of bitmap.
 * \param[in]
 *    Bitmap_Width - The width of bitmap.
 * \param[in]
 *    Bitmap - A pointer to bitmap.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void drawBitmap(int8_t x, int8_t y, uint8_t Bitmap_Width, uint8_t *Bitmap);
  void drawBitmapEYE(unsigned long int  EYEBitmap);

  void showClock(uint8_t hour, uint8_t minute, bool = PointOn);

/**
 * \par Function
 *    showNum
 * \par Description
 *    Show the number on LED Matrix.
 * \param[in]
 *    value - The float data need show.
 * \param[in]
 *    digits - Number of digits to display.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  //void showNum(float value,uint8_t = 3);

/**
 * \par Function
 *    reset
 * \par Description
 *    Reset the port of LED Matrix.
 * \param[in]
 *    port - The port of LED Matrix.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void reset(uint8_t port);

private:
  uint8_t u8_SCKPin;
  uint8_t u8_DINPin;

  bool b_Color_Index;
  bool b_Draw_Str_Flag;

  uint8_t u8_Display_Buffer[LED_BUFFER_SIZE];

  int16_t i16_Str_Display_X_Position;
  int8_t i8_Str_Display_Y_Position;
  int16_t i16_Number_of_Character_of_Str;
  //char i8_Str_Display_Buffer[STRING_DISPLAY_BUFFER_SIZE];

/**
 * \par Function
 *    writeByte
 * \par Description
 *    Write byte to LED Matrix.
 * \param[in]
 *    data - The data wrote to LED Matrix.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void writeByte(uint8_t data);
  
/**
 * \par Function
 *    writeBytesToAddress
 * \par Description
 *    Write byte to LED Matrix's address.
 * \param[in]
 *    Address - The address you want to write in LED Matrix.
 * \param[in]
 *    P_data - The pointer points to data.
 * \param[in]
 *    count_of_data - The length of data.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void writeBytesToAddress(uint8_t Address, const uint8_t *P_data, uint8_t count_of_data);
  
/**
 * \par Function
 *    showStr
 * \par Description
 *    Show the string in LED Matrix.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void showStr();

};

#endif
