/* AEON_ROM.h

*/

#ifndef AEON_ROM_h
#define AEON_ROM_h

#include <Arduino.h>

enum EReturn_ROM
{
  ROM_RETURN_NULL,             // No Return
  ERROR_EEPROM_NOT_VALID_DATA, // ROM doesn't store valid data! Write new signature.
  ERROR_EEPROM_COMMIT_FAILD,   // EEPROM commit failed
};

enum EBirthdayMonth
{
  January,
  February,
  March,
  April,
  May,
  June,
  July,
  August,
  September,
  October,
  November,
  December,
};

enum ESex {
  Woman,
  Man
};

class AEON_ROM
{
private:
  // EEPROM init signature
  // const int wrtnSig   = 0xC001D00D; // 0xC0EDBABE;
  const int wrtnSig = 10; // ROM signature
  const int stoAdd = 0;   // stored signature address
  int romSig;             // check signature at address 0

  const int ARRAY_SIZE = 8; // {init, birthdayYear, bithdayMonth, birthdayDay, sex, lifespan, (lastErrorState<-not)}
  const int EEPROM_ADDRESS = stoAdd + sizeof(wrtnSig);
  const int defaultLifespanWoman = 86;
  const int defaultLifespanMan = 79;

  bool init;
  int birthdayYear;
  int birthdayMonth;          // 1 -12
  int birthdayDay;            // 1- 31
  int sex;                    // 0 = Female; 1 = Male
  int lifespanWoman;          // ~ 86
  int lifespanMan;            // ~ 79

      /*
  * 00 = EEPROM_RETURN_NULL
  * 01 = EEPROM_NOT_VALID_DATA
  * 02 = EEPROM_COMMIT_FAILD
  * 10 = TIME_RETURN_NULL
  * 11 = TIME_NO_RTC
  * 12 = TIME_LOST_POWER
  * 20 = DISPLAY_RETURN_NULL
  * 21 = DISPLAY_ALLOCATION_FAILD 
  */

  EReturn_ROM lastErrorState;        

public:
  EReturn_ROM setupEEPROM();
  void saveToEEPROM();
  void getEEPROM();
  void setDefault(int year, int month, int day, int sex, int defaultLifetimeWoman, int defaultLifetimeMan);
  void resetEEPROM();

  void setInit(bool init);
  void setBirthdayYear(int value);
  void setBirthdayMonth(int value);
  void setBirthdayDay(int value);
  void setSex();
  void setLifespan(int value);
  void resetErrorStateRom();

  bool getInit();
  int getBirthdayYear();
  int getBirthdayMonth();
  int getBirthdayDay();
  long getBirthdayAsUnix();
  int getSex();
  int getDefaultLifespanWoman();
  int getDefaultLifespanMan();
  int getLifespan();
  EReturn_ROM getErrorState();

  bool writeIntArrayIntoEEPROM(int address, int numbers[], int arraySize);
  void readIntArrayFromEEPROM(int address, int numbers[], int arraySize);
};

#endif