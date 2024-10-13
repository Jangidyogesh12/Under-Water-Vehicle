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
const int escPin = 13;

/* UART pin for receiving data from transmitter ESP32 */
#define UART_RX 18

/* Variables for motor control */
int throttleValue = 1500;  // Default neutral value

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  
  /* Begin UART communication with transmitter ESP32 */
  Serial1.begin(115200, SERIAL_8N1, UART_RX, -1);  // TX pin set to -1 as it's not used
  
  /* Configure ESC pin as PWM output */
  ledcAttachPin(escPin, 0);  // Attach PWM channel 0 to the pin
  ledcSetup(0, 50, 16);      // PWM frequency: 50Hz, resolution: 16-bit
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
      /* Get throttle value from SBUS Channel 2 (index 1) */
      throttleValue = data.ch[1];
      
      /* Map throttle value to PWM signal for ESC */
      int escSignal = map(throttleValue, 172, 1811, 1000, 2000);
      
      /* Send the PWM signal to the ESC */
      uint32_t dutyCycle = (escSignal * 65535) / 20000;  // Convert to duty cycle
      ledcWrite(0, dutyCycle);
      
      /* Display the channel values */
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