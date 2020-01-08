//*************************************************************************
//*  Otto's Calibration Tool
//*  (c) 16/07/2016 - Rafael López Verdejo
//*  rlopezverdejo@gmail.com
//*
//*  This program is free software: you can redistribute it and/or modify
//*  it under the terms of the GNU General Public License as published by
//*  the Free Software Foundation, either version 3 of the License, or
//*  (at your option) any later version.
//*
//*  This program is distributed in the hope that it will be useful,
//*  but WITHOUT ANY WARRANTY; without even the implied warranty of
//*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//*  GNU General Public License for more details.
//*
//*  You should have received a copy of the GNU General Public License
//*  along with this program. If not, see <http://www.gnu.org/licenses/>.
//*
//*  Tweaks by Jonathan Hess December 2016 for arm support.
//*************************************************************************

#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <SerialUI.h>

#define N_SERVOS 6 // # servos

//-- First step: Configure the pins where the servos are attached

#define RA_ID 0 // Right ankle ID
#define LA_ID 1 // Left ankle ID
#define RH_ID 2 // Right hip ID
#define LH_ID 3 // Left hip ID
#define RARM_ID 4 // Right Arm ID
#define LARM_ID 5 // Left Arm ID
#define RA_PIN 5 // Right ankle PIN
#define LA_PIN 4 // Left ankle PIN
#define RH_PIN 3 // Right hip PIN
#define LH_PIN 2 // Left hip PIN
#define RARM_PIN 7 // Right arm PIN
#define LARM_PIN 6 // Left arm PIN

#define EEPROM_BASE_ADDRESS 0

// MENU CONFIGURATION

SUI_DeclareString(device_greeting, "Otto's calibration tool\r\nCopyright (C) 2016 Rafael Lopez Verdejo [rlopezverdejo@gmail.com] \r\nGPL License\r\nWrite ? for help.");
SUI_DeclareString(top_menu_title, "Main menu");

SUI_DeclareString(la_key, "la");
SUI_DeclareString(la_help, "Left ankle trim");
SUI_DeclareString(la_plus_key, "+");
SUI_DeclareString(la_plus_help, "Left ankle trim +1");
SUI_DeclareString(la_minus_key, "-");
SUI_DeclareString(la_minus_help, "Left ankle trim -1");

SUI_DeclareString(ra_key, "ra");
SUI_DeclareString(ra_help, "Right ankle trim");
SUI_DeclareString(ra_plus_key, "+");
SUI_DeclareString(ra_plus_help, "Right ankle trim +1");
SUI_DeclareString(ra_minus_key, "-");
SUI_DeclareString(ra_minus_help, "Right ankle trim -1");

SUI_DeclareString(lh_key, "lh");
SUI_DeclareString(lh_help, "Left hip trim");
SUI_DeclareString(lh_plus_key, "+");
SUI_DeclareString(lh_plus_help, "Left hip trim +1");
SUI_DeclareString(lh_minus_key, "-");
SUI_DeclareString(lh_minus_help, "Left hip trim -1");

SUI_DeclareString(rh_key, "rh");
SUI_DeclareString(rh_help, "Right hip trim");
SUI_DeclareString(rh_plus_key, "+");
SUI_DeclareString(rh_plus_help, "Right hip trim +1");
SUI_DeclareString(rh_minus_key, "-");
SUI_DeclareString(rh_minus_help, "Right hip trim -1");

// New experimental code here:
SUI_DeclareString(rarm_key, "rr");
SUI_DeclareString(rarm_help, "Right arm trim");
SUI_DeclareString(rarm_plus_key, "+");
SUI_DeclareString(rarm_plus_help, "Right arm trim +1");
SUI_DeclareString(rarm_minus_key, "-");
SUI_DeclareString(rarm_minus_help, "Right arm trim -1");

