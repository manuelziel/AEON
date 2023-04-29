/*
This is the Code for AEON Lifetime Clock

The Project uses the following components:

- Display SSD1306 128x64 Pixel (I2C)
  - EA OLEDM128-6LWA or EA OLEDL128-6LWA
- RTC
  - DS3231SN (I2C)
- NOR Flash
  - W25Q128JWPIQ (SPI) (128 Mbit - 16MB)
- RP2040 microcontroller (16MB noFS / 8MB-8MB FS, Generic SPI/2, 133MHz, C++ Exceptions Enabled, Pico SDK)

Pick one up today!
------> http://www.ManuelZiel.com

Manuel Ziel invests time and resources providing this open
source code, please support AEON and open-source
hardware by purchasing products from Manuel Ziel!

Written by Manuel Ziel, with contributions from the open source community.
Special thanks to Daniel Fritsch.

MIT license, check license.txt for more information.

This example is for a display using I2C to communicate
3 pins are required to interface (two I2C and one reset).

Declaration for an SSD1306 display connected to I2C (SDA, SCL)
The pins for I2C are defined by the Wire-library.
On an AEON:       4(SDA), 5(SCL), 3(RESET)
On an Jupiter:    20(SDA), 21(SCL)
*/

/*
  Includes
*/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "AEON_ROM.h"
#include "AEON_Time.h"
#include "AEON_Display.h"
#include "AEON_FSM.h"
#include "AEON_Button.h"

/*
  Defines
*/
FSM fsm;
AEON_ROM rom;
AEON_Display aeon;
AEON_Time timer;
AEON_Button buttons[] = {AEON_Button(22, "SET"), AEON_Button(23, "+"), AEON_Button(24, "-"), AEON_Button(25, "OK")};

typedef struct
{
  EReturn_ROM return_ROM;
  EReturn_TIME return_TIME;
  EReturn_DISPLAY return_DISPLAY;
} SGLOBAL_ERROR_STATES;

SGLOBAL_ERROR_STATES globalErrorStates = {EReturn_ROM::ROM_RETURN_NULL, EReturn_TIME::TIME_RETURN_NULL, EReturn_DISPLAY::DISPLAY_RETURN_NULL};

