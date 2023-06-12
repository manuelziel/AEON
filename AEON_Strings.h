/*
AEON_Strings.h
*/

#ifndef AEON_STRINGS_h
#define AEON_STRINGS_h

#include <Arduino.h>
#include "AEON_Global.h"
#include "AEON_Enums.h"

class AEON_Strings {
private:
  static const char* dayOfWeek[][4];
  static const char* monthOfYear[][4];
  static const char* text[][4];

public:
  enum class EStrings {
    Error,
    Ok,
    RemainingDays,
    Setup,
    SetupTime,
    Time,
    SetupDate,
    Date,
    SetupBirthday,
    Birthday,
    SetupSex,
    Sex,
    Female,
    Male,
    SetupLifespan,
    Lifespan,
    SetupLanguage,
    Language,
    English,
    German,
    French,
    Spain,
    SetupReset,
    Reset,
    YES,
    NO,
    SetupBack
  };

  const char* getString(EStrings string);
  const char* getWeekday(int weekday);
  const char* getMonth(int month);  
    
};

#endif