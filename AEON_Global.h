/*
AEON_Global.h
*/

#ifndef AEON_GLOBAL_h
#define AEON_GLOBAL_h

#include <Arduino.h>

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
    static int defaultSex;
    static int defaultLifespanWoman;
    static int defaultLifespanMan;
};

enum EStates
{
  STATE_Base,                     // Base
  STATE_Setup_Time,               // Setup Time
  STATE_Setup_Time_Set_Hour,      // Setup Time Hour
  STATE_Setup_Time_Set_Minute,    // Setup Time Minute
  STATE_Setup_Time_Set_Second,    // Setup Time Seconde
  STATE_Setup_Date,               // Setup Date
  STATE_Setup_Date_Set_Year,      // Setup Date Year
  STATE_Setup_Date_Set_Month,     // Setup Date Month
  STATE_Setup_Date_Set_Day,       // Setup Date Day
  STATE_Setup_Birthday,           // Setup Birthday
  STATE_Setup_Birthday_Set_Year,  // Setup Birthday Year
  STATE_Setup_Birthday_Set_Month, // Setup Birthday Month
  STATE_Setup_Birthday_Set_Day,   // Setup Birthday Day
  STATE_Setup_Sex,                // Setup Sex
  STATE_Setup_Sex_Set,            // Setup Sex Set
  STATE_Setup_Lifespan,           // Setup Lifespan
  STATE_Setup_Lifespan_Set,       // Setup Lifespan Set
  // STATE_Setup_Language?
  STATE_Setup_Reset,       // Setup Reset
  STATE_Setup_Reset_Yes,   // Setup Reset Yes
  STATE_Setup_Reset_No,    // Setup Reset No
  STATE_Setup_Reset_Count, // Setup Reset Countdown
  STATE_Setup_Back,        // Setup Back
  STATE_ERROR              // Show Errors
};

enum EEvents
{
  EVENT_SET, // Press Button SET
  EVENT_P,   // Press Button P
  EVENT_N,   // Press Button N
  EVENT_OK   // Press Button OK
};

extern const char *monthOfYearMapping[];
extern const char *dayOfWeekMapping[];

enum EMonth
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

enum ELanguage {
  English,
  German
};

#endif