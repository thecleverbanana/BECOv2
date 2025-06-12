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

int getSinusoidalTarget(float t) {
  return (int)(900 * (1.0 - cos(omega * t)));  // 0–1800
}

int readPosition() {
  int raw = analogRead(feedbackPin);  // 0–1023 from 0–3.3V
  return map(raw, 0, 675, 0, 1800);   // Map to 0–1800 (adjust 675 if needed)
}

void waitForFrequencyInput() {
  Serial.println("Enter frequency in Hz (default 0.5 Hz). Press ENTER to use default:");
  String input = "";
  unsigned long startTime = millis();
  while (millis() - startTime < 10000) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') break;
      input += c;
    }
  }
  if (input.length() > 0) {
    frequency = input.toFloat();
    if (frequency <= 0) frequency = 0.5;
  }
  omega = 2 * PI * frequency;
  Serial.print("Using frequency: ");
  Serial.print(frequency, 3);
  Serial.println(" Hz");
}

void setup() {
  Serial.begin(115200);
  pinMode(actuatorPin, OUTPUT);
  waitForFrequencyInput();
  Serial.println("Time(ms), Target, Actual, Error, PWM");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;

    float period = 1.0 / frequency;
    float t = fmod(currentMillis / 1000.0 + phaseShift / omega, period);

    int target = getSinusoidalTarget(t);
    int actual = readPosition();
    int error = target - actual;
    int pwm = constrain(Kp * error, 0, maxPWM);  // One-directional actuator (extend only)

    analogWrite(actuatorPin, pwm);

    // Serial plot output
    Serial.print(">");
    Serial.print("currentMillis:");Serial.print(currentMillis); Serial.print(", ");
    Serial.print("target:");Serial.print(target); Serial.print(", ");
    Serial.print("pwm:");Serial.println(pwm);
    Serial.println();
  }
}
