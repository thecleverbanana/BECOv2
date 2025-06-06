#include <Arduino.h>

const int actuatorPin = 4;  // PWM for shaft actuator
int shaft_pwm = 0;

void sendLSSCommand(const char* cmd) {
    Serial1.print(cmd);
    Serial1.print("\r");  // LSS commands require carriage return
    Serial.print("[LSS] Sent: ");
    Serial.println(cmd);
  }

void setup() {
  Serial.begin(115200);       // USB serial monitor
  Serial1.begin(115200);      // UART1 → LSS
  pinMode(actuatorPin, OUTPUT);

  Serial.println("Mega2560 LSS phase controller ready.");
  Serial.println("Initializing motors to 45 degrees...");

  // 初始角度设定为 45 度
  sendLSSCommand("#0D0450");  // 45.0°
  sendLSSCommand("#1D0450");  // 45.0°
  delay(500);
  Serial.println("Type 0~4 to trigger phase:");
}


void execute_phase(int phase) {
  switch (phase) {
    case 0:
      Serial.println("Phase 0: Reset both motors to 45°");
      sendLSSCommand("#0D0450");
      sendLSSCommand("#1D0450");
      break;
    case 1:
      Serial.println("Phase 1: Motor0 -> 0°, Motor1 -> 90°");
      sendLSSCommand("#0D0000");
      sendLSSCommand("#1D0900");
      break;
    case 2:
      Serial.println("Phase 2: Extend shaft");
      shaft_pwm = 255;
      break;
    case 3:
      Serial.println("Phase 3: Motor0 -> 90°, Motor1 -> 0°");
      sendLSSCommand("#0D0900");
      sendLSSCommand("#1D0000");
      break;
    case 4:
      Serial.println("Phase 4: Retract shaft");
      shaft_pwm = 0;
      break;
    default:
      Serial.println("Invalid phase");
      return;
  }

  analogWrite(actuatorPin, shaft_pwm);
  Serial.print("PWM set to: ");
  Serial.println(shaft_pwm);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c >= '0' && c <= '4') {
      int phase = c - '0';
      execute_phase(phase);
    }
  }
}
