#include <Arduino.h>

/* SBUS data structure */
struct SbusData {
  uint16_t ch[16];
  bool lost_frame;
  bool failsafe;
};

/* SBUS data */
SbusData data;

/* Pin for ESC signal */
const int escPin1 = 13;
const int escPin2 = 12;
const int escPin3 = 14;
const int escPin4 = 27;
const int escPin5 = 26;
const int escPin6 = 25;

/* UART pin for receiving data from transmitter ESP32 */
#define UART_RX 18

/* Variables for motor control */
int throttleValue1 = 1500;  // Default neutral value
int throttleValue2 = 1500;
int throttleValue3 = 1500;
int throttleValue4 = 1500;
int throttleValue5 = 1500;
int throttleValue6 = 1500;

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  
  /* Begin UART communication with transmitter ESP32 */
  Serial1.begin(115200, SERIAL_8N1, UART_RX, -1);  // TX pin set to -1 as it's not used
  
  /* Configure ESC pins as PWM outputs */
  ledcAttachPin(escPin1, 0);  // Channel 0
  ledcAttachPin(escPin2, 1);  // Channel 1
  ledcAttachPin(escPin3, 2);  // Channel 2
  ledcAttachPin(escPin4, 3);  // Channel 3
  ledcAttachPin(escPin5, 4);  // Channel 4
  ledcAttachPin(escPin6, 5);  // Channel 5

  /* Setup PWM channels - 50Hz for standard ESCs */
  for(int i = 0; i < 6; i++) {
    ledcSetup(i, 50, 16);  // 50Hz, 16-bit resolution
  }
}

void loop() {
  if (Serial1.available() > 0 && Serial1.read() == 0xFF) {
    /* Read channel data */
    for (int i = 0; i < 16; i++) {
      data.ch[i] = (Serial1.read() << 8) | Serial1.read();
    }
    
    /* Read flags */
    uint8_t flags = Serial1.read();
    data.lost_frame = flags & 0x01;
    data.failsafe = (flags & 0x02) >> 1;
    
    /* Read end byte */
    if (Serial1.read() == 0xFE) {
      /* Get throttle values from SBUS Channels */
      throttleValue1 = data.ch[0];
      throttleValue2 = data.ch[1];
      throttleValue3 = data.ch[2];
      throttleValue4 = data.ch[3];
      throttleValue5 = data.ch[4];
      throttleValue6 = data.ch[5];
      
      /* Map throttle values to PWM signals for ESCs */
      int escSignals[6] = {
        map(throttleValue1, 172, 1811, 1000, 2000),
        map(throttleValue2, 172, 1811, 1000, 2000),
        map(throttleValue3, 172, 1811, 1000, 2000),
        map(throttleValue4, 172, 1811, 1000, 2000),
        map(throttleValue5, 172, 1811, 1000, 2000),
        map(throttleValue6, 172, 1811, 1000, 2000)
      };
      
      /* Send PWM signals to ESCs */
      for(int i = 0; i < 6; i++) {
        uint32_t dutyCycle = (escSignals[i] * 65535) / 20000;  // Convert to duty cycle
        ledcWrite(i, dutyCycle);  // Write to corresponding channel
      }
      
      /* Display channel values for debugging */
      for (int i = 0; i < 16; i++) {
        Serial.print(data.ch[i]);
        Serial.print("\t");
      }
      Serial.print(data.lost_frame);
      Serial.print("\t");
      Serial.println(data.failsafe);
    }
  }
}