/*
  Main prototypes
*/
void setup()
{
  Serial.begin(9600);

  // while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB
  //}

  // Setup ROM, Time and Display
  globalErrorStates.return_ROM = rom.setupEEPROM();       // load the init and saved values
  globalErrorStates.return_TIME = timer.setupTime();      // load the time and date
  globalErrorStates.return_DISPLAY = aeon.setupDisplay(); // start and set the display

  // Setup Buttons
  for (int i = 0; i < 4; i++)
  {
    buttons[i].setupButton();
  }

  // Finite State Machine
  // Base -> Setup (Current STATE?, NULL, NULL, NULL)
  fsm.addState((StateId)(STATE_Base), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET), // Event
                      NULL,                 // Guard
                      []()
                      {
                        aeon.pageSetupTime();
                      },                           // Transition
                      (StateId)(STATE_Setup_Time)) // Next State
      ->end();

  // SetupTime -> Setup_Time_Set_Hour || SetupBack
  fsm.addState((StateId)(STATE_Setup_Time), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                 // Event
                      NULL,                                 // Guard
                      /*&page_setup_*/ NULL,                // Transition
                      (StateId)(STATE_Setup_Time_Set_Hour)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupDate();
                      },                           // Transition
                      (StateId)(STATE_Setup_Date)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupBack();
                      },                           // Transition
                      (StateId)(STATE_Setup_Back)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK),                  // Event
                      NULL,                                 // Guard
                      NULL,                                 // Transition
                      (StateId)(STATE_Setup_Time_Set_Hour)) // Next State
      ->end();

  // Setup_Time_Set_Hour -> Setup_Time_Set_Minute
  fsm.addState((StateId)(STATE_Setup_Time_Set_Hour), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                   // Event
                      NULL,                                   // Guard
                      /*&page_setup_*/ NULL,                  // Transition
                      (StateId)(STATE_Setup_Time_Set_Minute)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setHour(1);
                      },                                    // Transition
                      (StateId)(STATE_Setup_Time_Set_Hour)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setHour(-1);
                      },                                    // Transition
                      (StateId)(STATE_Setup_Time_Set_Hour)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupTime();
                      },                           // Transition
                      (StateId)(STATE_Setup_Time)) // Next State
      ->end();

  // Setup_Time_Set_Minute -> Setup_Time_Set_Second
  fsm.addState((StateId)(STATE_Setup_Time_Set_Minute), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                   // Event
                      NULL,                                   // Guard
                      /*&page_setup_*/ NULL,                  // Transition
                      (StateId)(STATE_Setup_Time_Set_Second)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setMinute(1);
                      },                                      // Transition
                      (StateId)(STATE_Setup_Time_Set_Minute)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setMinute(-1);
                      },                                      // Transition
                      (StateId)(STATE_Setup_Time_Set_Minute)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupTime();
                      },                           // Transition
                      (StateId)(STATE_Setup_Time)) // Next State
      ->end();

  // Setup_Time_Set_Second -> Setup_Back
  fsm.addState((StateId)(STATE_Setup_Time_Set_Second), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                 // Event
                      NULL,                                 // Guard
                      NULL,                                 // Transition
                      (StateId)(STATE_Setup_Time_Set_Hour)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setSecond(1);
                      },                                      // Transition
                      (StateId)(STATE_Setup_Time_Set_Second)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setSecond(-1);
                      },                                      // Transition
                      (StateId)(STATE_Setup_Time_Set_Second)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupTime();
                      },                           // Transition
                      (StateId)(STATE_Setup_Time)) // Next State
      ->end();

  // SetupDate -> Setup_Date_Set_Year || Setup_Back
  fsm.addState((StateId)(STATE_Setup_Date), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                 // Event
                      NULL,                                 // Guard
                      NULL,                                 // Transition
                      (StateId)(STATE_Setup_Date_Set_Year)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupBirthday();
                      },                               // Transition
                      (StateId)(STATE_Setup_Birthday)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupTime();
                      },                           // Transition
                      (StateId)(STATE_Setup_Time)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK),                  // Event
                      NULL,                                 // Guard
                      NULL,                                 // Transition
                      (StateId)(STATE_Setup_Date_Set_Year)) // Next State
      ->end();

  // Setup_Date_Set_Year -> Setup_Date_Set_Month
  fsm.addState((StateId)(STATE_Setup_Date_Set_Year), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                  // Event
                      NULL,                                  // Guard
                      NULL,                                  // Transition
                      (StateId)(STATE_Setup_Date_Set_Month)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setYear(1);
                      },                                    // Transition
                      (StateId)(STATE_Setup_Date_Set_Year)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setYear(-1);
                      },                                    // Transition
                      (StateId)(STATE_Setup_Date_Set_Year)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupDate();
                      },                           // Transition
                      (StateId)(STATE_Setup_Date)) // Next State
      ->end();

  // Setup_Date_Set_Month -> Setup_Date_Set_Month
  fsm.addState((StateId)(STATE_Setup_Date_Set_Month), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                // Event
                      NULL,                                // Guard
                      NULL,                                // Transition
                      (StateId)(STATE_Setup_Date_Set_Day)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setMonth(1);
                      },                                     // Transition
                      (StateId)(STATE_Setup_Date_Set_Month)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setMonth(-1);
                      },                                     // Transition
                      (StateId)(STATE_Setup_Date_Set_Month)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupDate();
                      },                           // Transition
                      (StateId)(STATE_Setup_Date)) // Next State
      ->end();

  // Setup_Date_Set_Day -> Setup_Date
  fsm.addState((StateId)(STATE_Setup_Date_Set_Day), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                 // Event
                      NULL,                                 // Guard
                      NULL,                                 // Transition
                      (StateId)(STATE_Setup_Date_Set_Year)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setDay(1);
                      },                                   // Transition
                      (StateId)(STATE_Setup_Date_Set_Day)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        timer.setDay(-1);
                      },                                   // Transition
                      (StateId)(STATE_Setup_Date_Set_Day)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupDate();
                      },                           // Transition
                      (StateId)(STATE_Setup_Date)) // Next State
      ->end();

  // Setup_Birthday -> Setup_Birthday_Year || Setup_Back
  fsm.addState((StateId)(STATE_Setup_Birthday), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                     // Event
                      NULL,                                     // Guard
                      NULL,                                     // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Year)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupSex();
                      },                          // Transition
                      (StateId)(STATE_Setup_Sex)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupDate();
                      },                           // Transition
                      (StateId)(STATE_Setup_Date)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK),                      // Event
                      NULL,                                     // Guard
                      NULL,                                     // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Year)) // Next State
      ->end();

  // Setup_Birthday_Set_Year -> Setup_Birthday_Set_Month
  fsm.addState((StateId)(STATE_Setup_Birthday_Set_Year), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                      // Event
                      NULL,                                      // Guard
                      NULL,                                      // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Month)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setBirthdayYear(+1);
                        rom.saveToEEPROM();
                      },                                        // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Year)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setBirthdayYear(-1);
                        rom.saveToEEPROM();
                      },                                        // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Year)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupBirthday();
                      },                               // Transition
                      (StateId)(STATE_Setup_Birthday)) // Next State
      ->end();

  // Setup_Birthday_Set_Month -> Setup_Birthday_Set_Day
  fsm.addState((StateId)(STATE_Setup_Birthday_Set_Month), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                    // Event
                      NULL,                                    // Guard
                      NULL,                                    // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Day)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setBirthdayMonth(+1);
                        rom.saveToEEPROM();
                      },                                         // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Month)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setBirthdayMonth(-1);
                        rom.saveToEEPROM();
                      },                                         // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Month)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupBirthday();
                      },                               // Transition
                      (StateId)(STATE_Setup_Birthday)) // Next State
      ->end();

  // Setup_Birthday_Set_Day -> Setup_Birthday_Set_Year
  fsm.addState((StateId)(STATE_Setup_Birthday_Set_Day), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                     // Event
                      NULL,                                     // Guard
                      NULL,                                     // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Year)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setBirthdayDay(+1);
                        rom.saveToEEPROM();
                      },                                       // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Day)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setBirthdayDay(-1);
                        rom.saveToEEPROM();
                      },                                       // Transition
                      (StateId)(STATE_Setup_Birthday_Set_Day)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupBirthday();
                      },                               // Transition
                      (StateId)(STATE_Setup_Birthday)) // Next State
      ->end();

  // Setup_Sex -> Setup_Sex_Set
  fsm.addState((StateId)(STATE_Setup_Sex), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),           // Event
                      NULL,                           // Guard
                      NULL,                           // Transition
                      (StateId)(STATE_Setup_Sex_Set)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupLifespan();
                      },                               // Transition
                      (StateId)(STATE_Setup_Lifespan)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupBirthday();
                      },                               // Transition
                      (StateId)(STATE_Setup_Birthday)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupSex();
                      },                              // Transition
                      (StateId)(STATE_Setup_Sex_Set)) // Next State
      ->end();

  // Setup_Sex_Set -> Setup_Sex
  fsm.addState((StateId)(STATE_Setup_Sex_Set), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),           // Event
                      NULL,                           // Guard
                      NULL,                           // Transition
                      (StateId)(STATE_Setup_Sex_Set)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setSex();
                        rom.saveToEEPROM();
                      },                              // Transition
                      (StateId)(STATE_Setup_Sex_Set)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setSex();
                        rom.saveToEEPROM();
                      },                              // Transition
                      (StateId)(STATE_Setup_Sex_Set)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupSex();
                      },                          // Transition
                      (StateId)(STATE_Setup_Sex)) // Next State
      ->end();

  // Setup_Lifespan -> Setup_Lifespan_Set
  fsm.addState((StateId)(STATE_Setup_Lifespan), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),                // Event
                      NULL,                                // Guard
                      NULL,                                // Transition
                      (StateId)(STATE_Setup_Lifespan_Set)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupReset();
                      },                            // Transition
                      (StateId)(STATE_Setup_Reset)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupSex();
                      },                          // Transition
                      (StateId)(STATE_Setup_Sex)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK),                 // Event
                      NULL,                                // Guard
                      /*&pageSetupSex*/ NULL,              // Transition
                      (StateId)(STATE_Setup_Lifespan_Set)) // Next State
      ->end();

  // Setup_Lifespan_Set -> Setup_Lifespan
  fsm.addState((StateId)(STATE_Setup_Lifespan_Set), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),            // Event
                      NULL,                            // Guard
                      NULL,                            // Transition
                      (StateId)(STATE_Setup_Lifespan)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setLifespan(+1);
                        rom.saveToEEPROM();
                      },                                   // Transition
                      (StateId)(STATE_Setup_Lifespan_Set)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        rom.setLifespan(-1);
                        rom.saveToEEPROM();
                      },                                   // Transition
                      (StateId)(STATE_Setup_Lifespan_Set)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupLifespan();
                      },                               // Transition
                      (StateId)(STATE_Setup_Lifespan)) // Next State
      ->end();

  // Setup_Birthday -> Setup_Reset
  fsm.addState((StateId)(STATE_Setup_Reset), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),            // Event
                      NULL,                            // Guard
                      NULL,                            // Transition
                      (StateId)(STATE_Setup_Reset_No)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupBack();
                      },                           // Transition
                      (StateId)(STATE_Setup_Back)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupLifespan();
                      },                               // Transition
                      (StateId)(STATE_Setup_Lifespan)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK),             // Event
                      NULL,                            // Guard
                      /*&page_base*/ NULL,             // Transition
                      (StateId)(STATE_Setup_Reset_No)) // Next State
      ->end();

  // Setup_Reset_No -> Setup_Reset_Yes
  fsm.addState((StateId)(STATE_Setup_Reset_No), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),             // Event
                      NULL,                             // Guard
                      NULL,                             // Transition
                      (StateId)(STATE_Setup_Reset_Yes)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P),               // Event
                      NULL,                             // Guard
                      /*&pageSetupReset*/ NULL,         // Transition
                      (StateId)(STATE_Setup_Reset_Yes)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N),               // Event
                      NULL,                             // Guard
                      /*&pageSetupReset*/ NULL,         // Transition
                      (StateId)(STATE_Setup_Reset_Yes)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        aeon.pageSetupReset();
                      },                            // Transition
                      (StateId)(STATE_Setup_Reset)) // Next State
      ->end();

  // Setup_Reset_No -> Setup_Reset_Yes
  fsm.addState((StateId)(STATE_Setup_Reset_Yes), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),            // Event
                      NULL,                            // Guard
                      NULL,                            // Transition
                      (StateId)(STATE_Setup_Reset_No)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P),              // Event
                      NULL,                            // Guard
                      /*&pageSetupReset*/ NULL,        // Transition
                      (StateId)(STATE_Setup_Reset_No)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N),              // Event
                      NULL,                            // Guard
                      /*&pageSetupReset*/ NULL,        // Transition
                      (StateId)(STATE_Setup_Reset_No)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK),                // Event
                      NULL,                               // Guard
                      /*&pageSetupReset*/ NULL,           // Transition
                      (StateId)(STATE_Setup_Reset_Count)) // Next State
      ->end();

  // Setup_Reset_No -> Setup_Reset_Yes
  fsm.addState((StateId)(STATE_Setup_Reset_Count), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),            // Event
                      NULL,                            // Guard
                      NULL,                            // Transition
                      (StateId)(STATE_Setup_Reset_No)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P),              // Event
                      NULL,                            // Guard
                      /*&pageSetupReset*/ NULL,        // Transition
                      (StateId)(STATE_Setup_Reset_No)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N),              // Event
                      NULL,                            // Guard
                      /*&pageSetupReset*/ NULL,        // Transition
                      (StateId)(STATE_Setup_Reset_No)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        resetFinal(false);
                      },                               // Transition
                      (StateId)(STATE_Setup_Reset_No)) // Next State
      ->end();

  // Setup_Back -> Setup_Time || Base
  fsm.addState((StateId)(STATE_Setup_Back), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET),  // Event
                      NULL,                  // Guard
                      NULL,                  // Transition
                      (StateId)(STATE_Base)) // Next State

      // P
      ->addTransition((EventId)(EVENT_P), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupTime();
                      },                           // Transition
                      (StateId)(STATE_Setup_Time)) // Next State

      // N
      ->addTransition((EventId)(EVENT_N), // Event
                      NULL,               // Guard
                      []()
                      {
                        aeon.pageSetupReset();
                      },                            // Transition
                      (StateId)(STATE_Setup_Reset)) // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK),   // Event
                      NULL,                  // Guard
                      /*&page_base*/ NULL,   // Transition
                      (StateId)(STATE_Base)) // Next State
      ->end();

  // ERROR -> Base
  fsm.addState((StateId)(STATE_ERROR), NULL, NULL, NULL)
      // SET
      ->addTransition((EventId)(EVENT_SET), // Event
                      NULL,                 // Guard
                      []()
                      {
                        globalErrorStates.return_ROM = EReturn_ROM::ROM_RETURN_NULL;
                        globalErrorStates.return_TIME = EReturn_TIME::TIME_RETURN_NULL;
                        globalErrorStates.return_DISPLAY = EReturn_DISPLAY::DISPLAY_RETURN_NULL;
                        rom.resetErrorStateRom();
                        timer.resetErrorStateTime();
                        aeon.resetErrorStateDisplay();
                      },                     // Transition
                      (StateId)(STATE_Base)) // Next State

      /*/ P
        ->addTransition((EventId)(EVENT_P), // Event
                        NULL,               // Guard
                        NULL,               // Transition
                        NULL)               // Next State

        // N
        ->addTransition((EventId)(EVENT_N), // Event
                        NULL,               // Guard
                        NULL,               // Transition
                        NULL)*/
      // Next State

      // OK
      ->addTransition((EventId)(EVENT_OK), // Event
                      NULL,                // Guard
                      []()
                      {
                        globalErrorStates.return_ROM = EReturn_ROM::ROM_RETURN_NULL;
                        globalErrorStates.return_TIME = EReturn_TIME::TIME_RETURN_NULL;
                        globalErrorStates.return_DISPLAY = EReturn_DISPLAY::DISPLAY_RETURN_NULL;
                        rom.resetErrorStateRom();
                        timer.resetErrorStateTime();
                        aeon.resetErrorStateDisplay();
                      },                     // Transition
                      (StateId)(STATE_Base)) // Next State
      ->end();

  // Check error and jump to the error state, if no error exist the state jump to the normal base state
  if (globalErrorStates.return_ROM == EReturn_ROM::ROM_RETURN_NULL || globalErrorStates.return_TIME == EReturn_TIME::TIME_RETURN_NULL || globalErrorStates.return_DISPLAY == EReturn_DISPLAY::DISPLAY_RETURN_NULL)
  {
    fsm.setCurrentStateId((StateId)(STATE_Base));
  }
  else
  {
    fsm.setCurrentStateId((StateId)(STATE_ERROR));

    /*
    Check EEPROM that exist a init and no commit error in the EEPROM
    when no init exist the defaults are set to the time from the rtc
    and hardcoded defaults.
    */
    if ((globalErrorStates.return_ROM == EReturn_ROM::ERROR_EEPROM_COMMIT_FAILD) || (globalErrorStates.return_ROM == ERROR_EEPROM_NOT_VALID_DATA))
    {
      Serial.println("Error EEPROM reset EEPROM");
      rom.resetEEPROM();
    }
  }
}

