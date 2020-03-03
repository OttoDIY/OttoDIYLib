/*
   @file Lip_Synching_Robot_MAX7219.ino
   @brief 8 x 8 LED Matrix animation using Arduino.
   @author Kei Takagi
   @date 2016.10.2
*/

#include <MsTimer2.h>
#define FallVelocity 50

int Max7219_pinDIN = A3;
int Max7219_pinCS = A2;
int Max7219_pinCLK = A1;

int anime_frame = 0;
int cat = 0;
int ano = 0;
int val = 0;
int max = 0;
int min = 1024;
int i = 0;

unsigned long time;
byte layer[8];

byte face[15][8] = {
  {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b11111111,
    0b00000000,
    0b00000000,
  }, {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b11100111,
    0b00011000,
    0b00000000,
  }, {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b11111111,
    0b00011000,
    0b00000000,
  }, {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b10000001,
    0b01111110,
    0b00000000,
  }, {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b11000011,
    0b00111100,
    0b00000000,
  }, {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
  }, {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00100100,
    0b00100100,
    0b00100100,
    0b00011000,
  }, {
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b10000001,
    0b10000001,
    0b01111110,
    0b00000000,
  }, {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
  }, {
    0b00000000,
    0b00000000,
    0b01111110,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110,
  }, {
    0b00000000,
    0b00000000,
    0b01111110,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110,
  }, {
    0b00000000,
    0b00000000,
    0b01111110,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110,
  }, {
    0b00000000,
    0b01111110,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110,
  }
  , {
    0b01111110,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110,
    0b00000000,
  }
  , {
    0b01111110,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110,
  }
};

void setup()
{
  // Init MAX7219
  pinMode(Max7219_pinDIN, OUTPUT);
  pinMode(Max7219_pinCS, OUTPUT);
  pinMode(Max7219_pinCLK, OUTPUT);
  delay(50);
  Write_Max7219(0x09, 0x00);
  Write_Max7219(0x0a, 0x03);
  Write_Max7219(0x0b, 0x07);
  Write_Max7219(0x0c, 0x01);
  Write_Max7219(0x0f, 0x00);

  // Timer
  time = millis() +  FallVelocity;
  MsTimer2::set(1, display);
  MsTimer2::start();

  randomSeed(analogRead(A6));
}

void loop()
{
  val = 0.7 * val + 0.3 * analogRead(A6);
  if (max < val)max = val;
  if (min > val)min = val;
  if (time < millis()) {
    time = millis() +  FallVelocity;

    anime_frame = map(max, min, 1023, 0, 15);
    for ( i = 0; i < 8; i++) {
      *(layer + i) = *(face[anime_frame] + i);
    }
    switch (random(0, 50) ) {
      case 1:
        *(layer + 0) = 0b00000000;
        *(layer + 1) = 0b00000000;
        break;
      case 2:
        *(layer + 0) = 0b00000000;
        *(layer + 1) = 0b00000000;
        break;
      case 3:
        *(layer + 0) = 0b00000000;
        break;
      case 4:
        *(layer + 1) = 0b00000000;
        break;
    }
    min = max;
    max = 0;
  }
}

void display()
{
  cat < 7 ? cat++ :  cat = 0;
  Write_Max7219(cat + 1 , layer[cat]);
}

void Write_Max7219(unsigned char address, unsigned char dat)
{
  digitalWrite(Max7219_pinCS, LOW);
  Write_Max7219_byte(address);
  Write_Max7219_byte(dat);
  digitalWrite(Max7219_pinCS, HIGH);
}

void Write_Max7219_byte(unsigned char DATA)
{
  digitalWrite(Max7219_pinCS, LOW);
  for (unsigned char j = 0; j < 8; j++)
  {
    digitalWrite(Max7219_pinCLK, LOW);
    digitalWrite(Max7219_pinDIN, (DATA << j) & 0x80);
    digitalWrite(Max7219_pinCLK, HIGH);
  }
}
