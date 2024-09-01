#ifndef PARAMATERS_H
#define PARAMATERS_H

#include "libraries.h"


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

//Display Pin
#define CLK_PIN 14   
#define DATA_PIN 13  
#define CS_PIN 12     


// SD Card Pins
#define SD_CS_PIN     5
#define SPI_MOSI      23 
#define SPI_MISO      19 //
#define SPI_SCK       18


// Audio Pins
#define I2S_BCLK 26
#define I2S_LRC 25
#define I2S_DOUT 22

// Firebase project credentials
#define FIREBASE_HOST "https://totemic-client-604-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "AjB8B9574CjOJgW4zUvboupm5su0mJkJYQDWCQTO"

#define NUM_BYTES_TO_READ_FROM_FILE 1024   // How many bytes to read from wav file at a time

#endif