/*
The loop to get the time every second, check if the button is pressed
and jump or refresh the current page. If an error comes up the state
change to the error state.
*/
void loop()
{
  loopTime();
  loopButton();
  loopPages();
  loopError();
}

/*
Check for errors and change to the error state
*/
void loopError()
{
  // Check error and change the error state
  if (rom.getErrorState() != EReturn_ROM::ROM_RETURN_NULL || timer.getErrorState() != EReturn_TIME::TIME_RETURN_NULL || aeon.getErrorState() != EReturn_DISPLAY::DISPLAY_RETURN_NULL)
  {
    globalErrorStates.return_ROM = rom.getErrorState();
    globalErrorStates.return_TIME = timer.getErrorState();
    globalErrorStates.return_DISPLAY = aeon.getErrorState();

    fsm.setCurrentStateId((StateId)(STATE_ERROR));
  }
}

/*
Update the current time every 1000ms and print it to serial.
*/
const long interval_time = 1000;
unsigned long currentMillis_time = 0;
unsigned long previousMillis_time = 0;

void loopTime()
{
  currentMillis_time = millis();
  if (currentMillis_time - previousMillis_time >= interval_time)
  {
    previousMillis_time = currentMillis_time;

    timer.updateTime();
    Serial.println(timer.getTimeAsString());
  }
}

