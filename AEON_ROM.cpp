/*
AEON_ROM.cpp
*/

#include <Arduino.h>
#include <EEPROM.h>
#include <time.h>
#include "AEON_Enums.h"
#include "AEON_Global.h"
#include "AEON_ROM.h"

/*
ROM
*/
EReturn_ROM AEON_ROM::setupEEPROM()
{
  // Initialize return status to null
  EReturn_ROM localReturn = EReturn_ROM::ROM_RETURN_NULL;

  // Initialize EEPROM
  Serial.println("Setup EEPROM");
  EEPROM.begin(4096);

  // Check init signature
  romSig = EEPROM.read(stoAdd);

  if (romSig == wrtnSig)
  {
    // EEPROM already initialized
    Serial.print("ROM signature OK. ROM length: ");
    Serial.println(EEPROM.length());
    Serial.println();
    getEEPROM();
  }
  else
  {
    // EEPROM not initialized
    Serial.println("ROM doesn't store valid data! Write new signature.");
    EEPROM.write(stoAdd, wrtnSig); // EEPROM need 3,3 millisec to save!
    resetEEPROM();
    localReturn = EReturn_ROM::ERROR_EEPROM_NOT_VALID_DATA;

    // Commit ROM
    if (!EEPROM.commit())
    {
      Serial.println("ERROR! EEPROM commit failed");
      localReturn = EReturn_ROM::ERROR_EEPROM_COMMIT_FAILD;
    }
    else
    {
      Serial.println("EEPROM successfully committed");
    }
  }
  return localReturn;
}

/*
This function is responsible for writing data to an EEPROM.
*/
void AEON_ROM::saveToEEPROM()
{
  EReturn_ROM localReturn = EReturn_ROM::ROM_RETURN_NULL;

  // Check if initialization values need to be set to true for the first time.
  // If all birthday fields are zero, then do not set initialization flag to true.
  this->init = true;

  // Create an array to hold the values to be written to EEPROM.
  int contentArray[ARRAY_SIZE] = {
      this->init,
      this->birthdayYear,
      this->birthdayMonth,
      this->birthdayDay,
      this->sex,
      this->lifespanFemale,
      this->lifespanMale,
      this->language,
  };

  // Write the array of values to EEPROM at the specified address.
  if (!writeIntArrayIntoEEPROM(EEPROM_ADDRESS, contentArray, ARRAY_SIZE))
  {
    // Define an error code for EEPROM write failure.
    localReturn = EReturn_ROM::ERROR_EEPROM_COMMIT_FAILD;
  }
}

/*
This method reads data from the EEPROM and updates the object properties.
It also prints the loaded data to the Serial Monitor.
*/
void AEON_ROM::getEEPROM()
{
  Serial.println("Loading data from EEPROM... \n");

  int arrayContent[ARRAY_SIZE];
  // The content of the EEPROM is an array with the following structure:
  // {init, birthdayYear, birthdayMonth, birthdayDay, sex, lifespanWoman, lifespanMan}
  readIntArrayFromEEPROM(EEPROM_ADDRESS, arrayContent, ARRAY_SIZE);

  // Update the object properties with the loaded data
  this->init = arrayContent[0];
  if (this->init)
  {
    this->birthdayYear = arrayContent[1];
    this->birthdayMonth = arrayContent[2];
    this->birthdayDay = arrayContent[3];
    this->sex = static_cast<ESex>(arrayContent[4]);
    this->lifespanFemale = arrayContent[5];
    this->lifespanMale = arrayContent[6];
    this->language = static_cast<ELanguage>(arrayContent[7]);
  }

  // Print the loaded data to the Serial Monitor
  Serial.printf("Init: %d, Birthday Year: %d, Birthday Month: %d, Birthday Day: %d, Sex: %d, Lifespan Female: %d, Lifespan Male: %d, Language: %d",
                this->init,
                this->birthdayYear,
                this->birthdayMonth,
                this->birthdayDay,
                this->sex,
                this->lifespanFemale,
                this->lifespanMale,
                this->language);

  Serial.println();
}

