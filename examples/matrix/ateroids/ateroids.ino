/********************************************************
 * This is a simple game using the MAX72XX LED-Matrix.
 * You are a star ship and you have to avoid contact with 
 * the asteroids falling down.
 * 
 *  (c) 2018 by Joshua Weßner 
 *******************************************************/
//LedControl by Eberhard Fahle
// see: http://wayoda.github.io/LedControl/
#include "LedControl.h"
/*
 Now we need a LedControl to work with.
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1); //DIN, CLK, LOAD 
/**************************************************************
 * Global Variables
 */
/* delay between updates of the display */
unsigned long delaytime=1000;

/* to Display the score */
String scoreString;
int score;
int hundreds;
String scoreStr;
String scoreArr[] = {"" ,"" ,"" };

/* micro switches for control */
int pinLeft = 7;
int pinRight = 3;
volatile unsigned long buttonPressed;
int buttonDelay = 150;  //handle contact bounce

volatile bool gameOver = false;

/* "rythm" of the game, im milliseconds */
int tick;
int tickCounter = 1;

/* timestamp */
unsigned long now;

/* display */
int ship; /* Initial ship position */
int columns[] = {0,0,0,0,0,0,0,0}; // 8 columns, every int is row 1-8.
int randomInt;

/**************************************************************
 * setup
 */

void setup() {
  /* Initializing */
  gameOver = false;
  hundreds = 0;
  scoreArr[0] = "";
  scoreArr[1] = "";
  scoreArr[2] = "";
  score = 0;
  tick = 300;
  tickCounter = 1;
  ship = 3;
  now = millis();
  buttonPressed = millis();
  randomSeed(analogRead(15)); // better random numbers

  for(int i = 0; i<8; i++)
    columns[i] = 0;
  
  // Debugging
  // Serial.begin(9600);
  // Serial.print(columns[0]);
  
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,1);
  /* and clear the display */
  lc.clearDisplay(0);

  pinMode(pinLeft, INPUT_PULLUP);
  pinMode(pinRight, INPUT_PULLUP);

  /* attach button press to interrupts */

  
}

/**************************************************************
 * handle left button press
 */
void left()
{
  if(millis() - buttonPressed > buttonDelay) // handle switch contact bounce
  {
        if(ship != 0)
        ship--;
      else
        ship = 7;
    
      lc.clearDisplay(0);
      buttonPressed = millis();
  }

  // after game over, button press will restart game
  if(gameOver == true){
  gameOver = false;
  setup();
  }
}

/**************************************************************
 *  handle right button press
 */
void right()
{
    if(millis() - buttonPressed > buttonDelay) // handle switch contact bounce
    {
      if(ship != 7)
        ship++;
      else
        ship = 0;
      
      lc.clearDisplay(0);
       buttonPressed = millis();
    }

  // after game over, button press will restart game
  if(gameOver == true){
  gameOver = false;
  setup();
  }   
}


/**************************************************************
 *  loop
 */
