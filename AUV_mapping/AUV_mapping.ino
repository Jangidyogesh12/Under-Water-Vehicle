#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float velocityX = 0, velocityY = 0, velocityZ = 0;
float distanceX = 0, distanceY = 0, distanceZ = 0;
unsigned long lastTime = 0;
const float GRAVITY = 9.81; // m/s^2

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 ready");
  lastTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // Time difference in seconds
  lastTime = currentTime;

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Subtract gravity from Z-axis and apply a simple threshold
  float aX = a.acceleration.x + 0.95 ;
  float aY = a.acceleration.y;
  float aZ = a.acceleration.z - GRAVITY;

  // Apply a threshold to filter out small vibrations
  const float THRESHOLD = 0.1;
  aX = (abs(aX) > THRESHOLD) ? aX : 0;
  aY = (abs(aY) > THRESHOLD) ? aY : 0;
  aZ = (abs(aZ) > THRESHOLD) ? aZ : 0;

  // Update velocities
  velocityX += aX * dt;
  velocityY += aY * dt;
  velocityZ += aZ * dt;

  // Update distances
  distanceX += velocityX * dt + 0.5 * aX * dt * dt;
  distanceY += velocityY * dt + 0.5 * aY * dt * dt;
  distanceZ += velocityZ * dt + 0.5 * aZ * dt * dt;

  // Print results
  Serial.print("Acceleration (m/s^2) X: ");
  Serial.print(aX);
  Serial.print(", Y: ");
  Serial.print(aY);
  Serial.print(", Z: ");
  Serial.println(aZ);

  Serial.print("Velocity (m/s) X: ");
  Serial.print(velocityX);
  Serial.print(", Y: ");
  Serial.print(velocityY);
  Serial.print(", Z: ");
  Serial.println(velocityZ);

  Serial.print("Distance (m) X: ");
  Serial.print(distanceX);
  Serial.print(", Y: ");
  Serial.print(distanceY);
  Serial.print(", Z: ");
  Serial.println(distanceZ);

  Serial.println();
  delay(1000); // Reduced delay for more frequent updates
}