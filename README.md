# Libraries
A purpose of this repository is to discuss and document properly the Otto libraries for Arduino IDE.

## Installation:
How do I setup the Arduino IDE and libraries? or How to install the libraries (not Otto specific) ?
Answer: "There are multiple ways to add libraries to your Arduino IDE after installed the most easy for me is to just Download all the libraries from the files tab for the specific project and move them to C:\Users\user\Documents\Arduino\libraries (or wherever your libraries folder was installed)
Other ways use .zip installation through menu or even automatic updates online(i wish i knew how to do this for Otto) but i never need to do it this way, is a very general topic that all new Arduino users face and there is a lot information by just googling.
﻿https://www.arduino.cc/en/Guide/Libraries﻿
 
## Compatibility
Otto Core Libraries is the code to be deployed to the user's Documents\Arduino\libraries directory, including: BatReader, EnableInterrupt, MaxMatrix, Oscillator, Otto, OttoSerialCommand
Different and similar versions of Otto Core Libraries are currently included in multiple repositories and sub-projects:

Robot  | Library can use
------------- | -------------
DIY | Otto9
DIY+  | Otto9
Humanoid  | Otto9
Q (Quad) | Quad
X(explorer)  | Otto9
R (Rover)  | Otto9
W (Wheels)  | Otto9

To resolve the need to maintain so many different deployments/versions here in this centralized repository, in order to maintin consistency along the Otto remixes we stablished a naming structure: 
Otto_(followed by the component name and version) for example:

* Otto_9 contains all the main functions and is the version 9
* Otto_gestures9 contains all the gestures functions
* Otto_mouth9	contains all the mouth functions
* Otto_sound9 contains all the sound functions
* Otto_Matrix9 contains all the matrix functions

Complementary libraries like Ultrasound, Interrupts and BatReader are independent of Otto(they can work with other projects) therefore are just named like they are, for example:
* US contains the functions of the ultrasound
* Bat Reader contains the functions to meassure the battery.
* Adafruit_NeoPixel external library from Adafruit to control NeoPixel LEDs

Project releases could include a (zipped) version of the Core Libraries that work with that release.
If a project needs a change that should not be part of the Core Libraries, then that project repository may be a good home for those changes.
Reference Information for Arduino Libraries:
﻿https://www.arduino.cc/en/Reference/APIStyleGuide﻿
﻿https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification﻿
﻿https://www.arduino.cc/en/Hacking/LibraryTutorial﻿
 
 ## Wirings and Electornic diagrams:
 It vary pero Otto but we will follow these standard connections
 <img src="https://github.com/OttoDIY/Libraries/blob/master/OttoDIY_wirings.jpg" width="500" align="center">
 
 ## Contribute
 There are 3 ways you can contribute to this project:
1. Test and if find a problem then post an issue.
2. Helps us solve the issues or other bugs by making pull request of the changes.
3. Bring missing libraries from other Otto remixes here.

Just make sure to keep consistency in the naming and make a record of the change or improvement made.
Welcome to the Otto code development team!
