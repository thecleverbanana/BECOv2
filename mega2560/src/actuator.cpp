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
    analogWrite(pwmPin, pos);
}

void Actuator::setPosition(float pos, bool closedLoop) {
    pos = constrain(pos, 0, pwmMax);

    if (!closedLoop) {
        int pwmVal = map(pos, 0, pwmMax, 0, 255);  
        analogWrite(pwmPin, pwmVal);
        return;
    }

    const float tolerance = 5.0f;
    const unsigned long timeout = 5000;
    unsigned long startTime = millis();

    while (millis() - startTime < timeout) {
        float currentPos = getPosition();
        float error = pos - currentPos;

        if (abs(error) <= tolerance) {
            analogWrite(pwmPin, 0); 
            break;
        }

        int pwmVal = constrain(kp * error, 0, 255);
        analogWrite(pwmPin, pwmVal);
        delay(5);
    }
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