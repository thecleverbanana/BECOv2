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

  Serial.println("Starting LSS Move Test on motor #1...");
  delay(2000);
}

void loop() {
  sendLSSCommand("#1MD0900"); 
  delay(2000);

  sendLSSCommand("#1MD-0900");
  delay(2000);
}
