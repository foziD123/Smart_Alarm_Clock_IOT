#ifndef BUTTONS_H
#define BUTTONS_H
#include "libraries.h"
#include "paramaters.h"
#include <time.h>

#define BUTTON_SHOW_TASKS_PIN 32 //DISPLAY TASKS reminder - white button : /LONG = STOP -- SHORT = show tasks
#define BUTTON_STOP_ALARM_PIN 15 //LONG = STOP -- red button
#define BUTTON_SNOOZE_ALARM_PIN 33 //  SHORT = SNOOZE - GREEN - LONG = RESET
#define BUTTON_SWITCH_MODE_PIN 4 //SHORT = SWITCH DISPLAY - green button

enum ButtonState
{
    PRESSED,
    UNPRESSED
};

enum ButtonIsPressedResult
{
    NOTHING,
    SHORT_PRESS,
    LONG_PRESS
};


void initButtons()
{
    pinMode(BUTTON_SWITCH_MODE_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SHOW_TASKS_PIN, INPUT_PULLUP);
    pinMode(BUTTON_STOP_ALARM_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SNOOZE_ALARM_PIN, INPUT_PULLUP);
}

// Variables will change:
int currentButtonState = UNPRESSED;
int previousButtonState = UNPRESSED;
int currentButtonState2 = UNPRESSED;
int previousButtonState2 = UNPRESSED;
int currentButtonState3 = UNPRESSED;
int previousButtonState3 = UNPRESSED;
int currentButtonState4 = UNPRESSED;
int previousButtonState4 = UNPRESSED;

unsigned long buttonPress_time = 0;
unsigned long last_press_duration = 0;
unsigned long buttonPress_time2 = 0;
unsigned long last_press_duration2 = 0;
unsigned long buttonPress_time3 = 0;
unsigned long last_press_duration3 = 0;
unsigned long buttonPress_time4 = 0;
unsigned long last_press_duration4 = 0;

// For modes
int mode = 1;
bool modeChanged = false;
bool isPressed = false;

int isButtonPressed(int buttonPin, unsigned int longPressDuration = 1)
{
    if (buttonPin == BUTTON_SHOW_TASKS_PIN)
    {
        currentButtonState = digitalRead(buttonPin);
        if (currentButtonState != previousButtonState)
        {
            if (currentButtonState == PRESSED)
            {
                previousButtonState = currentButtonState;
                buttonPress_time = millis();
            }
            else // if unPressed
            {
                previousButtonState = currentButtonState;
                last_press_duration = millis() - buttonPress_time;
                Serial.print("BUTTON_SHOW_TASKS_PIN last_press_duration = ");
                Serial.println(last_press_duration);
                if (last_press_duration > 25 && last_press_duration < 800){
                    Serial.print("SHORT_PRESS ");
                    return SHORT_PRESS;
                }
                    
                else if (last_press_duration > longPressDuration * 800){
                    Serial.print("LONG_PRESS ");
                    return LONG_PRESS;
                }
                    
            }
        }
    }
    else if (buttonPin == BUTTON_STOP_ALARM_PIN)
    {
        currentButtonState2 = digitalRead(buttonPin);
        if (currentButtonState2 != previousButtonState2)
        {
            if (currentButtonState2 == PRESSED)
            {
                previousButtonState2 = currentButtonState2;
                buttonPress_time2 = millis();
            }
            else // if unPressed
            {
                previousButtonState2 = currentButtonState2;
                last_press_duration2 = millis() - buttonPress_time2;
                Serial.print("BUTTON_STOP_ALARM_PIN last_press_duration = ");
                Serial.println(last_press_duration2);
                if (last_press_duration2 > 25 && last_press_duration2 < 800){
                    Serial.print("SHORT_PRESS ");
                    return SHORT_PRESS;
                }
                    
                else if (last_press_duration2 > longPressDuration * 800){
                    Serial.print("LONG_PRESS ");
                    return LONG_PRESS;
                }
                    
            }
        }
    }
    else if (buttonPin == BUTTON_SWITCH_MODE_PIN)
    {
        currentButtonState3 = digitalRead(buttonPin);
        if (currentButtonState3 != previousButtonState3)
        {
            if (currentButtonState3 == PRESSED)
            {
                previousButtonState3 = currentButtonState3;
                buttonPress_time3 = millis();
            }
            else // if unPressed
            {
                previousButtonState3 = currentButtonState3;
                last_press_duration3 = millis() - buttonPress_time3;
                Serial.print("BUTTON_SWITCH_MODE_PIN last_press_duration = ");
                Serial.println(last_press_duration3);
                if (last_press_duration3 > 25 && last_press_duration3 < 800){
                    Serial.print("SHORT_PRESS ");
                    return SHORT_PRESS;}
                else if (last_press_duration3 > longPressDuration * 800){
                  Serial.print("LONG_PRESS ");
                  return LONG_PRESS;
                }
                    
            }
        }
    }
        else if (buttonPin == BUTTON_SNOOZE_ALARM_PIN)
    {
        currentButtonState4 = digitalRead(buttonPin);
        if (currentButtonState4 != previousButtonState4)
        {
            if (currentButtonState4 == PRESSED)
            {
                previousButtonState4 = currentButtonState4;
                buttonPress_time4 = millis();
            }
            else // if unPressed
            {
                previousButtonState4 = currentButtonState4;
                last_press_duration4 = millis() - buttonPress_time4;
                Serial.print("BUTTON_SNOOZE_ALARM_PIN last_press_duration = ");
                Serial.println(last_press_duration3);
                if (last_press_duration4 > 25 && last_press_duration4 < 5000){
                    Serial.print("SHORT_PRESS ");
                    return SHORT_PRESS;}
                else if (last_press_duration4 > longPressDuration * 5000){
                  Serial.print("LONG_PRESS ");
                  return LONG_PRESS;
                }
            }
        }
    }
    return NOTHING;
}

#endif