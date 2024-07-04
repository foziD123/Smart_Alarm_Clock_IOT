#include <MD_MAX72xx.h>
#include <SPI.h>
#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>
#define MAX_DEVICES 4
#define CS_PIN 5
#define CLK_PIN 18
#define DATA_PIN 23

// I2S configuration constants
#define I2S_NUM         (0) // I2S port number
#define I2S_SAMPLE_RATE (44100)
#define I2S_BITS        (16)
#define I2S_CHANNELS    (2)

// Function to generate a sine wave audio signal
void generateSineWave(int16_t* buffer, int bufferSize, float frequency, float amplitude) {
    for (int i = 0; i < bufferSize; i += 2) {
        float sample = amplitude * sin(2 * M_PI * frequency * (i / 2) / I2S_SAMPLE_RATE);
        buffer[i] = buffer[i + 1] = (int16_t) sample;
    }
}
MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::FC16_HW, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void setup() {
  Serial.begin(115200);
  
  mx.begin();
  //mx.setIntensity(0); // Set brightness (0-15)
  mx.clear(); // Clear display
  
  Serial.println("Matrix test started.");
      // Initialize I2S configuration
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX), // Master transmitter
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // Interrupt level 1
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false  // Set to true if using APLL (ESP32-S2 specific)
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = 26,   // BCK pin
        .ws_io_num = 25,    // LRCK pin
        .data_out_num = 22, // DATA pin
        .data_in_num = I2S_PIN_NO_CHANGE // Not used
    };

    // Install and start I2S driver
    i2s_driver_install((i2s_port_t)I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin((i2s_port_t)I2S_NUM, &pin_config);

    // Prepare a buffer to store audio data
    const int bufferSize = 1024;
    int16_t buffer[bufferSize];
    size_t bytes_written = 0;

    // Generate and play A4 (440 Hz) tone
    generateSineWave(buffer, bufferSize, 440.0, 1000); // A4
    i2s_write((i2s_port_t)I2S_NUM, buffer, bufferSize * sizeof(int16_t), &bytes_written, portMAX_DELAY);

    delay(1000); // 1 second delay

    // Generate and play C5 (523.25 Hz) tone with lower amplitude
    generateSineWave(buffer, bufferSize, 523.25, 800); // C5
    i2s_write((i2s_port_t)I2S_NUM, buffer, bufferSize * sizeof(int16_t), &bytes_written, portMAX_DELAY);

    delay(1000); // 1 second delay

    // Generate and play G4 (392 Hz) tone
    generateSineWave(buffer, bufferSize, 392.0, 1200); // G4
    i2s_write((i2s_port_t)I2S_NUM, buffer, bufferSize * sizeof(int16_t), &bytes_written, portMAX_DELAY);
}

void loop() { // loop needs to be fixed 
  for (int i = 0; i < MAX_DEVICES; i++) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        int position = i * 8 + col;
        mx.setPoint(row, position, true); // Turn on LED at position `position` on matrix `i`
        delay(50);
      }
    }
    delay(500);
    mx.clear(i); // Clear display for the next matrix
  }
}
