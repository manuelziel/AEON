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

class AEON_ROM
{
private:
  // EEPROM init signature
  // const int wrtnSig   = 0xC001D00D; // 0xC0EDBABE;
  const int wrtnSig = 10; // ROM signature
  const int stoAdd = 0;   // stored signature address
  int romSig;             // check signature at address 0

  const int EEPROM_ADDRESS = stoAdd + sizeof(wrtnSig);
  const int ARRAY_SIZE = 8; 

  // ARRAY 
  bool init;
  int birthdayYear;
  int birthdayMonth;
  int birthdayDay;
  int sex;
  int lifespanWoman;
  int lifespanMan;

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
  void resetEEPROM();

  void setInit(bool init);
  void setBirthdayYear(int value);
  void setBirthdayMonth(int value);
  void setBirthdayDay(int value);
  void setSex();
  void setLifespan(int value);
  //void setLanguage(ELanguage language);
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