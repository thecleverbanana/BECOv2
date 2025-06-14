#include "motor.h"

Motor::Motor(HardwareSerial& serialPort, int id)
    : serial(serialPort), motorID(id) {}

// angle command （e.g#1D0900）
void Motor::sendAngle(int angle) {
    char cmd[16];
    sprintf(cmd, "#%dD%04d", motorID, angle);
    serial.print(cmd);
    serial.print("\r");

    // Serial.print("[LSS] Sent: ");
    // Serial.println(cmd);
}

void Motor::sendRawCommand(const char* cmd) {
    serial.print(cmd);
    serial.print("\r");

    // Serial.print("[LSS] Sent: ");
    // Serial.println(cmd);
}
