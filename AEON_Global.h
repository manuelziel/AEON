/*
AEON_Global.h
*/

#ifndef AEON_GLOBAL_h
#define AEON_GLOBAL_h

#include <map>
#include <Arduino.h>
#include "AEON_Enums.h"

struct GLOBAL_DEFAULTS
{
    static int defaultInit;
    static int defaultYear;
    static int defaultMonth;
    static int defaultDay;

    static int defaultHour;
    static int defaultMinute;
    static int defaultSecond;

    static int defaultBirthdayYear;
    static int defaultBirthdayMonth;
    static int defaultBirthdayDay;
    static ESex defaultSex;
    static std::map<ELanguage, int> defaultLifespanFemale;
    static std::map<ELanguage, int> defaultLifespanMale;
    static ELanguage defaultLanguage;
};

#endif