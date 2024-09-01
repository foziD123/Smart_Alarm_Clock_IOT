#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include "Font.h"
#include <WiFiManager.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Firebase_ESP_Client.h>
#include <driver/i2s.h>
#include <time.h>  // Include time.h for RTC functionality
#include <ArduinoJson.h>
#include <SD.h>
#include "Audio.h"
// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
#include <map>
#include <string>
#include <sstream>

#include <set>
#include <unordered_map>
#include <vector>
#include <utility>

using std::string;
using std::vector;
using std::set;
using std::pair;
using std::unordered_map;