SUI_DeclareString(larm_key, "lr");
SUI_DeclareString(larm_help, "Left arm trim");
SUI_DeclareString(larm_plus_key, "+");
SUI_DeclareString(larm_plus_help, "Left arm trim +1");
SUI_DeclareString(larm_minus_key, "-");
SUI_DeclareString(larm_minus_help, "Left arm trim -1");
// End new code

SUI_DeclareString(info_key, "info");
SUI_DeclareString(info_help, "Show the trim values loaded into memory");

SUI_DeclareString(save_key, "save");
SUI_DeclareString(save_help, "Save memory trim values to EEPROM");

SUI_DeclareString(load_key, "load");
SUI_DeclareString(load_help, "Load memory trim values from EEPROM");

SUI_DeclareString(reset_key, "reset");
SUI_DeclareString(reset_help, "Reset memory trim values");

SUI::SerialUI mySUI = SUI::SerialUI(device_greeting);

Oscillator servo[N_SERVOS];

void setup()
{
  mySUI.begin(9600);
  mySUI.setMaxIdleMs(60000);

  servo[RA_ID].attach(RA_PIN);
  servo[LA_ID].attach(LA_PIN);
  servo[RH_ID].attach(RH_PIN);
  servo[LH_ID].attach(LH_PIN);

  // New code

  servo[LARM_ID].attach(LARM_PIN);
  servo[RARM_ID].attach(RARM_PIN);

  // End New Code

  resetServos();
  // CB_load();

  SUI::Menu * mainMenu = mySUI.topLevelMenu();
  mainMenu->setName(top_menu_title);
  SUI::Menu * laMenu = mainMenu->subMenu(la_key, la_help);
  laMenu->addCommand(la_plus_key, CB_leftAnklePlus, la_plus_help);
  laMenu->addCommand(la_minus_key, CB_leftAnkleMinus, la_minus_help);
  SUI::Menu * raMenu = mainMenu->subMenu(ra_key, ra_help);
  raMenu->addCommand(ra_plus_key, CB_rightAnklePlus, ra_plus_help);
  raMenu->addCommand(ra_minus_key, CB_rightAnkleMinus, ra_minus_help);
  SUI::Menu * lhMenu = mainMenu->subMenu(lh_key, lh_help);
  lhMenu->addCommand(lh_plus_key, CB_leftHipPlus, lh_plus_help);
  lhMenu->addCommand(lh_minus_key, CB_leftHipMinus, lh_minus_help);

  SUI::Menu * rhMenu = mainMenu->subMenu(rh_key, rh_help);
  rhMenu->addCommand(rh_plus_key, CB_rightHipPlus, rh_plus_help);
  rhMenu->addCommand(rh_minus_key, CB_rightHipMinus, rh_minus_help);

  // New code

  SUI::Menu * rarmMenu = mainMenu->subMenu(rarm_key, rarm_help);
  rarmMenu->addCommand(rarm_plus_key, CB_rightArmPlus, rarm_plus_help);
  rarmMenu->addCommand(rarm_minus_key, CB_rightArmMinus, rarm_minus_help);

  SUI::Menu * larmMenu = mainMenu->subMenu(larm_key, larm_help);
  larmMenu->addCommand(larm_plus_key, CB_leftArmPlus, larm_plus_help);
  larmMenu->addCommand(larm_minus_key, CB_leftArmMinus, larm_minus_help);

  // End new code


  mainMenu->addCommand(info_key, CB_info, info_help);
  mainMenu->addCommand(save_key, CB_save, save_help);
  mainMenu->addCommand(load_key, CB_load, load_help);
  mainMenu->addCommand(reset_key, CB_reset, reset_help);
}

void loop()
{
  if (mySUI.checkForUser(150)) {
    mySUI.enter();

    while (mySUI.userPresent())
    {
      mySUI.handleRequests();
    }
  }
}

/************ CALLBACKS *************/

void CB_leftAnklePlus()
{
  servo[LA_ID].SetTrim(servo[LA_ID].getTrim() + 1);
  servo[LA_ID].SetPosition(90);
}