/*
Set defaults for RAM and save to ROM
*/
void AEON_ROM::resetEEPROM()
{
  this->birthdayYear = GLOBAL_DEFAULTS::defaultBirthdayYear;
  this->birthdayMonth = GLOBAL_DEFAULTS::defaultBirthdayMonth;
  this->birthdayDay = GLOBAL_DEFAULTS::defaultBirthdayDay;
  this->sex = GLOBAL_DEFAULTS::defaultSex;
  this->lifespanFemale = GLOBAL_DEFAULTS::defaultLifespanFemale[this->language];
  this->lifespanMale = GLOBAL_DEFAULTS::defaultLifespanMale[this->language];
  this->language = GLOBAL_DEFAULTS::defaultLanguage;
  Serial.println("Set Defaults and reset EEPROM");
  saveToEEPROM();
}

/*
Get the Init.
*/
bool AEON_ROM::getInit()
{
  return this->init;
}

/*
Set Init.
*/
void AEON_ROM::setInit(bool value)
{
  this->init = value;
}

/*
Get the Birthday Year
*/
int AEON_ROM::getBirthdayYear()
{
  return this->birthdayYear;
}

/*
Set Birthday Year
*/
void AEON_ROM::setBirthdayYear(int value)
{
  if (value > 0)
  {
    this->birthdayYear++;
  }
  else
  {
    this->birthdayYear--;
  }
}

/*
Get the birthday month
*/
int AEON_ROM::getBirthdayMonth()
{
  return this->birthdayMonth;
}

/*
Set new birthday month
Increment or decrement the birthday month by one.
*/
void AEON_ROM::setBirthdayMonth(int value)
{
  // Check if the month is December and the increment value is positive, then set it to January.
  if ((this->birthdayMonth == EMonth::December) && (value > 0))
  {
    this->birthdayMonth = EMonth::January;
  }
  // Check if the month is January and the increment value is negative, then set it to December.
  else if ((this->birthdayMonth == EMonth::January) && (value < 0))
  {
    this->birthdayMonth = EMonth::December;
  }
  // Increment or decrement the month based on the input value
  else
  {
    this->birthdayMonth += value;
  }
}

/*
Get the birthday day
*/
int AEON_ROM::getBirthdayDay()
{
  return this->birthdayDay;
}

/*
Set the day of the month for the birthday.
*/
void AEON_ROM::setBirthdayDay(int value)
{
  // Calculate the last day of the current month
  struct tm b_day = {.tm_mday = 31, .tm_mon = this->birthdayMonth, .tm_year = this->birthdayYear - 1900};
  mktime(&b_day);

  // Determine the last day of the current month
  int lastDayOfMonth = b_day.tm_mday == 31 ? 31 : 31 - b_day.tm_mday;

  // If value is 1 and the current day is the last day of the month, set the day to 1
  if (value == 1 && this->birthdayDay == lastDayOfMonth)
  {
    this->birthdayDay = 1;
  }

  // If value is -1 and the current day is the first day of the month, set the day to the last day of the previous month
  else if (value == -1 && this->birthdayDay == 1)
  {
    this->birthdayDay = lastDayOfMonth;
  }

  // If value is 1 and the current day is not the last day of the month, increment the day
  else if (value == 1 && this->birthdayDay < lastDayOfMonth)
  {
    this->birthdayDay++;
  }

  // If value is -1 and the current day is not the first day of the month, decrement the day
  else if (value == -1 && this->birthdayDay > 1)
  {
    this->birthdayDay--;
  }
}

/*
Get the sex
*/
ESex AEON_ROM::getSex()
{
  return this->sex;
}

/*
Switch the sex.
*/
void AEON_ROM::switchSex()
{
  if (this->sex == ESex::Female)
  {
    this->sex = ESex::Male;
  }
  else
  {
    this->sex = ESex::Female;
  }
}

