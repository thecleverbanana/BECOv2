#include <Arduino.h>
#include <DFRobot_BMI160.h>
#include "actuator.h"
#include "motor.h"

Actuator spine(4, A0);         // PWM pin: 4, Feedback pin: A0
Motor leg0(Serial1, 0);        // Motor ID 0
Motor leg1(Serial1, 1);        // Motor ID 1

DFRobot_BMI160 bmi160;
const uint8_t I2C_ADDR = 0x69;  // IMU Address

const float spineMin = 107.1f;
const float spineMax = 251.9f;
const int legAngleMin = 0;
const int legAngleMax = 1800;

const int Period = 15; 
//notes: the minimum period for full extension is 30s
//futhur than that, the linear actuator wouldn't be able to catch
const float omega = 2*PI/Period;
const float offset =  0 ;
const int N_Sample = 300;
float t_k[N_Sample];
float leg0AngleTrajectory[N_Sample];
float leg1AngleTrajectory[N_Sample];
float spinePWMTrajectory[N_Sample];

// IMU setup
const float ACC_LSB_PER_G     = 16384.0;  // For ±2g
const float GYRO_LSB_PER_DPS  = 16.4;     // For ±2000 °/s
int16_t imu_data[6] = {0};  // Gx, Gy, Gz, Ax, Ay, Az
float beta_acc = 0.0f;;
float beta = 0.0f;  
unsigned long lastMicros = 0;

unsigned long startTime = 0;
bool isRunning = false;
// refreshing frequency control (5ms, 200Hz)
const int updateIntervalMs = 5;
unsigned long lastUpdateTime = 0;

/// @brief  The detailed formula can be found in analysis sinusoidal_wave_control.ipynb
void sinusoidal_trajectory_generator() {
  for (int i = 0; i < N_Sample; i++) {
    float t = (float)i / N_Sample * Period;
    t_k[i] = t;

    float pwm = ((spineMax - spineMin) / 2.0) * cos(omega * t + offset)
                + ((spineMax + spineMin) / 2.0);

    spinePWMTrajectory[i] = pwm;

    float theta0 = acos(cos(omega * t ));
    float theta1 = acos(cos(omega * t + PI));
    float leg0 = theta0 / PI * (legAngleMax-legAngleMin);
    float leg1 = theta1 / PI * (legAngleMax-legAngleMin);
    leg0AngleTrajectory[i] = leg0;
    leg1AngleTrajectory[i] = leg1;
  }
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println("Mega2560 Triangle Wave Controller Ready.");

  Serial.println("Generating Sinusoidal Trajectory for Legs and Spine...");
  sinusoidal_trajectory_generator();
  Serial.println("Sinusoidal Trajectory Generated");

  Serial.println("Initializing Spine and Leg to starting position");
  spine.setPosition(spinePWMTrajectory[0]);
  leg0.sendAngle(leg0AngleTrajectory[0]);
  leg1.sendAngle(leg1AngleTrajectory[0]);
  delay(5000);

  if (bmi160.softReset() != BMI160_OK) {
    Serial.println("Reset failed"); while (1);
  }
  if (bmi160.I2cInit(I2C_ADDR) != BMI160_OK) {
    Serial.println("Init failed"); while (1);
  }
  lastMicros = micros();
  Serial.println("BMI160 ready (DFRobot driver)");

  Serial.println("Initialized. Press 's' to start.");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 's' || c == 'S') {
      isRunning = !isRunning;
      if (isRunning) {
        Serial.println("Oscillator started.");
        startTime = millis();  // record start time for each loop
        lastUpdateTime = millis();
      } else {
        Serial.println("Oscillator stopped.");
      }
    }
  }

  if (!isRunning) return;

  unsigned long currentMillis = millis();

  // refreshing frequency control
  if(currentMillis - lastUpdateTime < updateIntervalMs) return;
  lastUpdateTime = currentMillis;

  float elapsed = (currentMillis - startTime) / 1000.0;  // convert to second
  float t_now = fmod(elapsed, Period);  // find t in period [0, Period]
  int index = (int)(t_now / Period * N_Sample);
  index = constrain(index, 0, N_Sample - 1);  // find closest t_k

  spine.setPosition(spinePWMTrajectory[index]);
  leg0.sendAngle(leg0AngleTrajectory[index]);
  leg1.sendAngle(leg1AngleTrajectory[index]);

  if (bmi160.getAccelGyroData(imu_data) == BMI160_OK){
    beta_acc = imu_data[0] / GYRO_LSB_PER_DPS;

    unsigned long nowMicros = micros();
    float dt = (nowMicros - lastMicros) * 1e-6f;
    lastMicros = nowMicros;

    beta += beta_acc * dt;
  }

  // Serial.print(">");
  // Serial.print("Leg0:");
  Serial.print(leg0AngleTrajectory[index]);
  Serial.print(",");

  // Serial.print("Leg1:");
  Serial.print(leg1AngleTrajectory[index]);
  Serial.print(",");

  // Serial.print("Spine:");
  Serial.print(spinePWMTrajectory[index]);
  Serial.print(",");
  
  // Serial.print("Beta Acc [°/s]: ");
  Serial.print(beta_acc, 2);
  Serial.print(",");

  // Serial.print("Beta [°]: ");
  Serial.print(beta, 2);
  Serial.println();
}





