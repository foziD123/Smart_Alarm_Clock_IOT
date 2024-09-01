#include "libraries.h"
#include "buttons.h"
#include "Font.h"
#include "paramaters.h"
#include "display.h"


//Data sructures for alarms 
vector<set<pair<String,String> > > alarms(7);
unordered_map<string,vector<int>> map_id_to_days;
unordered_map<string,string> map_id_to_ringtone;
unordered_map<string,String> map_id_to_label;

//Data sructures for tasks
unordered_map<string,pair<String,String>> tasks_map;// id->(task_date,task_message)
std::vector<string> myVectorTasks_to_say;
std::vector<string> myVectorTasks_to_show;
std::map<int,string> tasks_to_say_map;

// Time params
uint16_t h, m, s;
String _Date; 
String _Time; 
String _Day;
String snoozeTime;
unsigned long snoozeMinutes = 1;
struct tm time_FB_format;
uint32_t sec = millis();

//buffers for display/tts
char buffer[1024];
char buffer_tasks[10000];
char buffer_display[1024];

//alarms params/flags
String curr_alarm;
String curr_alarm_label;
bool isAlarm = false;
bool newAlarmSet = false;
bool stopAlarm = false;
bool snoozeAlarm = false;
unsigned long timeAlarmStarted;

//tasks params/flags
int numTaskToday = 0;
bool displayTasks = false;
bool isReadTask = false;
bool changedTasks = false;
//read tasks params:
int currentTaskIndex = 0;
string tasks_for_today = "";
string tasks_to_say="";
unsigned long previousMillis = 0;
const long interval = 2000;  // 2 seconds delay between tasks

//msg params/flags
String msg ;
bool isShowMsg = false;
int randomNumber = 1;
bool isWifiConnected = false;

// Settings params
int _Volume;// = 20;
int _Brightness;// = 15;
int _Brightness_tmp;



//Firebase params:
bool FB_first_sync = true;
FirebaseData streamRoot;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
WiFiManager wifiManager;

//sync time NTP
const long gmtOffset_sec = 3600 * 2; // Adjust to your local timezone offset in seconds
const int daylightOffset_sec = 3600; // Daylight Saving Time offset
bool timeSynchronized = false;

//audio
Audio my_audio;

void updateDisplay();

std::string dateReconstructor();

void freeHeap(int num)
{
  size_t freeHeap = esp_get_free_heap_size();
  Serial.print("Free heap size ");
  Serial.print(num);
  Serial.print(": ");
  Serial.print(freeHeap);
  Serial.print(" bytes\n");
}

