#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>

class Actuator {
public:
    Actuator(int pwmPin, int feedbackPin,
             float kp = 0.3, float kd = 0.01,
             int pwmMax = 225
        );

    void setPosition(float pos);
    float getVoltage();
    float getPosition();

    void setvMin();              
    void setvMax();
    float getvMin();              
    float getvMax();

private:
    int pwmPin, feedbackPin;
    int pwmMax, rawMax, posMax;
    float kp, kd;
    float frequency;      // Hz
    float omega;          // 2πf

    const float vRef = 3.3;
    const int   adcMax = 1023;
    float vMax = 2.2;
    float vMin = 0.0;
};

#endif
