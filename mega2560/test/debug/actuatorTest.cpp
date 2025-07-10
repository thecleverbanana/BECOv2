
#include "actuator.h"
#include <Arduino.h>

Actuator actuator(4, A0);  // pwmPin = 4, feedbackPin = A0


void setup() {
  actuator.setPosition(0);
}

void loop() {
}
