/* 
AEON_Global.cpp

Lifespan based on The World Factbook - https://www.cia.gov/the-world-factbook/
*/

#include <map>
#include "AEON_Global.h"
#include "AEON_Enums.h"

int GLOBAL_DEFAULTS::defaultInit    = 1;

int GLOBAL_DEFAULTS::defaultYear    = 2000;
int GLOBAL_DEFAULTS::defaultMonth   = 1;  
int GLOBAL_DEFAULTS::defaultDay     = 1;

int GLOBAL_DEFAULTS::defaultHour    = 0;
int GLOBAL_DEFAULTS::defaultMinute  = 0;
int GLOBAL_DEFAULTS::defaultSecond  = 0;

int GLOBAL_DEFAULTS::defaultBirthdayYear    = 2000;
int GLOBAL_DEFAULTS::defaultBirthdayMonth   = 0;
int GLOBAL_DEFAULTS::defaultBirthdayDay     = 1;
ESex GLOBAL_DEFAULTS::defaultSex            = ESex::Female;
ELanguage GLOBAL_DEFAULTS::defaultLanguage  = ELanguage::English;

// Initialising the default Lifespan Female
std::map<ELanguage, int> GLOBAL_DEFAULTS::defaultLifespanFemale = {
    {ELanguage::English, 82}, // Europe Union
    {ELanguage::German, 83},  // Germany
    {ELanguage::French, 85},  // French Worldwide
    {ELanguage::Spain, 85},   // Spain Worldwide
};

// Initialising the default Lifespan Male
std::map<ELanguage, int> GLOBAL_DEFAULTS::defaultLifespanMale = {
    {ELanguage::English, 77}, // Europe Union
    {ELanguage::German, 78},  // Germany
    {ELanguage::French, 79},  // French Worldwide
    {ELanguage::Spain, 79},   // Spain Worldwide
};