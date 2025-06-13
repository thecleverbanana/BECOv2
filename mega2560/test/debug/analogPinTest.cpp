#include <Arduino.h>

const int feedbackPin = A0;
const float vRef = 3.3;  // ADC reference voltage is 3.3V
const int adcMax = 1023;

void setup() {
  Serial.begin(115200);
  pinMode(feedbackPin, INPUT);
  Serial.println("Reading actuator feedback (0–3.3V)...");
}

void loop() {
  int raw = analogRead(feedbackPin);
  float voltage = raw * (vRef / adcMax);

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.println(" V");

  delay(200);
}