void CB_leftAnkleMinus()
{
  servo[LA_ID].SetTrim(servo[LA_ID].getTrim() - 1);
  servo[LA_ID].SetPosition(90);
}

void CB_rightAnklePlus()
{
  servo[RA_ID].SetTrim(servo[RA_ID].getTrim() + 1);
  servo[RA_ID].SetPosition(90);
}

void CB_rightAnkleMinus()
{
  servo[RA_ID].SetTrim(servo[RA_ID].getTrim() - 1);
  servo[RA_ID].SetPosition(90);
}

void CB_leftHipPlus()
{
  servo[LH_ID].SetTrim(servo[LH_ID].getTrim() + 1);
  servo[LH_ID].SetPosition(90);
}

void CB_leftHipMinus()
{
  servo[LH_ID].SetTrim(servo[LH_ID].getTrim() - 1);
  servo[LH_ID].SetPosition(90);
}

void CB_rightHipPlus()
{
  servo[RH_ID].SetTrim(servo[RH_ID].getTrim() + 1);
  servo[RH_ID].SetPosition(90);
}

void CB_rightHipMinus()
{
  servo[RH_ID].SetTrim(servo[RH_ID].getTrim() - 1);
  servo[RH_ID].SetPosition(90);
}

// New code

void CB_rightArmPlus()
{
  servo[RARM_ID].SetTrim(servo[RARM_ID].getTrim() + 1);
  servo[RARM_ID].SetPosition(90);
}

void CB_rightArmMinus()
{
  servo[RARM_ID].SetTrim(servo[RARM_ID].getTrim() - 1);
  servo[RARM_ID].SetPosition(90);
}


void CB_leftArmPlus()
{
  servo[LARM_ID].SetTrim(servo[LARM_ID].getTrim() + 1);
  servo[LARM_ID].SetPosition(90);
}

void CB_leftArmMinus()
{
  servo[LARM_ID].SetTrim(servo[LARM_ID].getTrim() - 1);
  servo[LARM_ID].SetPosition(90);
}

// End new code

void CB_info()
{
  showTrims();
}

void CB_save()
{
  // SAVE EEPROM VALUES

  for (int i = 0; i < N_SERVOS; i++) {
    EEPROM.write(EEPROM_BASE_ADDRESS + i, servo[i].getTrim());
    mySUI.println(EEPROM_BASE_ADDRESS + i, servo[i].getTrim());
  }

  mySUI.println("Trim values saved");
}

void CB_load()
{
  int trim;

  // LOAD EEPROM VALUES

  for (int i = 0; i < N_SERVOS; i++)
  {
    trim = EEPROM.read(EEPROM_BASE_ADDRESS + i);

    // new code
    //if (trim > 128) trim = trim - 256;
    // end new code
    
    servo[i].SetTrim(trim);
  }

  showTrims();
  centerServos();
}

void CB_reset()
{
  resetServos();
  showTrims();
  centerServos();
}


/************ FUNCTIONS *************/

void centerServos()
{
  for (int i = 0; i < N_SERVOS; i++) servo[i].SetPosition(90);
  delay(200);
}

void resetServos()
{
  // RESET MEMORY VALUES

  for (int i = 0; i < N_SERVOS; i++)
  {
    servo[i].SetTrim(0);
  }
}

void showTrims()
{
  mySUI.print("Right ankle: ");
  mySUI.println(servo[RA_ID].getTrim());
  mySUI.print("Left ankle: ");
  mySUI.println(servo[LA_ID].getTrim());
  mySUI.print("Right hip: ");
  mySUI.println(servo[RH_ID].getTrim());+
  mySUI.print("Left hip: ");
  mySUI.println(servo[LH_ID].getTrim());

  // New code

  mySUI.print("Left arm: ");
  mySUI.println(servo[LARM_ID].getTrim());

  mySUI.print("Right arm: ");
  mySUI.println(servo[RARM_ID].getTrim());

  // end new code }
