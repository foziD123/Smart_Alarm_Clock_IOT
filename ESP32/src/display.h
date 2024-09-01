#ifndef DISPLAY_H
#define DISPLAY_H

#include "libraries.h"
#include "paramaters.h"
using std::string;
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);


void setupDisplay() {

    myDisplay.begin();
    myDisplay.setFont(0, numeric7Se);
    myDisplay.setTextAlignment(PA_CENTER);
}


std::map<int, string> int_to_string_days = {
    {0, "Sunday"},
    {1, "Monday"},
    {2, "Tuesday"},
    {3, "Wednesday"},
    {4, "Thursday"},
    {5, "Friday"},
    {6, "Saturday"}
};

std::map<string, int> string_to_int_days1 = {
    {"Sunday", 6},
    {"Monday", 0},
    {"Tuesday", 1},
    {"Wednesday", 2},
    {"Thursday", 3},
    {"Friday", 4},
    {"Saturday", 5}
};

std::map<string, int> string_to_int_days2 = {
    {"Sun", 6},
    {"Mon", 0},
    {"Tue", 1},
    {"Wed", 2},
    {"Thu", 3},
    {"Fri", 4},
    {"Sat", 5}
};
// Array of motivational quotes
String quotes[10] = {
        "Believe in yourself.",
        "Keep pushing forward.",
        "Stay positive, work hard.",
        "You are stronger than you think.",
        "Success is no accident.",
        "Dream big and dare to fail.",
        "Never give up.",
        "Your only limit is you.",
        "The best time is now.",
        "You got this!"
    };

#endif