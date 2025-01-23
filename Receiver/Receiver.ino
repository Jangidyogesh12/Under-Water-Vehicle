#include <ESP32Servo.h>

/* SBUS data structure */
struct SbusData {
  uint16_t ch[16];
  bool lost_frame;
  bool failsafe;
};

/* SBUS data */
SbusData data;

/* Create servo objects */
Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;
Servo esc5;
Servo esc6;
Servo esc7;
Servo esc8;
/* Pin for ESC signal */
const int escPin1 = 13;
const int escPin2 = 12;
const int escPin3 = 14;
const int escPin4 = 27;
const int escPin5 = 26;
const int escPin6 = 25;
const int escPin7 = 33;
const int escPin8 = 32;

/* UART pin for receiving data from transmitter ESP32 */
#define UART_RX 18

/* Variables for motor control */
int throttleValue1 = 1500;  // Default neutral value
int throttleValue2 = 1500;
int throttleValue3 = 1500;
int throttleValue4 = 1500;
int throttleValue5 = 1500;
int throttleValue6 = 1500;
int throttleValue7 = 1500;
int throttleValue8 = 1500;

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  
  /* Begin UART communication with transmitter ESP32 */
  Serial1.begin(115200, SERIAL_8N1, UART_RX, -1);
  
  /* Allow allocation of all timers for servo */
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  /* Attach servos to pins */
  esc1.attach(escPin1, 1000, 2000);
  esc2.attach(escPin2, 1000, 2000);
  esc3.attach(escPin3, 1000, 2000);
  esc4.attach(escPin4, 1000, 2000);
  esc5.attach(escPin5, 1000, 2000);
  esc6.attach(escPin6, 1000, 2000);
  esc7.attach(escPin7, 1000, 2000);
  esc8.attach(escPin8, 1000, 2000);
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
      /* Check if channel 8 has a significant signal */
      if (data.ch[7] != 1500) {  // Channel 8 (index 7) is not at neutral
        /* Apply channel 8 value to ESC 1 and 2 */
        throttleValue1 = data.ch[7];
        throttleValue2 = data.ch[7];
        throttleValue3 = data.ch[7];
        throttleValue4 = data.ch[7];
      } 
      
      /* Get throttle values for other ESCs from their respective channels */
      throttleValue5 = data.ch[0];
      throttleValue6 = data.ch[1];
      throttleValue7 = data.ch[2];
      throttleValue8 = data.ch[3];
      
      /* Map throttle values to PWM signals for ESCs */
      int escSignals[8] = {
        map(throttleValue1, 172, 1811, 1000, 2000),
        map(throttleValue2, 172, 1811, 1000, 2000),
        map(throttleValue3, 172, 1811, 1000, 2000),
        map(throttleValue4, 172, 1811, 1000, 2000),
        map(throttleValue5, 172, 1811, 1000, 2000),
        map(throttleValue6, 172, 1811, 1000, 2000),
        map(throttleValue7, 172, 1811, 1000, 2000),
        map(throttleValue8, 172, 1811, 1000, 2000),
      };
      
      /* Write values to ESCs */
      esc1.writeMicroseconds(escSignals[0]);
      esc2.writeMicroseconds(escSignals[1]);
      esc3.writeMicroseconds(escSignals[2]);
      esc4.writeMicroseconds(escSignals[3]);
      esc5.writeMicroseconds(escSignals[4]);
      esc6.writeMicroseconds(escSignals[5]);
      esc7.writeMicroseconds(escSignals[6]);
      esc8.writeMicroseconds(escSignals[7]);
      
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