void loop() { 

  if(millis() - now > tick){  //do every tick

    // score is: how many ticks you survived
    score++;
  
    now = millis();

    if(tickCounter == 1){ //every 4th tick

       // make game faster over time
       tick = tick/1.02;

      // randomly choose column
      randomInt = random(0, 8);

      // if no asteroid exists in column, create one in row 1.
      if(columns[randomInt] == 0){  
        columns[randomInt] = 1;
      }
    }

    if(tickCounter != 4)
      tickCounter++;
     else
      tickCounter = 1;

    // do for every column
    for(int i = 0; i<8; i++){
      
      if(columns[i] == 10) // delete asteroids when out of display
        columns[i] = 0;
        
      if(columns[i] != 0) // make asteroids fall down
        columns[i]++;
    }

    lc.clearDisplay(0);
  }


  /* write to display */

  // ship
  lc.setLed(0, 7, ship, true);

  // asteroids
  for(int i = 0; i<8; i++){
    if(columns[i] > 0)
      lc.setLed(0, columns[i]-2, i, true);
      lc.setLed(0, columns[i]-3, i, true);
  }

  // detect collision of ship with asteroid
  if(columns[ship] == 10 or columns[ship] == 9){
    lc.clearDisplay(0);

    // animate explosion
    for(int i = 0; i<4; i++){
      lc.setLed(0,7,ship+i,true);
      lc.setLed(0,7,ship-i,true);
      lc.setLed(0,7-i,ship+i,true);
      lc.setLed(0,7-i,ship-i,true);
      lc.setLed(0,7-1.5*i,ship,true);

      // explosion sound
      unsigned long time = millis();
      int randomSound=1000;
      while(millis() - time <= 250)  {  
        randomSound--;
        tone(13, random(randomSound, 1000));   // change the parameters of random() for different sound
      }

      lc.clearDisplay(0);
      noTone(13);
    }

    delay(500);

    /* display score */
   
    scoreStr = String(score);

    // get single digits from score int
    scoreArr[0] = scoreStr.charAt(0);
    scoreArr[1] = scoreStr.charAt(1);
    scoreArr[2] = scoreStr.charAt(2);

    // matrix can only display up to 2 digits (up to 99)
    if(score < 100){
        for(int i = 0; i<2; i++){
          if(scoreArr[i] == "0")
            draw0(1+i*4);
          if(scoreArr[i] == "1")
            draw1(1+i*4);
          if(scoreArr[i] == "2")
            draw2(1+i*4);
          if(scoreArr[i] == "3")
            draw3(1+i*4);
          if(scoreArr[i] == "4")
            draw4(1+i*4);
          if(scoreArr[i] == "5")
            draw5(1+i*4);
          if(scoreArr[i] == "6")
            draw6(1+i*4);
          if(scoreArr[i] == "7")
            draw7(1+i*4);
          if(scoreArr[i] == "8")
            draw8(1+i*4);
          if(scoreArr[i] == "9")
            draw9(1+i*4);
        }
          
        }
        // if score is > 100, display only last two digits of score
    else{

        for(int i = 1; i<3; i++){
          if(scoreArr[i] == "0")
            draw0(1+(i-1)*4);
          if(scoreArr[i] == "1")
            draw1(1+(i-1)*4);
          if(scoreArr[i] == "2")
            draw2(1+(i-1)*4);
          if(scoreArr[i] == "3")
            draw3(1+(i-1)*4);
          if(scoreArr[i] == "4")
            draw4(1+(i-1)*4);
          if(scoreArr[i] == "5")
            draw5(1+(i-1)*4);
          if(scoreArr[i] == "6")
            draw6(1+(i-1)*4);
          if(scoreArr[i] == "7")
            draw7(1+(i-1)*4);
          if(scoreArr[i] == "8")
            draw8(1+(i-1)*4);
          if(scoreArr[i] == "9")
            draw9(1+(i-1)*4);
        }
        // first digit of score ("hundreds") is displayed as dots: One dot for every hundred.
        for(int i = 1; i<10; i++){
          if(scoreArr[0] == String(i))
            hundreds = i;
        }
        for(int i = 1; i <= hundreds; i++){
          lc.setLed(0,0,i-1,true);
          lc.setLed(0,1,i-1,true);
          delay(200);          
        }
        
    }
   gameOver = true;
    while(gameOver == true){
          // do nothing but wait for button press
      }
  }
    
}

///////////////////////////// Draw score digits /////////////////////////////////////////
void draw1(int position){

    lc.setColumn(0,0+position,B00001000);
    lc.setColumn(0,1+position,B00011111);
  
}

void draw2(int position){

    lc.setColumn(0,0+position,B00010111);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00011101);  
}
void draw3(int position){

    lc.setColumn(0,0+position,B00010001);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00011111);  
}
void draw4(int position){

    lc.setColumn(0,0+position,B00011100);
    lc.setColumn(0,1+position,B00000100);
    lc.setColumn(0,2+position,B00011111);  
}
void draw5(int position){

    lc.setColumn(0,0+position,B00011101);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00010111);  
}
void draw6(int position){

    lc.setColumn(0,0+position,B00011111);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00010111);  
}
void draw7(int position){

    lc.setColumn(0,0+position,B00010000);
    lc.setColumn(0,1+position,B00010011);
    lc.setColumn(0,2+position,B00011100);  
}
void draw8(int position){

    lc.setColumn(0,0+position,B00011111);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00011111);  
}
void draw9(int position){

    lc.setColumn(0,0+position,B00011101);
    lc.setColumn(0,1+position,B00010101);
    lc.setColumn(0,2+position,B00011111);  
}

void draw0(int position){

    lc.setColumn(0,0+position,B00011111);
    lc.setColumn(0,1+position,B00010001);
    lc.setColumn(0,2+position,B00011111);  
}
