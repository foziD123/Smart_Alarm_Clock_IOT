# Smart Alarm Clock

This project is part of the ICST Lab at the Technion Institute of Technology. It is a comprehensive smart alarm clock system that includes an Arduino-based ESP32 microcontroller and a companion Android app developed using Dart and Flutter.

## Features

### ESP32 Microcontroller
- **Alarm Management:** ESP32 handles alarm settings, snooze functionality, and synchronization with Firebase.
- **Task Management:** Tasks are fetched from Firebase, sorted by priority, and managed on the device.
- **Display Control:** Includes features like brightness control and dynamic display toggling between time, date, and motivational quotes.
- **Speech Output:** The ESP32 can read out tasks for the day using a text-to-speech library.
- **Connectivity Monitoring:** The ESP32 constantly syncs with Firebase and reports its connection status.
- **Button Control:** Physical buttons on the device control snooze, alarm stop, display toggling, and task reading.

### Companion Android App
- **User Interface:** Developed using Dart and Flutter, providing a sleek and intuitive interface for managing alarms and tasks.
- **Alarm and Task Management:** Users can set, edit, and delete alarms and tasks. Tasks are assigned priorities (High, Medium, Low) and can be viewed sorted by date or priority.
- **Settings Control:** The app allows users to adjust the brightness and volume of the ESP32 display, as well as set the snooze time.
- **Synchronization:** The app ensures that all settings and tasks are synced with Firebase, keeping the ESP32 device up to date.

## Technologies and Libraries Used

### ESP32 Microcontroller
- **Programming Language:** C++
- **Firebase Communication:** Used to synchronize data between the ESP32 and Firebase.
- **Time Management:** Libraries used to handle alarms and scheduling.
- **Text-to-Speech:** For reading out tasks and notifications.
- **Display Control:** Manages the display of time, date, quotes, and other information on the device.

### Android App
- **Programming Language:** Dart
- **Framework:** Flutter
- **Firebase Integration:** For real-time database management and synchronization with the ESP32.

## Installation and Setup

### ESP32
1. Clone the repository.
2. Open the Arduino IDE and load the `.ino` file for the ESP32.
3. Install the necessary libraries (Firebase, WiFi, Time, etc.).
4. Upload the code to your ESP32 device.

### Android App
1. Clone the repository.
2. Open the project in Android Studio.
3. Run the app on an emulator or physical device.
