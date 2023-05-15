/* 
AEON_Time.h
*/

#ifndef AEON_TIME_h
#define AEON_TIME_h

#include <Arduino.h>
#include <RTClib.h>
#include "AEON_Global.h"
#include "AEON_Enums.h"

class AEON_Time
{
private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int unix;

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

  EReturn_TIME lastErrorState;  

public:
    EReturn_TIME setupTime();
    void updateTime();

    void saveTime(int year, int month, int day, int hour, int minute, int second, int unixTime);

    void setYear(int year);
    void setMonth(int month);
    void setDay(int day);
    void setHour(int hour);
    void setMinute(int minute);
    void setSecond(int second);
    void resetErrorStateTime();

    int getYear();
    int getMonth();
    int getDay();
    int getHour();
    int getMinute();
    int getSecond();
    int getUnixTime();
    double distanceUnixTime(int b_year, int b_month, int b_day, int time);
    EReturn_TIME getErrorState();

    DateTime getTimeAsDateTime();
    String getTimeAsString();
};

#endif