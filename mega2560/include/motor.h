#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(HardwareSerial& serialPort, int id);

    void sendAngle(int angle);                 
    void sendRawCommand(const char* cmd);      

private:
    HardwareSerial& serial;
    int motorID;
};

#endif
