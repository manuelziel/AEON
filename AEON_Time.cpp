/*

 AEON_Time.cpp

PIN btn_SET  = 22;
PIN btn_P    = 23;
PIN btn_N    = 24;
PIN btn_OK   = 25;

const byte BUTTON_PIN = 7;

*/

#include <Arduino.h>
#include <chrono>
#include "AEON_Time.h"
#include "RTClib.h"

RTC_DS3231 rtc;

/*
Time
*/
EReturn_TIME AEON_Time::setupTime()
{
  EReturn_TIME localReturn = EReturn_TIME::TIME_RETURN_NULL;

  Serial.println("Setup RTC");
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC!");
    localReturn = EReturn_TIME::ERROR_TIME_NO_RTC;
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, lets set the time!");
    // Year, Month, Day, Hour, Minute, Second
    rtc.adjust(DateTime(2000, 1, 1, 0, 0, 0));
    localReturn = EReturn_TIME::ERROR_TIME_LOST_POWER; 
  }
  else
  {
    Serial.println("RTC power, all OK!");
  }
  return localReturn;
}

/*

*/
void AEON_Time::loopTime()
{
}

/*

*/
void AEON_Time::updateTime()
{
  DateTime now = rtc.now();
  
  this->year = now.year();
  this->month = now.month();
  this->day = now.day();
  this->hour = now.hour();
  this->minute = now.minute();
  this->second = now.second();
  this->unix = now.unixtime();
}

/*

*/
// Year, Month, Day, Hour, Minute, Second, UnixTime
void AEON_Time::saveTime(int y, int mon, int d, int h, int mi, int sec, int u)
{
  this->year = y;
  this->month = mi;
  this->day = d; 
  this->hour = h;
  this->minute = mi;
  this->second = sec;
  this->unix = u;
}

/*

*/
void AEON_Time::setYear(int value)
{
  DateTime now = rtc.now();

  if (value > 0)
  {
    this->year++;
  }
  else
  {
    this->year--;
  }

  // Year, Month, Day, Hour, Minute, Second
  rtc.adjust(DateTime(this->year, now.month(), now.day(), now.hour(), now.minute(), now.second()));
}

/*

*/
void AEON_Time::setMonth(int value)
{

   // Input validation
  if ((value == 0) || (value < -11 || value > 11)) {
    return; // Do nothing if input is zero or if input is outside valid range
  }

  DateTime now = rtc.now();

  int newMonth = this->month + value;

  if (newMonth < 1) {
    newMonth += 12;
  } else if (newMonth > 12) {
    newMonth -= 12;
  }

  // Create new DateTime object with adjusted month
  rtc.adjust(DateTime(now.year(), newMonth, now.day(), now.hour(), now.minute(), now.second()));
  this->month = newMonth;
}

/*

*/
void AEON_Time::setDay(int value)
{
  DateTime now = rtc.now();

  struct tm time = {.tm_mday = 31, .tm_mon = now.month()-1, .tm_year = now.year() - 1900};
  mktime(&time);

  // last day >= current day
  if (((time.tm_mday == 31 ? 31 : 31 - time.tm_mday) <= now.day()) && value == 1)
  {
    this->day = 1;
  } 
  // add and remove day
  else if (/*remove*/((now.day() > 1) && (value == -1)) ||
  /*add*/(((time.tm_mday == 31 ? 31 : 31 - time.tm_mday/*last day*/) > now.day()) && value == 1))
  {
    this->day = now.day() + (value);
  }
  // set to last day
  else if ((now.day() ==  1) && value == -1)
  {
    this->day = time.tm_mday == 31 ? 31 : 31 - time.tm_mday;
  }

  // Create new DateTime object with adjusted day
  rtc.adjust(DateTime(now.year(), now.month(), this->day, now.hour(), now.minute(), now.second()));
}

/*

*/
void AEON_Time::setHour(int value)
{
  // Input validation
  if ((value == 0) || (value < -24 || value > 24)) {
    return; // Do nothing if input is zero or if input is outside valid range
  }

  DateTime now = rtc.now();
  int newHour = this->hour + value;

  if (newHour < 0) {
    newHour += 24;
  } else if (newHour > 23) {
    newHour -= 24;
  }

  // Create new DateTime object with adjusted hour
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), newHour, now.minute(), now.second()));
  this->hour = newHour;
}

