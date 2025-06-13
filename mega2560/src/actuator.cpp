#include "actuator.h"

Actuator::Actuator(int pwmPin, int feedbackPin, float kp, float kd,
                   int pwmMax)
    : pwmPin(pwmPin), feedbackPin(feedbackPin),
      kp(kp), kd(kd), pwmMax(pwmMax)
{
    pinMode(feedbackPin, INPUT);
    pinMode(pwmPin, OUTPUT);
}

void Actuator::setPosition(float pos) {
    pos = constrain(pos, 0, pwmMax);
    int pwmVal = map(pos, 0, pwmMax, 0, 255);  
    analogWrite(pwmPin, pwmVal);
}

float Actuator::getVoltage() {
    int raw = analogRead(feedbackPin);
    return raw * (vRef / adcMax);
}

float Actuator::getPosition() {
    float v = getVoltage();
    v = constrain(v, vMin, vMax);
    return (1-(v - vMin) / (vMax - vMin)) * pwmMax; // position needs to be reversed
}

void Actuator::setvMin() {
    vMin = getVoltage();
}

void Actuator::setvMax() {
    vMax = getVoltage();
}

float Actuator::getvMin() {
    return vMin;
}

float Actuator::getvMax() {
    return vMax;
}