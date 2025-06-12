#include <Arduino.h>

const int actuatorPin = 4;      // PWM output
const int feedbackPin = A0;     // Analog feedback input
const int updateInterval = 5;   // ms
const int maxPWM = 127;         // Max PWM output (0–255)
float Kp = 0.2;                 // Proportional gain (tune as needed)

float frequency = 0.5;               // Hz
float omega = 2 * PI * frequency;    // rad/s
float phaseShift = 0.0;              // rad (e.g., PI/2 = 90° shift)

unsigned long lastUpdateTime = 0;
const int feedbackPin = A0;

void setup() {
  Serial.begin(115200);
  pinMode(feedbackPin, INPUT);
  Serial.println("Reading actuator feedback (A0)...");
}

void loop() {
  int raw = analogRead(feedbackPin);  // 0–1023
  float voltage = raw * (5.0 / 1023.0);  // Convert to volts (assuming 5V reference)

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.println(" V");

  delay(200);
}
