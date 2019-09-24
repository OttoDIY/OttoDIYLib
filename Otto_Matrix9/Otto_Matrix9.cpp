/*
 * MaxMatrix
 * Version 1.0 Feb 2013
 * Copyright 2013 Oscar Kin-Chung Au
 * Adapted for OTTO version 9 use
 */


#include "Arduino.h"
#include "Otto_Matrix9.h"

Otto_Matrix::Otto_Matrix() 
{
    //data = _data;
    //load = _load;
    //clock = _clock;
    //num = _num;
    //for (int i=0; i<80; i++)
    //buffer[i] = 0;
}

void Otto_Matrix::init(byte _data, byte _load, byte _clock, byte _num, int _rotation)
{
    data = _data;
    load = _load;
    clock = _clock;
    num = _num;
    rotation = _rotation;
    if ((rotation > 4) || (rotation == 0)) rotation = 1; // we have to have number between 1 and 4
    // itoa(_rotation, rotation2,1);
    for (int i=0; i<80; i++)
	buffer[i] = 0;
    
    pinMode(load,  OUTPUT);

#if defined(ESP32)
    SPI.begin ( clock,  -1,  data, load);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    SPI.setHwCs(true);
#else
    pinMode(data,  OUTPUT);
    pinMode(clock, OUTPUT);
    digitalWrite(clock, HIGH); 
#endif

    setCommand(max7219_reg_scanLimit, 0x07);      
    setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    setCommand(max7219_reg_displayTest, 0x00); // no display test
	
    // empty registers, turn all LEDs off
    clearMatrix();
	
    setIntensity(0x0f);    // the first 0x0f is the value you can set
}

void Otto_Matrix::setIntensity(byte intensity)
{
    setCommand(max7219_reg_intensity, intensity);
}

void Otto_Matrix::clearMatrix()
{
    for (int i=0; i<8; i++) 
	setColumnAll(i,0);
		
    for (int i=0; i<80; i++)
	buffer[i] = 0;
}

void Otto_Matrix::setCommand(byte command, byte value)
{
#if defined(ESP32)
    SPI.transfer16(command << 8 | value);
#else
    digitalWrite(load, LOW);    
    for (int i=0; i<num; i++) 
    {
	shiftOut(data, clock, MSBFIRST, command);
	shiftOut(data, clock, MSBFIRST, value);
    }
    digitalWrite(load, LOW);
    digitalWrite(load, HIGH);
#endif
}


void Otto_Matrix::setColumn(byte col, byte value)
{
    int n = col / 8;
    int c = col % 8;
#if defined(ESP32)
    for (int i=0; i<num; i++)
    {
	if (i == n)
        {
	    SPI.transfer16((c + 1) << 8 | value);
	}
	else
	{
	    SPI.transfer16(0);
	}
    }
#else	
    digitalWrite(load, LOW);    
    for (int i=0; i<num; i++) 
    {
	if (i == n)
	{
	    shiftOut(data, clock, MSBFIRST, c + 1);
	    shiftOut(data, clock, MSBFIRST, value);
	}
	else
	{
	    shiftOut(data, clock, MSBFIRST, 0);
	    shiftOut(data, clock, MSBFIRST, 0);
	}
    }
    digitalWrite(load, LOW);
    digitalWrite(load, HIGH);
#endif
    buffer[col] = value;
}

void Otto_Matrix::setColumnAll(byte col, byte value)
{
#if defined(ESP32)
    for (int i=0; i<num; i++) 
    {
	SPI.transfer16((col + 1) << 8 | value);
	buffer[col * i] = value;
    }
#else
    digitalWrite(load, LOW);    
    for (int i=0; i<num; i++) 
    {
	shiftOut(data, clock, MSBFIRST, col + 1);
	shiftOut(data, clock, MSBFIRST, value);
	buffer[col * i] = value;
    }
    digitalWrite(load, LOW);
    digitalWrite(load, HIGH);
#endif
}

