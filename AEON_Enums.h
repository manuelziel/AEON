// enums.h

#ifndef ENUMS_H
#define ENUMS_H

enum EPressed
{
    SHORT,
    LONG,
    NO_CHANGE
};

enum ERotation
{
  degrees_0,
  degrees_90,
  degrees_180,
  degrees_270,
};

enum ETextSize
{
  TEXT_NULL,
  SMALL,
  MIDDLE,
  LARGE,
};

enum EState
{
  STATE_Base,                     // Base
  STATE_Setup_Time,               // Setup Time
  STATE_Setup_Time_Hour,          // Setup Time Hour
  STATE_Setup_Time_Minute,        // Setup Time Minute
  STATE_Setup_Time_Second,        // Setup Time Seconde
  STATE_Setup_Date,               // Setup Date
  STATE_Setup_Date_Year,          // Setup Date Year
  STATE_Setup_Date_Month,         // Setup Date Month
  STATE_Setup_Date_Day,           // Setup Date Day
  STATE_Setup_Birthday,           // Setup Birthday
  STATE_Setup_Birthday_Year,      // Setup Birthday Year
  STATE_Setup_Birthday_Month,     // Setup Birthday Month
  STATE_Setup_Birthday_Day,       // Setup Birthday Day
  STATE_Setup_Sex,                // Setup Sex
  STATE_Setup_Sex_Set,            // Setup Sex Set
  STATE_Setup_Lifespan,           // Setup Lifespan
  STATE_Setup_Lifespan_Set,       // Setup Lifespan Set
  STATE_Setup_Language,           // Setup Language
  STATE_Setup_Language_Set,       // Setup Language Set
  STATE_Setup_Reset,              // Setup Reset
  STATE_Setup_Reset_Yes,          // Setup Reset Yes
  STATE_Setup_Reset_No,           // Setup Reset No
  STATE_Setup_Reset_Count,        // Setup Reset Countdown
  STATE_Setup_Back,               // Setup Back
  STATE_ERROR                     // Show Errors
};

enum EEvent
{
  EVENT_SET, // Press Button SET
  EVENT_P,   // Press Button P
  EVENT_N,   // Press Button N
  EVENT_OK   // Press Button OK
};

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
  December
};

enum ESex {
  Female,
  Male
};

enum ELanguage {
  English,
  German,
  French,
  Spain,
  Count // To count all Languages
};

enum EReturn_DISPLAY
{
  DISPLAY_RETURN_NULL,           
  ERROR_DISPLAY_ALLOCATION_FAILD
};

enum EReturn_ROM
{
  ROM_RETURN_NULL,             // No Return
  ERROR_EEPROM_NOT_VALID_DATA, // ROM doesn't store valid data! Write new signature.
  ERROR_EEPROM_COMMIT_FAILD,   // EEPROM commit failed
};

enum EReturn_TIME
{
    TIME_RETURN_NULL,     // No Return
    ERROR_TIME_NO_RTC,    // Couldn't find RTC!
    ERROR_TIME_LOST_POWER // RTC lost power
};

#endif
