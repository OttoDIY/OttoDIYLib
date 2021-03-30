#include <Otto9.h>
Otto9 Otto;  //This is Otto!
// TEST VERSION
#include <EEPROM.h>

// direction
const int TOPA    = 0;
const int RIGHTA  = 1;
const int BOTTOMA = 2;
const int LEFTA   = 3;
// Snake length
const int MAX_SNAKE_LENGTH = 16;

// Variables
//Adafruit_8x8matrix matrix = Adafruit_8x8matrix();  // Display
int direction = RIGHTA;                               // direction of movement
int snakeX[MAX_SNAKE_LENGTH];                      // X-coordinates of snake
int snakeY[MAX_SNAKE_LENGTH];                      // Y-coordinates of snake
int snakeLength = 3;   
int score = 3;// 
unsigned long prevTime = 0;                        // for gamedelay (ms)
unsigned long delayTime = 500;                     // Game step in ms

int fruitX, fruitY;
unsigned long fruitPrevTime = 0;
unsigned long fruitBlinkTime = 100;
unsigned long bonusTime = 0;
unsigned long bonusPrevTime = 0;
int fruitLed = true;
int bonusLed = true;
int bonusX, bonusY;
// LED MATRIX PINs //////////////////////////////////////////////////////////////////////////
#define DIN_PIN    A3   //DIN pin (A3)
#define CS_PIN     A2   //CS pin (A2)
#define CLK_PIN    A1   //CLK pin (A1)
#define LED_DIRECTION  1// LED MATRIX CONNECTOR position (orientation) 1 = top 2 = bottom 3 = left 4 = right  DEFAULT = 1

void setup(){
Otto.initMATRIX( DIN_PIN, CS_PIN, CLK_PIN, LED_DIRECTION);   // set up Matrix display pins = DIN pin,CS pin, CLK pin, MATRIX orientation 
Otto.matrixIntensity(1);// set up Matrix display intensity
  Serial.begin(9600);
  
  randomSeed(analogRead(6));
  pinMode(7,INPUT);
  //pinMode(17,INPUT);
  pinMode(1,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(A0,INPUT);
  pinMode(5,INPUT);
  //buzzer
  pinMode(13,OUTPUT);
  
  
  snakeX[0] = 0;
  snakeY[0] = 4;
  for(int i=1; i<MAX_SNAKE_LENGTH; i++){
    snakeX[i] = snakeY[i] = -1;
  }
  makeFruit();
  
}
  
void loop(){
    delay(10);
    checkButtons();  // if any button is pressed or not
   
    unsigned long currentTime = millis();
    if(currentTime - prevTime >= delayTime){
    nextstep(); 
    
    prevTime = currentTime;
  }
  draw(); // make snack & food
}

void checkButtons(){
  if(digitalRead(A0)==1)
     direction = TOPA;
  else if(digitalRead(A0)==0)
     direction = RIGHTA;
  else if(digitalRead(5)==0)
     direction = LEFTA;
  else if(digitalRead(2)==0)
     direction = BOTTOMA;
}

void draw(){
  Otto.clearMouth();
  drawSnake();
  drawFruit();

}
void drawSnake(){
  for(int i=0; i<snakeLength; i++){
    Otto.setLed(snakeX[i], snakeY[i], 1);
  }
}
void drawFruit(){
  if(inPlayField(fruitX, fruitY)){
    unsigned long currenttime = millis();
    if(currenttime - fruitPrevTime >= fruitBlinkTime){
      fruitLed = (fruitLed == true) ? false : true;
      fruitPrevTime = currenttime;
    }
    Otto.setLed(fruitX, fruitY, fruitLed);
  }
}
boolean inPlayField(int x, int y){
  return (x>=0) && (x<8) && (y>=0) && (y<8);
}
void nextstep(){
    for(int i=snakeLength-1; i>0; i--){
    if((direction == RIGHTA)&&(snakeX[0]-snakeLength == 7))
    snakeX[0] = -1;
    else if((direction == LEFTA)&&(snakeX[0]+ snakeLength == 0))
    snakeX[0] = 8;
    else snakeX[i] = snakeX[i-1];
    if((direction == TOPA) && (snakeY[0]+snakeLength == 0))
    snakeY[0]=8;
    else if((direction == BOTTOMA) && (snakeY[0]-snakeLength == 7))
    snakeY[0]=-1;
    else snakeY[i] = snakeY[i-1];      
  }
  switch(direction){
    case TOPA:
      snakeY[0] = snakeY[0]-1; 
      break;
    case RIGHTA:
      snakeX[0] = snakeX[0]+1;
      break;
    case BOTTOMA:
      snakeY[0] = snakeY[0]+1;
      break;
    case LEFTA:
      snakeX[0]=snakeX[0]-1;
      break;
   }
  if((snakeX[0] == fruitX) && (snakeY[0] == fruitY)){
    snakeLength++;
    score++;
    tone(13,4500,50);
    if(snakeLength < MAX_SNAKE_LENGTH){      
      makeFruit();
    } 
    else {
      fruitX = fruitY = -1;
    }
    if(score%8==0)
      {
        snakeLength = 3;
        delayTime = delayTime - 100;
     }
  }
  snakeItSelf();  
}
void makeFruit(){
  int x, y;
  x = random(0, 8);
  y = random(0, 8);
  while(isPartOfSnake(x, y)){
    x = random(0, 8);
    y = random(0, 8);
  }
  fruitX = x;
  fruitY = y;
}

boolean isPartOfSnake(int x, int y){
  for(int i=0; i<snakeLength-1; i++){
    if((x == snakeX[i]) && (y == snakeY[i])){
      return true;
    }
  }
  return false;
}
void snakeItSelf(){  // check if snack touch itself
    for(int i=1;i<snakeLength;i++){
      if((snakeX[0] == snakeX[i]) && (snakeY[0] == snakeY[i]))
       gameOver();
     }
}
void gameOver(){
  // game over sound
  tone(13,1000,100);
  delay(100);
  tone(13,1500,200);
  delay(200);
  tone(13,2000,300);
  delay(300);
  tone(13, 494,500);
  delay(500);
  Otto.clearMouth();
  for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            Otto.setLed( r, c, HIGH);
            delay(50);
        }delay(50);
    }
  delay(300);
  score = 3;  
  snakeLength = 3;
  direction = RIGHT;
  snakeX[0]=3;
  snakeY[0]=4;
  delayTime = 500;
  loop();
}

void drawBonus(){
  if(inPlayField(fruitX, fruitY)){
    unsigned long bonusTime = millis();
    if(bonusTime - bonusPrevTime >= 300){
      bonusLed = (bonusLed == true) ? false : true;
      bonusPrevTime = bonusTime;
    }
    Otto.setLed(bonusX, bonusY, bonusLed);
  }
}
void makeBonus(){
  int x, y;
  x = random(0, 8);
  y = random(0, 8);
  while(isPartOfSnake(x, y)){
    x = random(0, 8);
    y = random(0, 8);
  }
  bonusX = x;
  bonusY = y;
}