void Otto_Matrix::setDot(byte col, byte row, byte value)
{
    bitWrite(buffer[col], row, value);

    int n = col / 8;
    int c = col % 8;
#if defined(ESP32)
    for (int i=0; i<num; i++) 
    {
	if (i == n)
	{
	    SPI.transfer16((c + 1) << 8 | buffer[col]);
	}
	else
	{
	    SPI.transfer16(0);
	}
    }
#else
    digitalWrite(load, LOW);    
    for (int i=0; i<num; i++) 
    {
	if (i == n)
	{
	    shiftOut(data, clock, MSBFIRST, c + 1);
	    shiftOut(data, clock, MSBFIRST, buffer[col]);
	}
	else
	{
	    shiftOut(data, clock, MSBFIRST, 0);
	    shiftOut(data, clock, MSBFIRST, 0);
	}
    }
    digitalWrite(load, LOW);
    digitalWrite(load, HIGH);
#endif
}

/* void MaxMatrix::writeSprite(int x, int y, const byte* sprite)
   {
   int w = sprite[0];
   int h = sprite[1];
	
   if (h == 8 && y == 0)
   for (int i=0; i<w; i++)
   {
   int c = x + i;
   if (c>=0 && c<80)
   setColumn(c, sprite[i+2]);
   }
   else
   for (int i=0; i<w; i++)
   for (int j=0; j<h; j++)
   {
   int c = x + i;
   int r = y + j;
   if (c>=0 && c<80 && r>=0 && r<8)
   setDot(c, r, bitRead(sprite[i+2], j));
   }
   }

   void MaxMatrix::reload()
   {
   for (int i=0; i<8; i++)
   {
   int col = i;
   digitalWrite(load, LOW);    
   for (int j=0; j<num; j++) 
   {
   shiftOut(data, clock, MSBFIRST, i + 1);
   shiftOut(data, clock, MSBFIRST, buffer[col]);
   col += 8;
   }
   digitalWrite(load, LOW);
   digitalWrite(load, HIGH);
   }
   }

   void MaxMatrix::shiftLeft(bool rotate, bool fill_zero)
   {
   byte old = buffer[0];
   int i;
   for (i=0; i<80; i++)
   buffer[i] = buffer[i+1];
   if (rotate) buffer[num*8-1] = old;
   else if (fill_zero) buffer[num*8-1] = 0;
	
   reload();
   }

   void MaxMatrix::shiftRight(bool rotate, bool fill_zero)
   {
   int last = num*8-1;
   byte old = buffer[last];
   int i;
   for (i=79; i>0; i--)
   buffer[i] = buffer[i-1];
   if (rotate) buffer[0] = old;
   else if (fill_zero) buffer[0] = 0;
	
   reload();
   }

   void MaxMatrix::shiftUp(bool rotate)
   {
   for (int i=0; i<num*8; i++)
   {
   bool b = buffer[i] & 1;
   buffer[i] >>= 1;
   if (rotate) bitWrite(buffer[i], 7, b);
   }
   reload();
   }

   void MaxMatrix::shiftDown(bool rotate)
   {
   for (int i=0; i<num*8; i++)
   {
   bool b = buffer[i] & 128;
   buffer[i] <<= 1;
   if (rotate) bitWrite(buffer[i], 0, b);
   }
   reload();
   }
*/
// rutina para Zowi, para meter sus caritas en la matriz de 8
void Otto_Matrix::writeFull(unsigned long value) {
    if (rotation == 1) {
	for (int r=0; r<5;r++){
            for (int c=0; c<6; c++){
                setDot(6-c,7-r,(1L & (value >> r*6+c)));
	    }
	}
    }
    if (rotation == 2) {
	for (int r=0; r<5;r++){
            for (int c=0; c<6; c++){
                //setDot(6-c,7-r,(1L & (value >> r*6+c)));
                setDot(1+c,r,    (1L & (value >> r*6+c)));
	    }
	}
    }
    if (rotation == 3) {
	for (int r=0; r<5;r++){
            for (int c=0; c<6; c++){
                //setDot(6-c,7-r,(1L & (value >> r*6+c)));
                setDot(r,6-c,    (1L & (value >> r*6+c)));
	    }
	}
    }
    if (rotation == 4) {
	for (int r=0; r<5;r++){
            for (int c=0; c<6; c++){
                //setDot(6-c,7-r,(1L & (value >> r*6+c)));
                setDot(7-r,1+c, (1L & (value >> r*6+c)));  
	    }
	}
    }
}
