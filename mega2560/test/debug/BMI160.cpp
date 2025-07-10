#include <Wire.h>
#include <Arduino.h>
#include <DFRobot_BMI160.h>

DFRobot_BMI160 bmi160;
const uint8_t I2C_ADDR = 0x69;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  if (bmi160.softReset() != BMI160_OK) {
    Serial.println("Reset failed"); while (1);
  }
  if (bmi160.I2cInit(I2C_ADDR) != BMI160_OK) {
    Serial.println("Init failed"); while (1);
  }
  Serial.println("BMI160 ready (DFRobot driver)");
}

const float ACC_LSB_PER_G     = 16384.0;  // For ±2g
const float GYRO_LSB_PER_DPS  = 16.4;     // For ±2000 °/s

void loop() {
  int16_t data[6] = {0};  // Gx, Gy, Gz, Ax, Ay, Az

  if (bmi160.getAccelGyroData(data) == BMI160_OK) {
    float gx = data[0] / GYRO_LSB_PER_DPS;
    float gy = data[1] / GYRO_LSB_PER_DPS;
    float gz = data[2] / GYRO_LSB_PER_DPS;

    float ax = data[3] / ACC_LSB_PER_G;
    float ay = data[4] / ACC_LSB_PER_G;
    float az = data[5] / ACC_LSB_PER_G;

    Serial.print("Gyro [°/s]: ");
    Serial.print(gx, 2); Serial.print(" ");
    Serial.print(gy, 2); Serial.print(" ");
    Serial.print(gz, 2); Serial.print(" | Accel [g]: ");
    Serial.print(ax, 3); Serial.print(" ");
    Serial.print(ay, 3); Serial.print(" ");
    Serial.println(az, 3);
  }

  delay(500);
}