/*
Check if the button is pressed. It can be recognized between two states.
One state is a short pressed button until 1500ms and a long pressed button
over 1500ms. Buttons SET = 0; P = 1; N = 2; OK = 3
*/
void loopButton()
{
  for (int i = 0; i < 4; i++)
  {
    switch (buttons[i].loopButton())
    {
    // Pressed short
    case (EPressed::SHORT):
      fsm.dispatch(static_cast<EEvents>(i));
      break;

    // Pressed long
    case (EPressed::LONG):
      // fsm.dispatch(static_cast<Events>(i));
      break;

    default:
      break;
    }
  }
}

/*
The loop from the pages and refresh every loop. Time and date comes
direct from the RTC.
*/
void loopPages()
{
  DateTime now = timer.getTimeAsDateTime();

  switch (fsm.getCurrentStateId())
  {
  case (EStates::STATE_Base):
    aeon.pageBase(now.year(), now.month() - 1, now.day(), now.dayOfTheWeek(), now.hour(), now.minute(), now.second(), calcLifetime());
    break;

  case (EStates::STATE_Setup_Date_Set_Year):
    aeon.pageSetupDate_set_date(aeon.Year, now.year(), now.month() - 1, now.day());
    break;

  case (EStates::STATE_Setup_Date_Set_Month):
    aeon.pageSetupDate_set_date(aeon.Month, now.year(), now.month() - 1, now.day());
    break;

  case (EStates::STATE_Setup_Date_Set_Day):
    aeon.pageSetupDate_set_date(aeon.Day, now.year(), now.month() - 1, now.day());
    break;

  case (EStates::STATE_Setup_Time_Set_Hour):
    aeon.pageSetupTime_set_time(aeon.Hour, now.hour(), now.minute(), now.second());
    break;

  case (EStates::STATE_Setup_Time_Set_Minute):
    aeon.pageSetupTime_set_time(aeon.Minute, now.hour(), now.minute(), now.second());
    break;

  case (EStates::STATE_Setup_Time_Set_Second):
    aeon.pageSetupTime_set_time(aeon.Second, now.hour(), now.minute(), now.second());
    break;

  case (EStates::STATE_Setup_Birthday_Set_Year):
    aeon.pageSetupBirthday_set_date(aeon.B_Year, rom.getBirthdayYear(), rom.getBirthdayMonth(), rom.getBirthdayDay());
    break;

  case (EStates::STATE_Setup_Birthday_Set_Month):
    aeon.pageSetupBirthday_set_date(aeon.B_Month, rom.getBirthdayYear(), rom.getBirthdayMonth(), rom.getBirthdayDay());
    break;

  case (EStates::STATE_Setup_Birthday_Set_Day):
    aeon.pageSetupBirthday_set_date(aeon.B_Day, rom.getBirthdayYear(), rom.getBirthdayMonth(), rom.getBirthdayDay());
    break;

  case (EStates::STATE_Setup_Sex_Set):
    aeon.pageSetupSex_set(rom.getSex());
    break;

  case (EStates::STATE_Setup_Lifespan_Set):
    aeon.pageSetupLifespan_set(rom.getLifespan());
    break;

  case (EStates::STATE_Setup_Reset_Yes):
    aeon.pageSetupReset_set(aeon.Reset_Yes);
    break;

  case (EStates::STATE_Setup_Reset_No):
    aeon.pageSetupReset_set(aeon.Reset_No);
    break;

  case (EStates::STATE_Setup_Reset_Count):
    resetFinal(true);
    break;

  case (EStates::STATE_ERROR):
    pageStateError();
    break;

  default:
    break;
  }
}

