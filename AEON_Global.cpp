/* 
AEON_Global.cpp
*/

#include "AEON_Global.h"

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
int GLOBAL_DEFAULTS::defaultSex             = 0;
int GLOBAL_DEFAULTS::defaultLifespanWoman   = 86;
int GLOBAL_DEFAULTS::defaultLifespanMan     = 79;

const char *monthOfYearMapping[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
const char *dayOfWeekMapping[] = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };