/* 
AEON_Display.cpp - This represents a display driver and includes functions to setup and control the display. 
It also contains various page-specific functions. The class has a private enumeration for sex and private member variables to keep track of text size,
cursor position, last printed integer, last printed string, and last error state. The last error state is used to keep track of errors encountered by
the display driver. The public enumeration includes various pages of the display such as year, month, day, hour, minute, second, birthday year,
birthday month, birthday day, reset yes, and reset no. The public functions include setting up and looping the display, clearing the display,
setting the display, setting the text size, setting the cursor, printing an integer or string, drawing a pixel, resetting the error state, 
and various page-specific functions. The display driver returns an error state to the caller if an error occurs during execution.
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "AEON_Display.h"
#include "AEON_Time.h"

extern AEON_Time timer;

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define OLED_RESET 3         // Reset pin # default 3 (old_18)
#define SCREEN_ADDRESS 0x3C  // See Datasheet for Address; 0x78 or 0x3C for 128x64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum ERotation {
  degrees_0,
  degrees_90,
  degrees_180,
  degrees_270,
};

enum ETextSize {
  TEXT_NULL,
  SMALL,
  MIDDLE,
  LARGE,
};

/*
 Display
*/
EReturn_DISPLAY AEON_Display::setupDisplay() {
  EReturn_DISPLAY localReturn = EReturn_DISPLAY::DISPLAY_RETURN_NULL;

  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  Serial.println("Setup Display");
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    // for (;;)
    //     ; // Don't proceed, loop forever
    localReturn = EReturn_DISPLAY::ERROR_DISPLAY_ALLOCATION_FAILD;
  }

  // Show initial display buffer contents on the screen
  // the library initializes this with an splash screen.
  display.display();
  delay(1);                
  display.clearDisplay();  
  display.setRotation(degrees_0);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();

  // Set Logo
  display.clearDisplay();
  display.setTextSize(LARGE);
  display.setTextColor(SSD1306_WHITE);
  display.getTextBounds("AEON", 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 3);
  display.println(F("AEON"));

  display.setTextSize(SMALL);
  display.setCursor(44, 56);
  display.println(F("by Manuel Ziel"));
  display.display();
  delay(2000);

  return localReturn;
}

/*

*/
void AEON_Display::loopDisplay() {}

/*

*/
void AEON_Display::clearDisplay() {
  display.clearDisplay();
}

/*

*/
void AEON_Display::setDisplay() {
  display.display();
}

/*

*/
void AEON_Display::setTextSize(int i) {
  display.setTextSize(i);  // Normal 1:1 pixel scale (1)
}

/*

*/
void AEON_Display::setCurs(int x, int y) {
  display.setCursor(x, y);  // Start at top-left corner
}

/*

*/
void AEON_Display::printInt(int i) {
  display.println(F(i));
}

/*

*/
void AEON_Display::printString(String s) {
  display.println(s);
}

/*

*/
void AEON_Display::drawPixel(int y, int x) {
  display.drawPixel(y, x, SSD1306_WHITE);
}

/*
Reset the error to state return null
*/
void AEON_Display::resetErrorStateDisplay() {
  EReturn_DISPLAY lastErrorState = EReturn_DISPLAY::DISPLAY_RETURN_NULL;
}

/*
Show the base page with Date, Time and remaining days.
*/
void AEON_Display::pageBase(int year, int month, int day, int dayOfTheWeek, int hour, int minute, int second, int lifetime) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.clearDisplay();
  display.setTextSize(SMALL);
  display.setTextColor(SSD1306_WHITE);

  // First line
  char bufFirstLine[20];
  snprintf(bufFirstLine, sizeof(bufFirstLine), "%s, %s %02d %4d",
           dayOfWeekMapping[dayOfTheWeek],
           monthOfYearMapping[month],
           day, year);
  display.setCursor(0, 0);
  display.println(bufFirstLine);

  // Second line
  char bufSecondLine[9];
  snprintf(bufSecondLine, sizeof(bufSecondLine), "%02d:%02d:%02d",
           hour, minute, second);
  display.setCursor(0, 10);
  display.println(bufSecondLine);

  // Third line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Fourth line
  const char *remainingDaysText = "Remaining Days";
  int remainingDaysTextLen = strlen(remainingDaysText);
  int remainingDaysTextXPos = (SCREEN_WIDTH - (remainingDaysTextLen * 6)) / 2;
  display.setCursor(remainingDaysTextXPos, 25);
  display.println(remainingDaysText);

  // Last line
  display.setTextSize(LARGE);

  char bufLifetime[8];
  sprintf(bufLifetime, "%02d:", lifetime);

  int value;
  if (lifetime > 0) {
    sprintf(bufLifetime, "%02d", lifetime);
  } else {
    value = abs(lifetime);  // remove minus
    sprintf(bufLifetime, "+%02d", value);
  }

  display.getTextBounds(bufLifetime, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 40);
  display.println(bufLifetime);

  display.display();
}

