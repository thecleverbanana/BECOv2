#include <Arduino.h>

void sendLSSCommand(const char* cmd) {
  Serial1.print(cmd);
  Serial1.print("\r");  
  Serial.print("[LSS] Sent: ");
  Serial.println(cmd);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(1000);
  Serial.println("=== LSS Motor Test Ready ===");
  Serial.println("Commands:");
  Serial.println("d = Move to 90°");
  Serial.println("m = Move +45°");
  Serial.println("t = Move to 90° in 0.5s");
  Serial.println("x = Move +45° in 0.5s");
  Serial.println("r = RESET");
  Serial.println("c = CLEAR all limits");
  Serial.println("l = LED blue");
  Serial.println("s = Query status");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case 'd': sendLSSCommand("#1D900"); break;
      case 'm': sendLSSCommand("#1MD0450"); break;
      case 't': sendLSSCommand("#1T09000500"); break;
      case 'x': sendLSSCommand("#1MT04500500"); break;
      case 'r': sendLSSCommand("#1RESET"); break;
      case 'c': sendLSSCommand("#1CLEAR"); break;
      case 'l': sendLSSCommand("#1LED3"); break;
      case 's': sendLSSCommand("#1Q"); break;
      default: Serial.println("Unknown command."); break;
    }
  }

  // Print back any response from LSS
  while (Serial1.available()) {
    byte c = Serial1.read();
    Serial.print(" ");
    Serial.print(c, HEX);  
  }  

  delay(10);
}
