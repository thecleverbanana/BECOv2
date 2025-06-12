#include <Arduino.h>

const int actuatorPin = 4;  // PWM for shaft actuator
int shaft_pwm = 0;

const int updateInterval = 5; 
const int motor0_id = 0;
const int motor1_id = 1;

unsigned long lastUpdateTime = 0;
bool isRunning = false;
float frequency = 0.1;          
float omega = 2 * PI * frequency;      

float phaseShift = PI / 2;
float spineLength = 127;    // 0~255 as full length (10cm)

void sendLSSCommand(int motorID, int angle) {
  char cmd[16];
  sprintf(cmd, "#%dD%04d", motorID, angle);
  Serial1.print(cmd);
  Serial1.print("\r");

  Serial.print("[LSS] Sent: ");
  Serial.println(cmd);
}

void sendLSSCommand(const char* cmd) {
    Serial1.print(cmd);
    Serial1.print("\r");  // LSS commands require carriage return
    Serial.print("[LSS] Sent: ");
    Serial.println(cmd);
  }


int getTargetAngle(float t) {
  float angle = 900 * (1.0 - cos(omega * t));  // 0 ~ 1800
  return (int)angle;
}

void waitForFrequencyInput() {
  Serial.println("Enter frequency in Hz (default 0.1 Hz). Press ENTER to use default:");

  String input = "";
  unsigned long startTime = millis();
  while (millis() - startTime < 10000) {  
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') {
        break;
      } else {
        input += c;
      }
    }
  }

  if (input.length() > 0) {
    frequency = input.toFloat();
    if (frequency <= 0) {
      frequency = 0.5;
      Serial.println("Invalid input. Using default 0.5 Hz.");
    }
  } else {
    Serial.println("No input. Using default 0.5 Hz.");
  }

  omega = 2 * PI * frequency;
  Serial.print("Frequency set to: ");
  Serial.print(frequency, 3);
  Serial.print(" Hz → Omega = ");
  Serial.println(omega, 5);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(actuatorPin, OUTPUT);
  delay(1000);

  Serial.println("Resetting Motor Position: Motor0 -> 0°, Motor1 -> 180°, Linear -> 0");
  sendLSSCommand("#0D0000");
  sendLSSCommand("#1D01800");
  analogWrite(actuatorPin, shaft_pwm);

  Serial.println("Oscillator open-loop motor controller ready.");
  waitForFrequencyInput();
  Serial.println("Type 's' to start oscillator.");
}

void loop() {
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

    float period = 1.0 / frequency;          // T
    float t = fmod(currentMillis / 1000.0, period);
    int angle = getTargetAngle(t);

    float t_spine = fmod(t + phaseShift / omega, period);  // wrapped phase-shifted time
    int spineAngle = getTargetAngle(t_spine);

    sendLSSCommand(motor0_id, angle);
    sendLSSCommand(motor1_id, 1800 - angle);
    int spine_pwm_value = map(spineAngle, 0, 1800, 0, spineLength);
    analogWrite(actuatorPin, spine_pwm_value);

  }
}
