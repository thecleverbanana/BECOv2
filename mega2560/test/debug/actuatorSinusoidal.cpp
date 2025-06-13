#include <Arduino.h>
#include "actuator.h"

const int updateInterval = 5; // ms
const float dt = updateInterval / 1000.0;

Actuator actuator(4, A0);  // pwmPin = 4, feedbackPin = A0

unsigned long lastUpdate = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Time(ms), Target, Actual, PWM, Frequency(Hz)");
}

void loop() {
    unsigned long now = millis();
    if (now - lastUpdate >= updateInterval) {
        lastUpdate = now;
        actuator.update(now, dt);
    }
}
