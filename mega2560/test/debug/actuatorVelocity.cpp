#include "actuator.h"
#include <Arduino.h>

Actuator actuator(4, A0);  // pwmPin = 4, feedbackPin = A0

const unsigned long interval = 5; // ms
unsigned long lastTime = 0;

const float dt = interval / 1000.0f;
float lastPosition = 0;

const int windowSize = 100;
float velocityWindow[windowSize] = {0};
int velocityIndex = 0;

void setup() {
  Serial.begin(115200);

  Serial.print("Calibrating: ");
  actuator.setPosition(255);
  delay(10000);
  actuator.setvMin();

  actuator.setPosition(0);
  delay(10000);
  actuator.setvMax();

  Serial.print("Max position voltage: ");
  Serial.print(actuator.getvMax(), 3);
  Serial.print("Min position voltage: ");
  Serial.println(actuator.getvMin(), 3);

  actuator.setPosition(255);
}

void loop() {
  unsigned long now = millis();
  if (now - lastTime >= interval) {
    lastTime = now;

    float currentPosition = actuator.getPosition();
    float velocity = (currentPosition - lastPosition) / dt;

    velocityWindow[velocityIndex] = velocity;
    velocityIndex = (velocityIndex + 1) % windowSize;

    float avgVelocity = 0;
    for (int i = 0; i < windowSize; ++i)
      avgVelocity += velocityWindow[i];
    avgVelocity /= windowSize;

    if (currentPosition >= 50 && currentPosition <= 200) {
      Serial.print("Time: "); Serial.print(now);
      Serial.print(" ms | Position: "); Serial.print(currentPosition, 2);
      Serial.print(" | Velocity: "); Serial.print(velocity, 2);
      Serial.print(" | AvgVelocity: "); Serial.println(avgVelocity, 2);
    }

    lastPosition = currentPosition;
  }
}
