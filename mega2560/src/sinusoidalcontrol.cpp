#include <Arduino.h>
#include "actuator.h"
#include "motor.h"

Actuator spine(4, A0);
Motor leg0(Serial1, 0);
Motor leg1(Serial1, 1);
 
const float spineMax = 127.0f;     // 0~255 as full length (10cm)
const int legAngleMin = 0;         // LSS minimum degree
const int legAngleMax = 1800;      // LSS maxium degree

const float frequency = 0.5f;      // Hz
const float omega = 2 * PI * frequency;

const int updateInterval = 5;
unsigned long lastUpdateTime = 0;
bool isRunning = false;   

void initialization();

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200); 
  
  Serial.println("Mega2560 Sinusoidal controller ready.");

  Serial.println("Initializing...");
  initialization();
  Serial.println("Initialized");
}

void loop(){
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 's' || c == 'S') {
      isRunning = !isRunning;
      if (isRunning) {
        Serial.println("Oscillator started.");
        lastUpdateTime = millis();
      } else {
        Serial.println("Oscillator stopped.");
      }
    }
  }

  if (!isRunning) return;

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;

    float t = currentMillis / 1000.0f;

    float spineTarget = 0.5f * spineMax * (1.0f - cos(omega * t));         // 0 ~ spineMax
    int leg0Target = (int)(0.5f * (legAngleMax - legAngleMin) * (1 - cos(omega * t)));  // 0~1800
    int leg1Target = (int)(0.5f * (legAngleMax - legAngleMin) * (1 + cos(omega * t)));  // 1800~0 

    spine.setPosition(spineTarget);
    leg0.sendAngle(leg0Target);
    leg1.sendAngle(leg1Target);
  }
}

void initialization(){
  Serial.println("Initializing actuator to mid position...");
  spine.setPosition(0, true);

  Serial.println("Initializing motor0 to 0 degree...");
  leg0.sendAngle(0);

  Serial.println("Initializing motor1 to 180 degree...");
  leg1.sendAngle(1800);

  delay(1000);
}