void setUpWifi(){
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");

    // Try to connect to existing WiFi network
    if (!wifiManager.autoConnect("SmartAlarm")) {
        Serial.println("Failed to connect and hit timeout");
        ESP.restart();
    }

    Serial.println("Connected to WiFi");
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Function to extract priority from the string (e.g., extract "high" from "task1 high")
std::string extractPriority(const std::string& str) {
    std::string keyword = "priority : ";
    size_t pos = str.find(keyword);
    
    if (pos != std::string::npos) {
        // Extract the priority word after "priority : "
        return str.substr(pos + keyword.length());
    }
    
    return "";  // Return empty string if "priority : " not found
}

// Custom comparator for sorting according to priority embedded in the string
bool compareByPriority(const std::string& a, const std::string& b) {
    // Define the priority order
    std::map<std::string, int> priorityMap = {
        {"High", 3},
        {"Medium", 2},
        {"Low", 1}
    };
    std::string priorityA = trim(extractPriority(a));
    std::string priorityB = trim(extractPriority(b));

    // Print for debugging purposes
    Serial.print("Comparing: ");
    Serial.print(priorityA.c_str());
    Serial.print("vs ");
    Serial.println(priorityB.c_str());

    // Compare based on priority
    // Compare based on the priority map values
    Serial.print("Comparing from map: ");
    Serial.print(priorityMap[priorityA]);
    Serial.print("vs ");
    Serial.println(priorityMap[priorityB]);
    return priorityMap[priorityA] > priorityMap[priorityB];
}

void InitAudio()
{
    my_audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    my_audio.setVolume(21);
}

void playTasks() {
    if (numTaskToday == 0){
        sprintf(buffer,"Free day - lululululululeesh");
        if(!my_audio.isRunning()) my_audio.connecttospeech(buffer, "en");
        return;
    }
    else {
      int num_msg=0;
      int index = 0;
      tasks_to_say = "";
      tasks_to_say_map.clear();
      myVectorTasks_to_say.clear();
      for(auto it = tasks_map.begin(); it != tasks_map.end(); it++){
          if(it->second.first.startsWith(dateReconstructor().c_str())){

              num_msg++;
              sprintf(buffer_tasks,"task %d is: %s  ",num_msg,it->second.second.c_str());
              myVectorTasks_to_say.push_back(buffer_tasks);

            }
          }

      std::sort(myVectorTasks_to_say.begin(), myVectorTasks_to_say.end(), compareByPriority);
      for(auto iter = myVectorTasks_to_say.begin(); iter != myVectorTasks_to_say.end() ; iter++){
        index++;
        string tmp = *iter;
        tmp[5] = *(std::to_string(index)).c_str();
        tasks_to_say_map[index] = tmp;
        Serial.println((*iter).c_str());
      }
      
      if (numTaskToday == 1){
          sprintf(buffer, "Your task for today is %d :" ,numTaskToday);
          
      }
      if(numTaskToday > 1) {
          sprintf(buffer, "Your tasks for today are %d :" ,numTaskToday);
      }

      tasks_to_say_map[0] = buffer;
      isReadTask = true;
    }
}

void playMessage() {
    randomNumber = std::rand() % 10 + 1;
    String motivation = quotes[randomNumber - 1];
    sprintf(buffer,"Hey you! %s. ",motivation.c_str());
    if(!my_audio.isRunning()) my_audio.connecttospeech(buffer, "en");
}

void playFile(std::string filename)
{   
      if (!my_audio.isRunning()) my_audio.connecttoFS(SD, filename.c_str());
      Serial.println("play file");
}

void InitSDCard()
{
    pinMode(SD_CS_PIN, OUTPUT);
    digitalWrite(SD_CS_PIN, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    SPI.setFrequency(1000000);
    if (!SD.begin(SD_CS_PIN, SPI))
    {
        Serial.println("SD Card Mount Failed");
    }
}


void syncTimeWithNTP() {
    configTime(gmtOffset_sec, daylightOffset_sec, "ntp.technion.ac.il", "time.nist.gov", "time.google.com");
    Serial.print("Waiting for NTP time sync: ");
    while (time(nullptr) < 8 * 3600 * 2) { // Arbitrary time check to ensure NTP sync
        //delay(500);
        //Serial.print("in while ");
        if (myDisplay.displayAnimate()) myDisplay.displayText("Sync NTP", PA_CENTER, 30, 100, PA_SCROLL_LEFT,PA_SCROLL_LEFT);
    }
    Serial.println("");
    Serial.println("Time synchronized with NTP server");
    timeSynchronized = true;
}

void countDown() {
    unsigned long startTime = millis();  // Record the start time

    while (millis() - startTime < 3000) {
        // Wait until 10 seconds have passed
        // You can perform other non-blocking tasks here if needed
        updateDisplay();
    }

    // 10 seconds have passed
    newAlarmSet = false;
    displayTasks = false;
    Serial.println("3 seconds passed.");
}

void initializeFirebase() {
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}


void streamTimeoutCallback(bool timeout) {
    if (timeout) {
        Serial.println("Stream timeout, resuming...");
    }
}



void calculateTimeSnooze() {
        // Parse the time string
        Serial.print("======== in calculateTimeSnooze ====");
         
        // Add 5 minutes to the time
        time_FB_format.tm_min += snoozeMinutes;
            
        // Normalize the time (this handles minute overflow, etc.)
        mktime(&time_FB_format);

        // Convert back to string format
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S.000", &time_FB_format);
        snoozeTime = String(buffer);

        // Optionally, reset any alarm status flags here
        
    
}

// Function to add 5 minutes to the alarm time
void snoozeAlarmFunc(String jsonPath) {
  if (Firebase.ready()) {
    Serial.print("snoozeTime before  calculateTimeSnooze");
    Serial.println(snoozeTime.c_str());
    calculateTimeSnooze();
    Serial.print("snoozeTime after  calculateTimeSnooze");
    Serial.println(snoozeTime.c_str());

    // Read the existing JSON object from Firebase
    if (Firebase.RTDB.getJSON(&fbdo, jsonPath)) {
      json = fbdo.jsonObject(); // Get the existing JSON object

      // Update a specific field within the JSON object
      json.set("time", snoozeTime);  // Update the "description" field

      // Write the updated JSON object back to Firebase
      if (Firebase.RTDB.setJSON(&fbdo, jsonPath, &json)) {
        Serial.println("Field updated successfully within JSON.");
      } else {
        Serial.println("Failed to update field within JSON.");
        Serial.println(fbdo.errorReason());
      }
    } else {
      Serial.println("Failed to get JSON from Firebase.");
      Serial.println(fbdo.errorReason());
    }
  }
}


void stopAlarmFunc(String jsonPath) {
  if (Firebase.ready()) {
    // Read the existing JSON object from Firebase
    if (Firebase.RTDB.getJSON(&fbdo, jsonPath)) {
      json = fbdo.jsonObject(); // Get the existing JSON object

      // Update a specific field within the JSON object
      json.set("enabled", false);  // Update the "description" field

      // Write the updated JSON object back to Firebase
      if (Firebase.RTDB.setJSON(&fbdo, jsonPath, &json)) {
        Serial.println("Field updated successfully within JSON.");
      } else {
        Serial.println("Failed to update field within JSON.");
        Serial.println(fbdo.errorReason());
      }
    } else {
      Serial.println("Failed to get JSON from Firebase.");
      Serial.println(fbdo.errorReason());
    }
  }
}

bool isGetClock(String* curr_alarm_id) {
    int currentDay = string_to_int_days2[_Day.c_str()];
    for(pair<String,String> p : alarms[currentDay]){

        if(p.first == _Time) {
            *curr_alarm_id = p.second; 
            return true;
        }
    }
    return false;
}

bool isGetAlarm(String* curr_alarm_id) {
    int currentDay = string_to_int_days1[_Day.c_str()];
    return currentDay >=0 && currentDay <7 && isGetClock(curr_alarm_id);
}

pair<String,String> get_pair(String& alarm_id , int day) {
    for(pair<String,String> p : alarms[day]){
        if(p.second == alarm_id){
            return p;
        }
    }
    return pair<String,String>("","");
}

void delete_alarmId(String alarm_id) {
  string alarm_id_str = alarm_id.c_str();
  for(int d : map_id_to_days[alarm_id_str]) {
    pair<String,String> p = get_pair(alarm_id,d);
    alarms[d].erase(p);
  }
  map_id_to_days[alarm_id_str].clear();
}

void insert_alarm(String& alarm_id, JsonArray& days_doc_str, String& clock, String& ringtone, String& label){
    vector<int> vec_days;
    JsonDocument days_doc;
    Serial.print("hereee iam out : ");
    int curr_day = string_to_int_days2[_Day.c_str()];
    vec_days.push_back(curr_day);
    pair<String,String> new_pair_clock(clock,alarm_id);
    alarms[curr_day].insert(new_pair_clock);
    for(int index = 0 ; index < days_doc_str.size(); index ++ ){
        if(curr_day != index && days_doc_str[index]){
            vec_days.push_back(index);
            pair<String,String> new_pair_clock(clock,alarm_id);
            alarms[index].insert(new_pair_clock);
            Serial.print("hereee iam in : ");
            Serial.println(index);
      }

      //Serial.print("hereee iam in : ");
    }
    string str_alarm_id = alarm_id.c_str();
    string str_ringtone = ringtone.c_str();
    string str_label = label.c_str();
    map_id_to_days[str_alarm_id] = vec_days;
    map_id_to_ringtone[str_alarm_id] = str_ringtone;
    map_id_to_label[str_alarm_id] = label;
    //newAlarmSet = true;
    //countDown();
}

void delete_alarm(String alarm_id){
  string alarm_id_str = alarm_id.c_str();
    for(int d : map_id_to_days[alarm_id_str]) {
        pair<String,String> p = get_pair(alarm_id,d);
        alarms[d].erase(p);
    }
    map_id_to_days.erase(alarm_id_str);
}

void dataExtractionJson(String& str_document, bool first_call){
  JsonDocument jdoc;
  deserializeJson(jdoc, str_document);  
  String clock = jdoc["time"].as<String>().substring(11,16);
  JsonArray str_days = jdoc["days"].as<JsonArray>();
  String str_enable = jdoc["enabled"].as<String>();
  String alarm_label = jdoc["label"].as<String>();
  String alarm_id = jdoc["id"].as<String>();
  String alarm_sound = jdoc["sound"].as<String>();
  string str_alarm_id = alarm_id.c_str();
  Serial.print("alarm id is : ");
  Serial.println(alarm_id);

  Serial.print("alarm_label  is : ");
  Serial.println(alarm_label);
  Serial.print("alarm_sound is : ");
  Serial.println(alarm_sound);
  Serial.print("clock is : ");
  Serial.println(clock);
  
  if (map_id_to_days.find(str_alarm_id) != map_id_to_days.end()){
      delete_alarmId(alarm_id);
  }
  if(str_enable == "false") return;
  else if(!first_call){
    newAlarmSet = true;
    countDown();
  }
  insert_alarm(alarm_id, str_days, clock,alarm_sound , alarm_label);
}



std::string dateReconstructor() {
    int d, m, y;
    sscanf(_Date.c_str(), "%d.%d.%d", &d, &m, &y);

    char formattedDate[11]; // 6 characters for the date + 1 for the null terminator
    sprintf(formattedDate, "%02d-%02d-20%d",d , m, y);
    Serial.print("formatted_date = ");
    Serial.println(formattedDate);
    return formattedDate;
}

void insertTask(string& id_task, String& str_doc_task){
        JsonDocument doc_task;
        deserializeJson(doc_task, str_doc_task);
        string date_task = doc_task["dueDate"].as<String>().c_str();
        String date_task_print = doc_task["dueDate"].as<String>();
        Serial.print("date_task_print = ");
        Serial.println(date_task_print);
        if(tasks_map.find(id_task) == tasks_map.end() && date_task == dateReconstructor().c_str()){
          numTaskToday+=1;
        }
        String message_task = doc_task["name"].as<String>();
        
        String priority_task = doc_task["priority"].as<String>();
        String msg_priority = message_task + " with priority : " + priority_task;
        String time_task = date_task.c_str();
        pair<String,String> new_pair_task(time_task,msg_priority);
        if(message_task == "null") return;
        tasks_map[id_task] = new_pair_task;
}



void StreamCallBackTasks(FirebaseStream& data, bool is_first_call_tasks, String str_doc_tasks){
  if (is_first_call_tasks){
    JsonDocument doc;
    deserializeJson(doc, str_doc_tasks);
    for(JsonPair kv : doc.as<JsonObject>()){
        string id_task = kv.key().c_str();
        String str_doc_task = kv.value().as<String>();
        insertTask(id_task,str_doc_task);
    }
  }
  else{
    if(data.dataType() == "null"){
      string id_task = data.dataPath().substring(7).c_str();
      String date_task = tasks_map[id_task].first;
      if(date_task.startsWith(dateReconstructor().c_str())){
        numTaskToday-=1;
      }
      tasks_map.erase(id_task);
    } else{
      string id_task = data.dataPath().substring(7).c_str();
      String str_doc_task = data.jsonString();
      Serial.print("data.dataPath()");
      Serial.println(data.dataPath());
      insertTask(id_task,str_doc_task);
    }
  }
  changedTasks = true;

}


void streamCallbackAlarms(FirebaseStream& data, bool first_call, String alarms_doc_str)
{ 
  if(first_call){
      JsonDocument doc;
      deserializeJson(doc, alarms_doc_str);
      for(JsonPair kv : doc.as<JsonObject>()){
          String jval = kv.value().as<String>();
          String alarm_id = kv.key().c_str();
          dataExtractionJson(jval,first_call );
      }
  }
   else{
      if(data.dataType() == "null"){
        String alarm_id = data.dataPath().substring(8);
        delete_alarm(alarm_id.c_str());
      }
      else{
        String str_data = data.jsonString();
        dataExtractionJson(str_data, first_call ); 
      }
        
   }

  Serial.println();
}

void StreamCallBackSettings(String& setting_doc_str){
  JsonDocument doc;
  deserializeJson(doc, setting_doc_str);
  _Volume = doc["volume"].as<int>();
  _Brightness = doc["brightness"].as<int>();
  snoozeMinutes = doc["snooze"].as<int>();
  float percentOf15 = (_Brightness / 100.0) * 15;
  int _Brightness_tmp = static_cast<int>(percentOf15); 
  if (_Brightness_tmp> 15) {
    _Brightness_tmp = 15;
  }
  Serial.print("_Brightness = ");
  Serial.println(_Brightness);
  Serial.print("_Volume = ");
  Serial.println(_Volume);
  Serial.print("snoozeMinutes = ");
  Serial.println(snoozeMinutes);
  myDisplay.setIntensity(_Brightness);
  my_audio.setVolume(_Volume);
}

void StreamCallBackRoot(FirebaseStream data){
  if(data.dataPath().startsWith("/esp32")) return;
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                 data.streamPath().c_str(),
                 data.dataPath().c_str(),
                 data.dataType().c_str(),
                 data.eventType().c_str());
  if (FB_first_sync){
     FB_first_sync = false;
      String str_data = data.jsonString();
      JsonDocument doc_root;
      deserializeJson(doc_root, str_data);
      for(JsonPair kv : doc_root.as<JsonObject>()){
          String root_child = kv.key().c_str();
          if(root_child == "alarms"){
            Serial.println("in alarms  first call...");
              String alarms_doc_str = kv.value().as<String>();
              streamCallbackAlarms(data, true, alarms_doc_str);
          } else if (root_child == "settings"){
            Serial.println("in  settings first call...");
            String settings_doc_str = kv.value().as<String>();
            Serial.println(settings_doc_str);
            StreamCallBackSettings(settings_doc_str);
          } else  {
            Serial.println("NOT in alarms and settings first call...");
            String tasks_doc_str = kv.value().as<String>();
            StreamCallBackTasks(data, true, tasks_doc_str);
          }
          
      }
  }
  else{
      if(data.dataPath().startsWith("/alarms")){
        Serial.println("in alarms not first call...");
        String str_data = data.jsonString();
        Serial.println("in alarms the path is ");
        Serial.println(str_data);
        if(data.dataType() == "boolean") return;
        streamCallbackAlarms(data, false, str_data);
    } else if ( data.dataPath().startsWith("/settings")){
      Serial.println("in settings not first call...");
      String str_setting_path ;//="/settings";
      if ( data.dataPath() == "/settings/volume"){
        _Volume = data.intData();
        Serial.print("_Volume = ");
        Serial.println(_Volume);
        my_audio.setVolume(_Volume);
      }
      else if(data.dataPath() == "/settings/brightness"){
        _Brightness = data.intData();
        Serial.print("_Brightness = ");
        Serial.println(_Brightness);
        float percentOf15 = (_Brightness / 100.0) * 15;
        _Brightness_tmp = static_cast<int>(percentOf15); 
        if (_Brightness_tmp> 15) {
          _Brightness_tmp = 15;
        }
        Serial.println(_Brightness_tmp);
        myDisplay.setIntensity(_Brightness);

      }
      else if(data.dataPath() == "/settings/snooze"){
        snoozeMinutes = data.intData();
        Serial.print("snoozeMinutes = ");
        Serial.println(snoozeMinutes);

      }

    } else  {
      Serial.println("NOT in alarms and settings not first call...");
      String str_data = data.jsonString();
      Serial.println("in tasks the path is ");
      Serial.println(str_data);
      StreamCallBackTasks(data, false, str_data);
    }
  }
}


void beginFirebaseStream()
{
    String streamPath = "/";
    Firebase.RTDB.beginStream(&streamRoot, streamPath);
    Firebase.RTDB.setStreamCallback(&streamRoot,StreamCallBackRoot , streamTimeoutCallback);
}



void showMessage(){
    String motivation_msg = quotes[randomNumber - 1];
    sprintf(buffer_display,"Hey you : %s. ",motivation_msg.c_str());
    if (myDisplay.displayAnimate()) myDisplay.displayText(buffer_display, PA_CENTER, 40, 100, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

}

void showTasks(){
    if(changedTasks){
        int num_msg = 0;
        int index = 0;
        tasks_for_today = "Your tasks for today : ";
        myVectorTasks_to_show.clear();
        for(auto it = tasks_map.begin(); it != tasks_map.end(); it++){

            if(it->second.first.startsWith(dateReconstructor().c_str())){
            num_msg++;
            sprintf(buffer_display,"task %d is: %s  ",num_msg,it->second.second.c_str());
            Serial.println(buffer_display);
            myVectorTasks_to_show.push_back(buffer_display);
            
        }
        }
        std::sort(myVectorTasks_to_show.begin(), myVectorTasks_to_show.end(), compareByPriority);
        for(auto iter = myVectorTasks_to_show.begin(); iter != myVectorTasks_to_show.end() ; iter++){
          index++;
          string tmp = *iter;
          tmp[5] = *(std::to_string(index)).c_str();
          tasks_for_today+=tmp;
        }
        if (num_msg == 0){
          tasks_for_today = "Free day - lululululululeesh";
        }
        changedTasks = false;
    }

    if(myDisplay.displayAnimate()) myDisplay.displayText(tasks_for_today.c_str(), PA_CENTER, 30, 100, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

}

void updateDisplay() {
    if(isAlarm){
        sprintf(buffer_display,"Alarm Ringing : %s. ",curr_alarm_label.c_str());
        if (myDisplay.displayAnimate()) myDisplay.displayText(buffer_display, PA_CENTER,  30, 100, PA_SCROLL_LEFT,PA_SCROLL_LEFT);
        
    }
    else if(displayTasks){
        showTasks();
    }
    /*else if(newAlarmSet){
        if (myDisplay.displayAnimate()) myDisplay.displayText("New Alarm is set", PA_CENTER,  30, 100, PA_SCROLL_LEFT,PA_SCROLL_LEFT);
    }*/
    else{
        switch (mode) {
        case 1:
            myDisplay.print(_Time);
            modeChanged = false;
            break;
        case 2:
            myDisplay.print(_Date.c_str());
            modeChanged = false;
            break;
        case 3:
            if(myDisplay.displayAnimate()) myDisplay.displayText(_Day.c_str(), PA_CENTER, 50, 100, PA_NO_EFFECT, PA_NO_EFFECT);
            modeChanged = false;
            break;
        case 4:
            showMessage();
            modeChanged = false;
            break;
        case 5:
            String _wifi;
            if(isWifiConnected) {
              _wifi = " Wifi";
            }
            else{
              _wifi = "No Wifi";
            }
            myDisplay.print(_wifi.c_str());
            modeChanged = false;
            break;

        }
        

    }

}

void updateStopAlarm() {
  if (millis() - timeAlarmStarted > 59800) {
    stopAlarm = false;

  } 
}

void updateTriggerButton() {
    if (isButtonPressed(BUTTON_SWITCH_MODE_PIN) == SHORT_PRESS) // if pressed change mode
    {
        mode = mode == 5 ? 1 : mode + 1;
        modeChanged = true;
        Serial.print("mode = ");
        Serial.println(mode);
        if(mode != 4) isShowMsg = false;
    } 
    if (isButtonPressed(BUTTON_SNOOZE_ALARM_PIN) == LONG_PRESS) { // reset
      Serial.println("****** Reset *******");
      myDisplay.print("Reset");
      delay(1500);
      wifiManager.resetSettings();
      ESP.restart();
  }

}


void updateTime() { 
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    int day = timeinfo.tm_wday;
    _Day = int_to_string_days[day].c_str();
    _Day = int_to_string_days[day].substr(0, 3).c_str();

    // Format day, month, and year
    if (timeinfo.tm_mday < 10 && timeinfo.tm_mon < 10) {
        sprintf(buffer, "%1d.%1d.%02d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year % 100);
    } else if (timeinfo.tm_mday < 10) {
        sprintf(buffer, "%1d.%02d.%02d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year % 100);
    } else if (timeinfo.tm_mon < 10) {
        sprintf(buffer, "%02d.%1d.%02d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year % 100);
    } else {
        sprintf(buffer, "%02d.%02d.%02d", timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year % 100);
    }
    _Date = buffer;
  
    sprintf(buffer, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    h = timeinfo.tm_hour;
    m =  timeinfo.tm_min;
    _Time = buffer;
    time_FB_format = timeinfo;
    
}


void setup() {
    Serial.begin(9600);
    freeHeap(1); 

    setupDisplay();
    freeHeap(2);

    _Volume = 20;
    _Brightness = 15;
    float percentOf15 = (_Brightness / 100.0) * 15;
    _Brightness_tmp = static_cast<int>(percentOf15); 
    if (_Brightness_tmp> 15) {
        _Brightness_tmp = 15;
      }
    myDisplay.setIntensity(_Brightness);

    setUpWifi();
    
    freeHeap(3);

    // Initialize Clock
    myDisplay.displayClear();
    //myDisplay.print("sync ntp");
    syncTimeWithNTP();  // Synchronize time with NTP server

    // Initialize Firebase
    initializeFirebase();
    beginFirebaseStream();
    freeHeap(4);

        
    //SDCardInit();
    InitAudio();
    InitSDCard();
    //setupI2S();
    //playFile("Sound 2.mp3");

  
    initButtons();
    freeHeap(5);
}

void loop() {

    updateTime();
    updateTriggerButton();

    myDisplay.setIntensity(_Brightness);

    updateDisplay();
    // Animate display (this must be called frequently)
    myDisplay.displayAnimate();

    unsigned long currentMillis = millis();

    // Check if it's time to read the next task
    if (displayTasks && isReadTask && currentMillis - previousMillis >= interval) {
        
        previousMillis = currentMillis;  // Update the timing
        if (currentTaskIndex <= numTaskToday) {
            // Extract the current task to be read out loud
                if (!my_audio.isRunning()) {
                    my_audio.connecttospeech(tasks_to_say_map[currentTaskIndex].c_str(), "en");
                    Serial.print("task ");
                    Serial.print(currentTaskIndex);
                    Serial.println(tasks_to_say_map[currentTaskIndex].c_str());
                    currentTaskIndex++;  // Move to the next task
                }

        }
        else{
          isReadTask = false;
        }

    } 

    // If Wi-Fi reconnected and time is not synchronized, re-synchronize the time
    if (WiFi.status() == WL_CONNECTED && !timeSynchronized) {
        syncTimeWithNTP();
    }
    if (WiFi.status() == WL_CONNECTED) isWifiConnected = true;
    if (WiFi.status() != WL_CONNECTED) {
      isWifiConnected = false;
      FB_first_sync = true;
    }

    if (sec < millis()) {
        
        Firebase.RTDB.setTimestampAsync(&fbdo, "/esp32/status");
        if(!displayTasks && !isShowMsg && mode == 4){
          isShowMsg = true;
          playMessage();
        }

        sec = millis() + 1000;
        if ( !stopAlarm && (isGetAlarm(&curr_alarm))) {
            Serial.println("Alarm  is on !");
            if (!isAlarm && isGetAlarm(&curr_alarm)) { // new alarm
                timeAlarmStarted = millis();
                isAlarm = true;
            }

            else {
                string curr_ringtone = map_id_to_ringtone[curr_alarm.c_str()];
                if(curr_ringtone == "Default") curr_ringtone = "Sound 1";
                curr_alarm_label = map_id_to_label[curr_alarm.c_str()];
                sprintf(buffer,"%s.mp3",curr_ringtone.c_str());
                playFile(buffer);
            }
        }
        else {

            isAlarm = false;
        }

    }


  //stoping audio
  if(!displayTasks && isButtonPressed(BUTTON_SHOW_TASKS_PIN) == SHORT_PRESS ){
    displayTasks = true;
    currentTaskIndex = 0;
    playTasks();

  }
  if(displayTasks && isButtonPressed(BUTTON_SHOW_TASKS_PIN) == LONG_PRESS ){
    displayTasks = false;
  }
    if (my_audio.isRunning())
    {
      my_audio.loop();
      if (isAlarm  &&  isButtonPressed(BUTTON_STOP_ALARM_PIN) == LONG_PRESS) {
        my_audio.stopSong();
        isAlarm = false;
        stopAlarm = true;
        char path_char [1024];
        sprintf(path_char,"/alarms/%s",  curr_alarm.c_str());
   
        Serial.print("path_char is : ");
        Serial.println(path_char);

        Serial.print("curr_alarm id is : ");
        Serial.println(curr_alarm);
        stopAlarmFunc(path_char);
     }
     if(isAlarm  &&  isButtonPressed(BUTTON_SNOOZE_ALARM_PIN) == SHORT_PRESS){
        //snooze
        my_audio.stopSong();
        isAlarm = false;
        stopAlarm = true;
        char path_char [1024];
        sprintf(path_char,"/alarms/%s",  curr_alarm.c_str());
        Serial.println("snooooooooooze is : ");
        Serial.print("path_char is : ");
        Serial.println(path_char);
        snoozeAlarmFunc(path_char);
     }
     if(!isAlarm){
        stopAlarm = true;
     }
    }

    if (isAlarm  &&  isButtonPressed(BUTTON_STOP_ALARM_PIN) == LONG_PRESS) {
        isAlarm = false;
        stopAlarm = true;
        char path_char [1024];
        sprintf(path_char,"/alarms/%s",  curr_alarm.c_str());

        Serial.print("path_char is : ");
        Serial.println(path_char);

        Serial.print("curr_alarm id is : ");
        Serial.println(curr_alarm);
        stopAlarmFunc(path_char);
     }
    if(isAlarm  &&  isButtonPressed(BUTTON_SNOOZE_ALARM_PIN) == SHORT_PRESS){
        //snooze
        isAlarm = false;
        stopAlarm = true;
        char path_char [1024];
        sprintf(path_char,"/alarms/%s",  curr_alarm.c_str());
        Serial.println("snooooooooooze is : ");
        Serial.print("path_char is : ");
        Serial.println(path_char);
        snoozeAlarmFunc(path_char);
     }

    updateStopAlarm();
}