/*
An Error State exist. This check the state and generate the string
for the error page.
*/
const long interval_error = 1000;
unsigned long currentMillis_error = 0;
unsigned long previousMillis_error = 0;

void pageStateError()
{
  char localText[24];                    // Display can only 20 chars at one line
  String noError = "No Error detected";  // 17 chars - No Error detected
  String notValid = "No data in EEPROM"; // 17 chars - EEPROM has no data sign is valid
  String commitFaild = "Error EEPROM";   // EEPROM error while save data commit error
  String noRTC = "RTC or I2C";           // Can't find RTC-Clock
  String rtcLostPower = "Fault Battery"; // RTC-Clock lost Power
  String noDisplay = "Display or I2C";   // Display not found

  // This must change if error exist
  int charLengh = 22;
  noError.toCharArray(localText, charLengh);

  // Check global rom
  switch (globalErrorStates.return_ROM)
  {
  case EReturn_ROM::ERROR_EEPROM_NOT_VALID_DATA:
    notValid.toCharArray(localText, charLengh);
    break;

  case EReturn_ROM::ERROR_EEPROM_COMMIT_FAILD:
    commitFaild.toCharArray(localText, charLengh);
    break;

  default:
    break;
  }

  // Check global time
  switch (globalErrorStates.return_TIME)
  {
  case EReturn_TIME::ERROR_TIME_NO_RTC:
    noRTC.toCharArray(localText, charLengh);
    break;

  case EReturn_TIME::ERROR_TIME_LOST_POWER:
    rtcLostPower.toCharArray(localText, charLengh);
    break;

  default:
    break;
  }

  // Check global display
  switch (globalErrorStates.return_DISPLAY)
  {
  case EReturn_DISPLAY::ERROR_DISPLAY_ALLOCATION_FAILD:
    noDisplay.toCharArray(localText, charLengh);
    break;

  default:
    break;
  }

  currentMillis_error = millis();
  if (currentMillis_error - previousMillis_error >= interval_error)
  {
    previousMillis_error = currentMillis_error;
    Serial.println(localText);
  }

  // return local text to error page;
  aeon.pageERROR(localText);
}