/*
Get Lifespan
*/
int AEON_ROM::getLifespan()
{
  int value;

  if (this->sex == ESex::Female)
  {
    value = this->lifespanFemale;
  }
  else if (this->sex == ESex::Male)
  {
    value = this->lifespanMale;
  }
  return value;
}

/*
Sets the lifespan value based on the provided value and the person's sex.
value: the value by which the lifespan should be adjusted (+1 for increase, -1 for decrease)
*/
void AEON_ROM::setLifespan(int value)
{
  if (value > 0)
  {
    // If the person is female, increment the lifespan of female.
    if (this->sex == ESex::Female)
    {
      this->lifespanFemale++;
    }

    // If the person is male, increment the lifespan of male.
    else if (this->sex == ESex::Male)
    {
      this->lifespanMale++;
    }
  }
  else if (value < 0)
  {
    // If the person is female, decrement the lifespan of female.
    if (this->sex == ESex::Female)
    {
      this->lifespanFemale--;
    }
    // If the person is male, decrement the lifespan of male.
    else if (this->sex == ESex::Male)
    {
      this->lifespanMale--;
    }
  }
}

/*
Update the Lifespan with defaults
*/
void AEON_ROM::updateDefaultLifespan()
{
  this->lifespanFemale = GLOBAL_DEFAULTS::defaultLifespanFemale[this->language];
  this->lifespanMale = GLOBAL_DEFAULTS::defaultLifespanMale[this->language];
}

/*
Get Language
*/
ELanguage AEON_ROM::getLanguage()
{
  return this->language;
}

/*
Sets the language
*/
void AEON_ROM::setLanguage(int value)
{
  // int numLanguages = std::size(ELanguage{});            // c++17
  int numLanguages = static_cast<int>(ELanguage::Count); //
  int currentLanguageIndex = static_cast<int>(this->language);

  // Next Language
  if (value > 0)
  {
    currentLanguageIndex = (currentLanguageIndex + 1) % numLanguages;
  }
  // Previous Language
  else if (value < 0)
  {
    currentLanguageIndex = (currentLanguageIndex - 1 + numLanguages) % numLanguages;
  }
  // Invalid value - keep language
  else
  {
    return;
  }

  language = static_cast<ELanguage>(currentLanguageIndex);
  this->language = language;

  // Update default lifespan based on language
  updateDefaultLifespan();
}

/*
Reset the error to state return null
*/
void AEON_ROM::resetErrorStateRom()
{
  EReturn_ROM lastErrorState = EReturn_ROM::ROM_RETURN_NULL;
}

/*
Get the Birthday as Unix
*/
long AEON_ROM::getBirthdayAsUnix()
{
  struct tm b_unix = {.tm_mday = this->birthdayDay, .tm_mon = this->birthdayMonth - 1, .tm_year = this->birthdayYear - 1900};
  time_t unixTime = mktime(&b_unix);
  return unixTime;
}

/*
Get Error State
*/
EReturn_ROM AEON_ROM::getErrorState()
{
  return this->lastErrorState;
}

/*
Write Intager into EEPROM
*/
bool AEON_ROM::writeIntArrayIntoEEPROM(int address, int numbers[], int arraySize)
{
  bool return_value = true;
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    EEPROM.write(addressIndex, numbers[i] >> 8);
    EEPROM.write(addressIndex + 1, numbers[i] & 0xFF);
    addressIndex += 2;

    if (!EEPROM.commit())
    {
      Serial.println("ERROR! EEPROM commit failed");
      return_value = false;
    }
  }
  Serial.printf("\n Save to EEPROM \n");
  return return_value;
}

/*
Read Intager Array from EEPROM
*/
void AEON_ROM::readIntArrayFromEEPROM(int address, int numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    numbers[i] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
    addressIndex += 2;
  }
}