/*

*/
void AEON_Time::setMinute(int value)
{
  // Input validation
  if ((value == 0) || (value < -60 || value > 60)) {
    return; // Do nothing if input is zero or if input is outside valid range
  }


  DateTime now = rtc.now();
  int newMinute = this->minute + value;

  if (newMinute < 0) {
    newMinute += 60;
  } else if (newMinute > 59) {
    newMinute -= 60;
  }

  // Create new DateTime object with adjusted minute
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), newMinute, now.second()));
  this->minute = newMinute;
}

/*

*/
void AEON_Time::setSecond(int value)
{
  // Input validation
  if ((value == 0) || (value < -60 || value > 60)) {
    return; // Do nothing if input is zero or if input is outside valid range
  }
  
  DateTime now = rtc.now();
  int newSecond = this->second + value;

  if (newSecond < 0) {
    newSecond += 60;
  } else if (newSecond > 59) {
    newSecond -= 60;
  }

  // Create new DateTime object with adjusted second
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), newSecond));
  this->second = newSecond;
}

/*
Reset the error to state return null
*/
void AEON_Time::resetErrorStateTime()
{
  EReturn_TIME lastErrorState = EReturn_TIME::TIME_RETURN_NULL;
}

/*

*/
int AEON_Time::getYear()
{
  return year;
}

/*

*/
int AEON_Time::getMonth()
{
  return month;
}

/*

*/
int AEON_Time::getDay()
{
  return day;
}

/*

*/
int AEON_Time::getHour()
{
  return hour;
}

/*

*/
int AEON_Time::getMinute()
{
  return minute;
}

/*

*/
int AEON_Time::getSecond()
{
  return second;
}

/*
Unixtime are saved in update.  
unix are saved in 
*/
int AEON_Time::getUnixTime()
{
  return unix;
}

/*
Calculate the distance in seconds between the current time and a specified Unix time.
 
Parameters:
b_year - birth year
b_month - birth month
b_day - birth day
time - Unix time to calculate the distance from
 
Returns:
The distance in seconds between the current time and the specified Unix time.
*/
double AEON_Time::distanceUnixTime(int b_year, int b_month, int b_day, int time)
{
  struct tm tm_end_span = {.tm_mday = b_day, .tm_mon = b_month -1, .tm_year = time - 1900};
  struct tm tm_birth    = {.tm_mday = b_day, .tm_mon = b_month -1, .tm_year = b_year - 1900};
  struct tm tm_now      = {.tm_mday = this->day, .tm_mon = this->month -1, .tm_year = this->year - 1900};
  
  double unix_end_span  = mktime(&tm_end_span);
  double unix_birth     = mktime(&tm_birth);
  double unix_now       = mktime(&tm_now);
  
  double unixDiff = (unix_birth + unix_end_span) - unix_now;

  return unixDiff;
}

/*

*/
EReturn_TIME AEON_Time::getErrorState()
{
  return this->lastErrorState;
}

/*

*/
DateTime AEON_Time::getTimeAsDateTime()
{
  DateTime now = rtc.now();
  return now;
}

/*

*/
String AEON_Time::getTimeAsString()
{
  DateTime now = rtc.now();
  // buffer can be defined using following combinations:
  // hh - the hour with a leading zero (00 to 23)
  // mm - the minute with a leading zero (00 to 59)
  // ss - the whole second with a leading zero where applicable (00 to 59)
  // YYYY - the year as four digit number
  // YY - the year as two digit number (00-99)
  // MM - the month as number with a leading zero (01-12)
  // MMM - the abbreviated English month name ('Jan' to 'Dec')
  // DD - the day as number with a leading zero (01 to 31)
  // DDD - the abbreviated English day name ('Mon' to 'Sun')

  char buf[] = "hh:mm:ss DDD, MMM DD YYYY";
  return now.toString(buf);
}