/*
This function displays a setup page on the display, allowing the user to set AEON.
The page consists of a title and a subtitle, separated by a horizontal line. The title
is centered at the top of the display and reads "Setup". The subtitle is centered in
the middle of the display and reads "Setup Time".
*/
void AEON_Display::pageSetupTime() {
  // Variables
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  // Clear display and set text color
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  const char *setupText = "Setup";
  display.setTextSize(MIDDLE);
  display.getTextBounds(setupText, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(setupText);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  const char *setupTime = "Setup Time";
  display.setTextSize(SMALL);
  display.getTextBounds(setupTime, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 40);
  display.println(setupTime);

  display.display();
}

/*
Sets the time for the page setup display. The display shows the current time with customizable boundary sizes
for hours, minutes, and seconds. The input values for the current time are passed as parameters. The parameter "p"
determines which boundary size to customize. The boundaries are defined by buffer combinations. This function clears the display,
sets the text color to white, and then prints the time and boundary sizes in the center of the screen.
*/
void AEON_Display::pageSetupTime_set_time(EPage p, int hour, int minute, int second) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  const char *time = "Time";
  display.setTextSize(MIDDLE);
  display.getTextBounds(time, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(time);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  uint16_t width_first_boundary;
  uint16_t height_first_boundary;
  int16_t x1_first_bundary;
  int16_t y1_first_bundary;

  uint16_t width_second_boundary;
  uint16_t height_second_boundary;
  int16_t x1_secound_bundary;
  int16_t y1_secound_bundary;

  uint16_t width_third_boundary;
  uint16_t height_third_boundary;
  int16_t x1_third_boundary;
  int16_t y1_third_boundary;

  // Set the boundary size of hour, minute or second
  int boundarySize[] = { SMALL, SMALL, SMALL };
  switch (p) {
    case Hour:
      ++boundarySize[0];
      break;

    case Minute:
      ++boundarySize[1];
      break;

    case Second:
      ++boundarySize[2];
      break;

    default:
      break;
  }

  char bufFirstBoundary[8];
  char bufSecondBoundary[8];
  char bufThirdBoundary[8];

  if (boundarySize[1] == MIDDLE) {
    sprintf(bufFirstBoundary, "%02d:",
            hour);

    sprintf(bufSecondBoundary, "%02d",
            minute);

    sprintf(bufThirdBoundary, ":%02d",
            second);
  } else {
    sprintf(bufFirstBoundary, "%02d",
            hour);

    sprintf(bufSecondBoundary, ":%02d:",
            minute);

    sprintf(bufThirdBoundary, "%02d",
            second);
  }

  // Calc width of the bounds
  display.setTextSize(boundarySize[0]);
  display.getTextBounds(bufFirstBoundary, 0, 0, &x1_first_bundary, &y1_first_bundary, &width_first_boundary, &height_first_boundary);

  display.setTextSize(boundarySize[1]);
  display.getTextBounds(bufSecondBoundary, 0, 0, &x1_secound_bundary, &y1_secound_bundary, &width_second_boundary, &height_second_boundary);

  display.setTextSize(boundarySize[2]);
  display.getTextBounds(bufThirdBoundary, 0, 0, &x1_third_boundary, &y1_third_boundary, &width_third_boundary, &height_third_boundary);

  uint16_t complBoundarysWidth = (width_first_boundary + (width_second_boundary + width_third_boundary));
  uint16_t spaceBoundarysToScreen = (SCREEN_WIDTH - complBoundarysWidth) / 2;

  // First boundary
  display.setTextSize(boundarySize[0]);
  display.setCursor(spaceBoundarysToScreen, 40);
  display.print(bufFirstBoundary);

  // Second boundary
  display.setTextSize(boundarySize[1]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary, 40);
  display.print(bufSecondBoundary);

  // Third boundary
  display.setTextSize(boundarySize[2]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary + width_second_boundary, 40);
  display.print(bufThirdBoundary);

  display.display();
}

/*
This function displays a setup page on the display, allowing the user to set AEON.
The page consists of a title and a subtitle, separated by a horizontal line. The title
is centered at the top of the display and reads "Setup". The subtitle is centered in
the middle of the display and reads "Setup Date".
*/
void AEON_Display::pageSetupDate() {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  const char *setup = "Setup";
  display.setTextSize(MIDDLE);
  display.getTextBounds(setup, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(setup);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  const char *setupDate = "Setup Date";
  display.setTextSize(SMALL);
  display.getTextBounds(setupDate, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 40);
  display.println(setupDate);

  display.display();
}

/*
Sets the Date for the page setup display. The display shows the current Date with customizable boundary sizes
for year, month, and days. The input values for the current Date are passed as parameters. The parameter "p"
determines which boundary size to customize. The boundaries are defined by buffer combinations. This function clears the display,
sets the text color to white, and then prints the Date and boundary sizes in the center of the screen.
*/
void AEON_Display::pageSetupDate_set_date(EPage p, int year, int month, int day) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  const char *date = "Date";
  display.setTextSize(MIDDLE);
  display.getTextBounds(date, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(date);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  uint16_t width_first_boundary;
  uint16_t height_first_boundary;
  int16_t x1_first_boundary;
  int16_t y1_first_boundary;

  uint16_t width_second_boundary;
  uint16_t height_second_boundary;
  int16_t x1_second_boundary;
  int16_t y1_second_boundary;

  uint16_t width_third_boundary;
  uint16_t height_third_boundary;
  int16_t x1_third_boundary;
  int16_t y1_third_boundary;

  // Set the boundary size of hour, minute or second
  int boundarySize[] = { SMALL, SMALL, SMALL };
  switch (p) {
    case Year:
      ++boundarySize[0];
      break;

    case Month:
      ++boundarySize[1];
      break;

    case Day:
      ++boundarySize[2];
      break;

    default:
      break;
  }

  char bufFirstBoundary[8];
  char bufSecondBoundary[8];
  char bufThirdBoundary[8];

  if (boundarySize[1] == MIDDLE) {
    sprintf(bufFirstBoundary, "%02d:",
            year);

    sprintf(bufSecondBoundary, "%s",
            monthOfYearMapping[month]);

    sprintf(bufThirdBoundary, ":%02d",
            day);
  } else {
    sprintf(bufFirstBoundary, "%02d",
            year);

    sprintf(bufSecondBoundary, ":%s:",
            monthOfYearMapping[month]);

    sprintf(bufThirdBoundary, "%02d",
            day);
  }

  // Calc width of the bounds
  display.setTextSize(boundarySize[0]);
  display.getTextBounds(bufFirstBoundary, 0, 0, &x1_first_boundary, &y1_first_boundary, &width_first_boundary, &height_first_boundary);

  display.setTextSize(boundarySize[1]);
  display.getTextBounds(bufSecondBoundary, 0, 0, &x1_second_boundary, &y1_second_boundary, &width_second_boundary, &height_second_boundary);

  display.setTextSize(boundarySize[2]);
  display.getTextBounds(bufThirdBoundary, 0, 0, &x1_third_boundary, &y1_third_boundary, &width_third_boundary, &height_third_boundary);

  uint16_t complBoundarysWidth = (width_first_boundary + (width_second_boundary + width_third_boundary));
  uint16_t spaceBoundarysToScreen = (SCREEN_WIDTH - complBoundarysWidth) / 2;

  // First boundary
  display.setTextSize(boundarySize[0]);
  display.setCursor(spaceBoundarysToScreen, 40);
  display.print(bufFirstBoundary);

  // Second boundary
  display.setTextSize(boundarySize[1]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary, 40);
  display.print(bufSecondBoundary);

  // Third boundary
  display.setTextSize(boundarySize[2]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary + width_second_boundary, 40);
  display.print(bufThirdBoundary);

  display.display();
}

/*
This function displays a setup page on the display, allowing the user to set AEON.
The page consists of a title and a subtitle, separated by a horizontal line. The title
is centered at the top of the display and reads "Birthday". The subtitle is centered in
the middle of the display and reads "Setup Birthday".
*/
void AEON_Display::pageSetupBirthday() {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  const char *setup = "Setup";
  display.setTextSize(MIDDLE);
  display.getTextBounds(setup, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(setup);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  const char *setupBirthday = "Setup Birthday";
  display.setTextSize(SMALL);
  display.getTextBounds(setupBirthday, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 40);
  display.println(setupBirthday);

  display.display();
}

/*
Sets the Birthday for the page setup display. The display shows the current Birthday with customizable boundary sizes
for year, month, and day. The input values for the Birthday are passed as parameters. The parameter "p"
determines which boundary size to customize. The boundaries are defined by buffer combinations. This function clears the display,
sets the text color to white, and then prints the Date and boundary sizes in the center of the screen.
*/
void AEON_Display::pageSetupBirthday_set_date(EPage p, int year, int month, int day) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  const char *birthday = "Birthday";
  display.setTextSize(MIDDLE);
  display.getTextBounds(birthday, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(birthday);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  uint16_t width_first_boundary;
  uint16_t height_first_boundary;
  int16_t x1_first_boundary;
  int16_t y1_first_boundary;

  uint16_t width_second_boundary;
  uint16_t height_second_boundary;
  int16_t x1_second_bundary;
  int16_t y1_second_boundary;

  uint16_t width_third_boundary;
  uint16_t height_third_boundary;
  int16_t x1_third_boundary;
  int16_t y1_third_boundary;

  // Set the boundary size of hour, minute or second
  int boundarySize[] = { SMALL, SMALL, SMALL };
  switch (p) {
    case B_Year:
      ++boundarySize[0];
      break;

    case B_Month:
      ++boundarySize[1];
      break;

    case B_Day:
      ++boundarySize[2];
      break;

    default:
      break;
  }

  char bufFirstBoundary[8];
  char bufSecondBoundary[8];
  char bufThirdBoundary[8];

  if (boundarySize[1] == MIDDLE) {
    sprintf(bufFirstBoundary, "%02d:",
            year);

    sprintf(bufSecondBoundary, "%s",
            monthOfYearMapping[month]);

    sprintf(bufThirdBoundary, ":%02d",
            day);
  } else {
    sprintf(bufFirstBoundary, "%02d",
            year);

    sprintf(bufSecondBoundary, ":%s:",
            monthOfYearMapping[month]);

    sprintf(bufThirdBoundary, "%02d",
            day);
  }

  // Calc width of the bounds
  display.setTextSize(boundarySize[0]);
  display.getTextBounds(bufFirstBoundary, 0, 0, &x1_first_boundary, &y1_first_boundary, &width_first_boundary, &height_first_boundary);

  display.setTextSize(boundarySize[1]);
  display.getTextBounds(bufSecondBoundary, 0, 0, &x1_second_bundary, &y1_second_boundary, &width_second_boundary, &height_second_boundary);

  display.setTextSize(boundarySize[2]);
  display.getTextBounds(bufThirdBoundary, 0, 0, &x1_third_boundary, &y1_third_boundary, &width_third_boundary, &height_third_boundary);

  uint16_t complBoundarysWidth = (width_first_boundary + (width_second_boundary + width_third_boundary));
  uint16_t spaceBoundarysToScreen = (SCREEN_WIDTH - complBoundarysWidth) / 2;

  // First boundary
  display.setTextSize(boundarySize[0]);
  display.setCursor(spaceBoundarysToScreen, 40);
  display.print(bufFirstBoundary);

  // Second boundary
  display.setTextSize(boundarySize[1]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary, 40);
  display.print(bufSecondBoundary);

  // Third boundary
  display.setTextSize(boundarySize[2]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary + width_second_boundary, 40);
  display.print(bufThirdBoundary);

  display.display();
}

/*
This function displays a setup page on the display, allowing the user to set AEON.
The page consists of a title and a subtitle, separated by a horizontal line. The title
is centered at the top of the display and reads "Sex". The subtitle is centered in
the middle of the display and reads "Setup Sex".
*/
void AEON_Display::pageSetupSex() {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  const char *setup = "Setup";
  display.setTextSize(MIDDLE);
  display.getTextBounds(setup, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(setup);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  const char *setupSex = "Setup Sex";
  display.setTextSize(SMALL);
  display.getTextBounds(setupSex, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 40);
  display.println(setupSex);

  display.display();
}

/*
Sets the Sex for the page setup display. The display shows the current Sex. This function clears the display,
sets the text color to white, and then prints the Sex and boundary sizes in the center of the screen.
*/
void AEON_Display::pageSetupSex_set(int sex) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  const char *charSex = "Sex";
  display.setTextSize(MIDDLE);
  display.getTextBounds(charSex, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(charSex);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  uint16_t width_first_boundary;
  uint16_t height_first_boundary;
  int16_t x1_first_boundary;
  int16_t y1_first_boundary;

  uint16_t width_second_boundary;
  uint16_t height_second_boundary;
  int16_t x1_second_bundary;
  int16_t y1_second_boundary;

  uint16_t width_third_boundary;
  uint16_t height_third_boundary;
  int16_t x1_third_boundary;
  int16_t y1_third_boundary;

  // Set the boundary size of hour, minute or second
  int boundarySize[] = { SMALL, MIDDLE, SMALL };

  char bufFirstBoundary[8];
  char bufSecondBoundary[8];
  char bufThirdBoundary[8];

  sprintf(bufFirstBoundary, "%s", " ");

  switch (sex) {
    case ESex::Woman:
      sprintf(bufSecondBoundary, "%s", "Woman");
      break;

    case ESex::Man:
      sprintf(bufSecondBoundary, "%s", "Man");
      break;

    default:
      break;
  }

  sprintf(bufThirdBoundary, "%s", " ");

  // Calc width of the bounds
  display.setTextSize(boundarySize[0]);
  display.getTextBounds(bufFirstBoundary, 0, 0, &x1_first_boundary, &y1_first_boundary, &width_first_boundary, &height_first_boundary);

  display.setTextSize(boundarySize[1]);
  display.getTextBounds(bufSecondBoundary, 0, 0, &x1_second_bundary, &y1_second_boundary, &width_second_boundary, &height_second_boundary);

  display.setTextSize(boundarySize[2]);
  display.getTextBounds(bufThirdBoundary, 0, 0, &x1_third_boundary, &y1_third_boundary, &width_third_boundary, &height_third_boundary);

  uint16_t complBoundarysWidth = (width_first_boundary + (width_second_boundary + width_third_boundary));
  uint16_t spaceBoundarysToScreen = (SCREEN_WIDTH - complBoundarysWidth) / 2;

  // First boundary
  display.setTextSize(boundarySize[0]);
  display.setCursor(spaceBoundarysToScreen, 40);
  display.print(bufFirstBoundary);

  // Second boundary
  display.setTextSize(boundarySize[1]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary, 40);
  display.print(bufSecondBoundary);

  // Third boundary
  display.setTextSize(boundarySize[2]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary + width_second_boundary, 40);
  display.print(bufThirdBoundary);

  display.display();
}

/*
This function displays a setup page on the display, allowing the user to set AEON.
The page consists of a title and a subtitle, separated by a horizontal line. The title
is centered at the top of the display and reads "Setup". The subtitle is centered in
the middle of the display and reads "Setup Lifespan".
*/
void AEON_Display::pageSetupLifespan() {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  const char *setup = "Setup";
  display.setTextSize(MIDDLE);
  display.getTextBounds(setup, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(setup);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  const char *setupLifespan = "Setup Lifespan";
  display.setTextSize(SMALL);
  display.getTextBounds(setupLifespan, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 40);
  display.println(setupLifespan);

  display.display();
}

/*
Sets the Lifespan for the page setup display. The display shows the current Lifespan for the current Sex. This function clears the display,
sets the text color to white, and then prints the Lifespan and boundary sizes in the center of the screen.
*/
void AEON_Display::pageSetupLifespan_set(int lifespan) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  const char *charLifespan = "Lifespan";
  display.setTextSize(MIDDLE);
  display.getTextBounds(charLifespan, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(charLifespan);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  uint16_t width_first_boundary;
  uint16_t height_first_boundary;
  int16_t x1_first_boundary;
  int16_t y1_first_boundary;

  uint16_t width_second_boundary;
  uint16_t height_second_boundary;
  int16_t x1_second_bundary;
  int16_t y1_second_boundary;

  uint16_t width_third_boundary;
  uint16_t height_third_boundary;
  int16_t x1_third_boundary;
  int16_t y1_third_boundary;

  // Set the boundary size of hour, minute or second
  int boundarySize[] = { SMALL, SMALL, MIDDLE };

  char bufFirstBoundary[8];
  char bufSecondBoundary[8];
  char bufThirdBoundary[8];

  sprintf(bufFirstBoundary, "%s",
          charLifespan);

  sprintf(bufSecondBoundary, "%s",
          " ");

  sprintf(bufThirdBoundary, "%02d",
          lifespan);

  // Calc width of the bounds
  display.setTextSize(boundarySize[0]);
  display.getTextBounds(bufFirstBoundary, 0, 0, &x1_first_boundary, &y1_first_boundary, &width_first_boundary, &height_first_boundary);

  display.setTextSize(boundarySize[1]);
  display.getTextBounds(bufSecondBoundary, 0, 0, &x1_second_bundary, &y1_second_boundary, &width_second_boundary, &height_second_boundary);

  display.setTextSize(boundarySize[2]);
  display.getTextBounds(bufThirdBoundary, 0, 0, &x1_third_boundary, &y1_third_boundary, &width_third_boundary, &height_third_boundary);

  uint16_t complBoundarysWidth = (width_first_boundary + (width_second_boundary + width_third_boundary));
  uint16_t spaceBoundarysToScreen = (SCREEN_WIDTH - complBoundarysWidth) / 2;

  // First boundary
  display.setTextSize(boundarySize[0]);
  display.setCursor(spaceBoundarysToScreen, 40);
  display.print(bufFirstBoundary);

  // Second boundary
  display.setTextSize(boundarySize[1]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary, 40);
  display.print(bufSecondBoundary);

  // Third boundary
  display.setTextSize(boundarySize[2]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary + width_second_boundary, 40);
  display.print(bufThirdBoundary);

  display.display();
}

/*
This function displays a setup page on the display, allowing the user to set AEON.
The page consists of a title and a subtitle, separated by a horizontal line. The title
is centered at the top of the display and reads "Setup". The subtitle is centered in
the middle of the display and reads "Setup Reset".
*/
void AEON_Display::pageSetupReset() {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  const char *setup = "Setup";
  display.setTextSize(MIDDLE);
  display.getTextBounds(setup, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(setup);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  const char *reset = "Reset";
  display.setTextSize(SMALL);
  display.getTextBounds(reset, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 40);
  display.println(reset);

  display.display();
}

/*
Reset the AEON. The display shows the question for the Reset. This function clears the display,
sets the text color to white, and then prints the Question and boundary sizes in the center of the screen.
*/
void AEON_Display::pageSetupReset_set(EPage p) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  const char *reset = "Reset";
  display.setTextSize(MIDDLE);
  display.getTextBounds(reset, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(reset);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  uint16_t width_first_boundary;
  uint16_t height_first_boundary;
  int16_t x1_first_boundary;
  int16_t y1_first_boundary;

  uint16_t width_second_boundary;
  uint16_t height_second_boundary;
  int16_t x1_second_bundary;
  int16_t y1_second_bundary;

  uint16_t width_third_boundary;
  uint16_t height_third_boundary;
  int16_t x1_third_boundary;
  int16_t y1_third_boundary;

  // Set the boundary size of string yes and no
  int boundarySize[] = { SMALL, SMALL, SMALL };

  switch (p) {
    case Reset_Yes:
      ++boundarySize[0];
      break;

    case Reset_No:
      ++boundarySize[2];
      break;

    default:
      break;
  }

  char bufFirstBoundary[8];
  char bufSecondBoundary[8];
  char bufThirdBoundary[8];

  sprintf(bufFirstBoundary, "%s",
          "YES");

  sprintf(bufSecondBoundary, "%s",
          " ");

  sprintf(bufThirdBoundary, "%s",
          "NO");

  // Calc width of the bounds
  display.setTextSize(boundarySize[0]);
  display.getTextBounds(bufFirstBoundary, 0, 0, &x1_first_boundary, &y1_first_boundary, &width_first_boundary, &height_first_boundary);

  display.setTextSize(boundarySize[1]);
  display.getTextBounds(bufSecondBoundary, 0, 0, &x1_second_bundary, &y1_second_bundary, &width_second_boundary, &height_second_boundary);

  display.setTextSize(boundarySize[2]);
  display.getTextBounds(bufThirdBoundary, 0, 0, &x1_third_boundary, &y1_third_boundary, &width_third_boundary, &height_third_boundary);

  uint16_t complBoundarysWidth = (width_first_boundary + (width_second_boundary + width_third_boundary));
  uint16_t spaceBoundarysToScreen = (SCREEN_WIDTH - complBoundarysWidth) / 2;

  // First boundary
  display.setTextSize(boundarySize[0]);
  display.setCursor(spaceBoundarysToScreen, 40);
  display.print(bufFirstBoundary);

  // Second boundary
  display.setTextSize(boundarySize[1]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary, 40);
  display.print(bufSecondBoundary);

  // Third boundary
  display.setTextSize(boundarySize[2]);
  display.setCursor(spaceBoundarysToScreen + width_first_boundary + width_second_boundary, 40);
  display.print(bufThirdBoundary);

  display.display();
}

/*

*/
void AEON_Display::pageSetupReset_count_final(int cnt_reset) {

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  const char *reset = "Reset";
  display.setTextSize(MIDDLE);
  display.getTextBounds(reset, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(reset);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  uint16_t width_first_bundary;
  uint16_t height_first_bundary;
  int16_t x1_first_bundary;
  int16_t y1_first_bundary;

  uint16_t width_second_bundary;
  uint16_t height_second_bundary;
  int16_t x1_second_bundary;
  int16_t y1_second_bundary;

  uint16_t width_third_boundary;
  uint16_t height_third_boundary;
  int16_t x1_third_boundary;
  int16_t y1_third_boundary;

  // Set the boundary size
  int boundarySize[] = { SMALL, SMALL, MIDDLE };

  char bufFirstBoundary[8];
  char bufSecoundBoundary[8];
  char bufThirdBoundary[8];

  sprintf(bufFirstBoundary, "%s",
          "Reset ");

  sprintf(bufSecoundBoundary, "%s",
          "in ");

  sprintf(bufThirdBoundary, "%d",
          cnt_reset);

  // Calc width of the bounds
  display.setTextSize(boundarySize[0]);
  display.getTextBounds(bufFirstBoundary, 0, 0, &x1_first_bundary, &y1_first_bundary, &width_first_bundary, &height_first_bundary);

  display.setTextSize(boundarySize[1]);
  display.getTextBounds(bufSecoundBoundary, 0, 0, &x1_second_bundary, &y1_second_bundary, &width_second_bundary, &height_second_bundary);

  display.setTextSize(boundarySize[2]);
  display.getTextBounds(bufThirdBoundary, 0, 0, &x1_third_boundary, &y1_third_boundary, &width_third_boundary, &height_third_boundary);

  uint16_t complBoundarysWidth = (width_first_bundary + (width_second_bundary + width_third_boundary));
  uint16_t spaceBoundarysToScreen = (SCREEN_WIDTH - complBoundarysWidth) / 2;

  // First boundary
  display.setTextSize(boundarySize[0]);
  display.setCursor(spaceBoundarysToScreen, 40);
  display.print(bufFirstBoundary);

  // Second boundary
  display.setTextSize(boundarySize[1]);
  display.setCursor(spaceBoundarysToScreen + width_first_bundary, 40);
  display.print(bufSecoundBoundary);

  // Third boundary
  display.setTextSize(boundarySize[2]);
  display.setCursor(spaceBoundarysToScreen + width_first_bundary + width_second_bundary, 40);
  display.print(bufThirdBoundary);

  display.display();
}

/*
Back to Base
*/
void AEON_Display::pageSetupBack() {

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;
  
  const char *setup = "Setup";
  display.setTextSize(MIDDLE);
  display.getTextBounds(setup, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println(setup);

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  const char *back = "Back";
  display.setTextSize(SMALL);
  display.getTextBounds(back, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 40);
  display.println(back);

  display.display();
}

/*

*/
void AEON_Display::pageERROR(char *errorText) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // First line
  display.setTextSize(MIDDLE);
  display.getTextBounds("ERROR", 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 0);
  display.println("ERROR");

  // Second line
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);  // Line from x0-y20 to x128-y20

  // Third line
  display.setTextSize(SMALL);
  display.getTextBounds(errorText, 0, 0, &x1, &y1, &width, &height);
  display.setCursor((SCREEN_WIDTH - width) / 2, 40);
  display.println(errorText);

  display.display();
}

/*

*/
EReturn_DISPLAY AEON_Display::getErrorState() {
  return this->lastErrorState;
}
