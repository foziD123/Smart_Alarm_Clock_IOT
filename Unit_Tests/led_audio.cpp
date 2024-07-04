#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include "Font.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN 18   // or SCK
#define DATA_PIN 23  // or MOSI
#define CS_PIN 5     // or SS

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define SPEED_TIME 75
#define PAUSE_TIME 0

void setup(void)
{
    Serial.begin(9600);
    myDisplay.begin();
    myDisplay.setIntensity(0); // Set brightness (0-15)
    myDisplay.displayClear(); // Clear display
    myDisplay.setFont(numeric7Seg); // Set font to numeric7Seg
}

void displayNumber(int number)
{
    char str[2];  // Buffer to hold the number as a string
    sprintf(str, "%d", number);  // Convert the number to a string

    myDisplay.displayClear();  // Clear the display
    myDisplay.displayText(str, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);  // Display the string
    myDisplay.displayAnimate();  // Animate the display
}

void loop(void)
{
    for (int num = 0; num <= 9; num++)
    {
        displayNumber(num);  // Display the current number
        delay(1000);  // Delay for 1 second
    }
}
