/* 
AEON_Button.cpp - This represents a physical button that can be connected to an board. This provides methods to initialize the button and check whether it has been pressed or not. The button has a debounce delay to avoid false readings and can be programmed to trigger a short or long press event. This also allows the user to set custom debounce delay and short press time values. The pin number and name of the button can be specified when creating an instance of the class.


PIN Button SET  = 22
PIN Button P    = 23
PIN Button N    = 24
PIN Button OK   = 25
*/

#include <Arduino.h>
#include "AEON_Enums.h"
#include "AEON_Button.h"

unsigned long AEON_Button::debounceDelay = 50;     // default 50
unsigned long AEON_Button::shortPressTime = 1500;  // default 1500

AEON_Button::AEON_Button(int pinNum, const char *buttonName) {
  this->pinNum = pinNum;
  this->buttonName = buttonName;
}

/*
Button
*/
void AEON_Button::setupButton() {
  // initialize the Pushbutton pin as an input:
  pinMode(this->pinNum, INPUT);  // pinMode(BUTTON_PIN, INPUT_PULLUP);
}

/*
Check if Button pressed
*/
EPressed AEON_Button::loopButton() {
  bool reading = digitalRead(this->pinNum);
  EPressed localPressed = NO_CHANGE;

  // Check if button state has changed
  if (reading != this->lastButtonState) {
    this->lastDebounceTime = millis();
    this->lastState = true;
  }

  this->lastButtonState = reading;

  // Check if debounce delay has passed
  if (this->lastState && (millis() - this->lastDebounceTime) > AEON_Button::debounceDelay) {

    this->lastState = false;

    // button is pressed
    if (reading) {
      this->pressedButtonTime = millis();
      // Serial.printf("Button pressed %s \n", this->buttonName);

    // button is released
    } else {

      // check if short press time has passed
      if ((millis() - this->pressedButtonTime) < AEON_Button::shortPressTime) {

        // button was pressed for a short time
        //Serial.printf("Pressed %s short \n", this->buttonName);
        localPressed = EPressed::SHORT;
      } else {

        // button was pressed for a long time
        //Serial.printf("Pressed %s long \n", this->buttonName);
        localPressed = EPressed::LONG;
      }
    }
  }

  return localPressed;
}
