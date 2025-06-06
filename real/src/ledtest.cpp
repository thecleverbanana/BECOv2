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

  Serial.println("Starting LED test on motor #1...");
}

void loop() {
  sendLSSCommand("#1LED3");  
  delay(1000);
  sendLSSCommand("#1LED1");
  delay(1000);
  sendLSSCommand("#1LED2");  
  delay(1000);
}
