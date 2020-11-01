# Libraries
This repository have all the main Otto libraries for Arduino IDE mainly for Nano compatible boards but you can use UNO and others in most cases.

## Installation:
How to install the Arduino IDE and libraries?
"There are multiple ways to add libraries to your Arduino IDE after the software is installed the most easy for me is to:
1. [Download the .zip Otto libraries here](https://github.com/OttoDIY/OttoDIYLib/archive/master.zip)
2. Open Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library. At the top of the drop down list, select the option to "Add .ZIP Library''.
3. You will be prompted to select the library. Navigate to the .zip file's location, that you just downloaded and open it.
4. Return to the Sketch > Include Library menu. menu. You should now see the library at the bottom of the drop-down menu. That means is ready to use Ottos example codes!
[for more details or other ways visit this link](https://www.arduino.cc/en/Guide/Libraries) 
 
## Compatibility:
Otto Core Libraries is the code to be deployed to the user's Documents\Arduino\libraries directory, including: BatReader, EnableInterrupt, MaxMatrix, Oscillator, Otto, OttoSerialCommand
Different and similar versions of Otto Core Libraries are currently included in multiple repositories and sub-projects:

Robot  | Library can use
------------- | -------------
DIY  | Otto9
+ | Otto9
Eyes | Otto9
Humanoid  | Otto9Humanoid
Wheels  | Otto9
Quad | Quad

In order to maintain consistency along the Otto robot remixes we stablished a naming structure: 
Otto_(followed by the component name and version) for example:

* Otto9 contains all the main functions
* Otto_gestures9 contains all the gestures functions
* Otto_mouth9	contains all the mouth functions
* Otto_sound9 contains all the sound functions
* Otto_Matrix9 contains all the matrix functions

Complementary libraries  are independent of Otto(they can work with other projects) therefore are just named like they are, for example:
* US contains the functions of the ultrasound
* Bat Reader contains the functions to meassure the battery.
* All Adafruit libraries can be installed separately
 
 ## Wiring:
We follow these connections in all our robots, buy some Otto do not use all pins so you can play with empty pins to add new sensors or actuators.

SERVOS:
PIN_YL 2 left leg // or Left Wheel
PIN_YR 3 right leg // or Righ Wheel
PIN_RL 4 left foot
PIN_RR 5 right foot
PIN_LA 6 Left arm  // for Humanoid only
PIN_RA 7 Right arm // for Humanoid only

ULTRASONIC:
PIN_Trigger  8  //TRIGGER pin
PIN_Echo    9  //ECHO pin

BLUETOOTH:
STATE pin of bluetooth to 10 // for BLE pairing
TX pin    of bluetooth to 11
RX pin    of bluetooth to 12 

BUZZER:
PIN_Buzzer  13 //BUZZER pin (13)
 
TOUCH SENSOR or PUSH BUTTON 
PIN_Button   A0 // TOUCH SENSOR Pin, PULL DOWN RESISTOR of 1K REQUIRED to stop false interrupts (interrupt PIN)

LED MATRIX 8x8 for the mouth (MAX7219) :
CLK_PIN    A1   //Clock
CS_PIN     A2   //Chip Select
DIN_PIN    A3   //Data In

LED MATRIX 16x8 for the Eyes (i2C)):
SDA    A4   // Synchronous Data
SCL    A5   // Synchronous Clock
More i2C devices can be added in parallel with a different address

SOUND SENSOR:
PIN_NoiseSensor A6  //SOUND SENSOR ANALOG pin

BATTERY SENSE:
boolean BATTcheck = true;    // SET TO FALSE IF NOT USING THIS OPTION
PIN_Battery   A7  //3.7V BATTERY MONITOR ANALOG pin
 
 ## How to Contribute
 Contributing to this software is warmly welcomed. There are 3 ways you can contribute to this project:
1. Test and if find a problem then post an issue.
2. Helps us solve the issues or other bugs by commenting
3. Bring missing libraries from other Otto remixes here.
You can do this [basically by forking](https://help.github.com/en/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/en/articles/about-pull-requests). Please add a change log and your contact into file header.

Thanks for your contribution.
Just make sure to keep consistency in the naming and make a record of the change or improvement made.
Welcome to the Otto DIY code development team!