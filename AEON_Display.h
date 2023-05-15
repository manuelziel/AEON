/* 
AEON_Display.h - The class represents a display driver and includes functions to setup and control the display. 
It also contains various page-specific functions. The class has a private enumeration for sex and private member variables to keep track of text size, 
cursor position, last printed integer, last printed string, and last error state. The last error state is used to keep track of errors encountered by the display driver. 
The public enumeration includes various pages of the display such as year, month, day, hour, minute, second, birthday year, birthday month, birthday day, reset yes, and reset no. 
The public functions include setting up and looping the display, clearing the display, setting the display, setting the text size, setting the cursor, 
printing an integer or string, drawing a pixel, resetting the error state, and various page-specific functions. The display driver returns an error state 
to the caller if an error occurs during execution.
*/

#ifndef AEON_DISPLAY_h
#define AEON_DISPLAY_h

#include <Arduino.h>
#include "AEON_Enums.h"

class AEON_Display
{
private:

  int textSize;
  int y_Cursor;
  int x_Cursor;
  int printI;
  String printStr;

  /*
  00 = EEPROM_RETURN_NULL
  01 = EEPROM_NOT_VALID_DATA
  02 = EEPROM_COMMIT_FAILD
  10 = TIME_RETURN_NULL
  11 = TIME_NO_RTC
  12 = TIME_LOST_POWER
  20 = DISPLAY_RETURN_NULL
  21 = DISPLAY_ALLOCATION_FAILD 
  */

 EReturn_DISPLAY lastErrorState; 

public:

  EReturn_DISPLAY setupDisplay();
  void loopDisplay();
  void clearDisplay();
  void setDisplay();
  
  void setTextSize(int i);
  void setCurs(int y, int x);
  void printInt(int i);
  void printString(String s);
  void drawPixel(int x, int y);
  void resetErrorStateDisplay();

  // Pages
  void pageBase(int year, int month, int day, int dayOfTheWeek, int hour, int minute, int second, int lifetime);
  void pageSetupTime();
  void pageSetupTime_set_time(EState state, int hour, int minute, int second);
  void pageSetupDate();
  void pageSetupDate_set_date(EState state, int year, int month, int day);
  void pageSetupBirthday();
  void pageSetupBirthday_set_date(EState state, int year, int month, int day);
  void pageSetupSex();
  void pageSetupSex_set(ESex sex);
  void pageSetupLifespan();
  void pageSetupLifespan_set(int lifespan);
  void pageSetupLanguage();
  void pageSetupLanguage_set(ELanguage language);
  void pageSetupReset();
  void pageSetupReset_set(EState state);
  void pageSetupReset_count_final(int cnt_reset);
  void pageSetupBack();
  void pageERROR(char *errorText);

  EReturn_DISPLAY getErrorState();
};

#endif