/*
A reset should be carried out. Check whether the reset should
be aborted. No abort than reset the ROM and reboot.
*/
const long interval_reset = 1000;
unsigned long currentMillis_reset;
unsigned long previousMillis_reset;
int cnt_reset = 4;

void resetFinal(boolean reset)
{
  currentMillis_reset = millis();
  if (!reset)
  {
    cnt_reset = 4;
    Serial.println("RESET ABORT");
  }
  else if ((currentMillis_reset - previousMillis_reset >= interval_reset) && reset)
  {
    // save the last time
    previousMillis_reset = currentMillis_reset;
    if (cnt_reset > -1)
    {
      aeon.pageSetupReset_count_final(cnt_reset);
      cnt_reset--;
    }
    else if (cnt_reset <= -1)
    {
      // Reset init and for the next start set the defaults.
      rom.resetEEPROM();
      // Restart the System
      NVIC_SystemReset();
    }
  }
}

/*
Calculate the lifepan to the death. The brain of the whole thing.
*/
int calcLifetime()
{
  int lifespan = rom.getLifespan();

  // Calculate the lifespan to Unix-Time
  int zero = 1970;
  int deathYear = zero + lifespan;

  double unix_distance = timer.distanceUnixTime(rom.getBirthdayYear(), rom.getBirthdayMonth(), rom.getBirthdayDay(), deathYear);

  // Calculate the remaining time components
  int days = unix_distance / (60 * 60 * 24); // unix in seconds
  // int days     = floor(unix_distance / (1000 * 60 * 60 * 24));     // unix in microseconds
  // int hours    = (unix_distance / (60 * 60)) % 24;
  // int minutes  = (unix_distance / 60) % 60;
  // int seconds  = unix_distance % 60;

  return days;
}