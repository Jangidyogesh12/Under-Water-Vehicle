#include "sbus.h"
#include <Arduino.h>

// SBUS object, reading SBUS
// Using GPIO 16 as RX pin for SBUS
bfs::SbusRx sbus_rx(&Serial2, 16, 17, true, false);

// SBUS data
bfs::SbusData data;

// UART pin for communication with the receiver ESP32
#define UART_TX 18  // Only TX pin is needed for sending data

void setup() {
  // Serial for debugging
  Serial.begin(115200);
  
  // Begin SBUS communication
  sbus_rx.Begin();
  
  // Begin UART communication with receiver ESP32
  Serial1.begin(115200, SERIAL_8N1, -1, UART_TX);  // RX pin set to -1 as it's not used
}

void loop() {
  if (sbus_rx.Read()) {
    // Grab the received SBUS data
    data = sbus_rx.data();
    
    // Send start byte
    Serial1.write(0xFF);
    
    // Send channel data
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      Serial1.write((data.ch[i] >> 8) & 0xFF);  // High byte
      Serial1.write(data.ch[i] & 0xFF);         // Low byte
    }
    
    // Send flags
    uint8_t flags = (data.lost_frame ? 0x01 : 0x00) | (data.failsafe ? 0x02 : 0x00);
    Serial1.write(flags);
    
    // Send end byte
    Serial1.write(0xFE);
    
    // Debug output
    Serial.println("SBUS data sent");
  }
}