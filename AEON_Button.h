/*
AEON_Button.h -For reading a button input and detecting short and long presses.
*/

#ifndef AEON_BUTTON_h
#define AEON_BUTTON_h

#include <Arduino.h>
#include "AEON_Enums.h"

class AEON_Button
{
private:
    const char *buttonName;
    int pinNum;
    bool lastState;
    bool lastButtonState = false;        // the previous reading from the input pin
    unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
    unsigned long pressedButtonTime = 0; // the time for pressed button
    static unsigned long debounceDelay;  // the debounce time
    static unsigned long shortPressTime; // time for short press

public:
    AEON_Button(int pinNum, const char *buttonName);
    void setupButton();
    EPressed loopButton();
};

#endif
