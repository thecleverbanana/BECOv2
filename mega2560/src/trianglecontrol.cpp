#include <Arduino.h>
#include "actuator.h"
#include "motor.h"

Actuator spine(4, A0);         // PWM pin: 4, Feedback pin: A0
Motor leg0(Serial1, 0);        // Motor ID 0
Motor leg1(Serial1, 1);        // Motor ID 1

const float spineMin = 0.0f;
const float spineMax = 127.0f;
const int legAngleMin = 0;
const int legAngleMax = 1800;

const int updateInterval = 5;
unsigned long lastUpdateTime = 0;
bool isRunning = false;

const float Maxfrequency = (255/33)*2; //Maximun frequency of the spine by measurement

const int N_Sample = 100;
int trajectoryIndex = 0;
float leg0Trajectory[N_Sample];
float leg1Trajectory[N_Sample];
float spineTrajectory[N_Sample];
void triangleTrajectory(float legAmplitude, float spineAmplitude,
                        float leg0_offset, float leg1_offset, float spine_offset);

void initialization();

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println("Mega2560 Triangle Wave Controller Ready.");
  Serial.println("Initializing...");
  initialization();

  Serial.println("Generate Triangle Trajectory for Legs and Spine.");
  triangleTrajectory((spineMax-spineMin), (legAngleMax-legAngleMin),900, 900, 0);
  Serial.println("Initialized. Press 's' to start.");
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

    float period_ms = 1000.0f / Maxfrequency;  // Period of one full cycle in ms
    float sampleDuration = period_ms / N_Sample; // Time per sample in ms
    static float accumulatedTime = 0;
    accumulatedTime += updateInterval;

    while (accumulatedTime >= sampleDuration) {
      // Apply trajectory to actuators
      leg0.sendAngle(leg0Trajectory[trajectoryIndex]*10);
      leg1.sendAngle(leg1Trajectory[trajectoryIndex]*10);
      spine.setPosition(spineTrajectory[trajectoryIndex]);

      // Advance the index
      trajectoryIndex = (trajectoryIndex + 1) % N_Sample;

      accumulatedTime -= sampleDuration;

      Serial.print(">");
      Serial.print("Leg0:");
      Serial.print(leg0Trajectory[trajectoryIndex]);
      Serial.print(",");

      Serial.print("Leg1:");
      Serial.print(leg1Trajectory[trajectoryIndex]);
      Serial.print(",");

      Serial.print("Spine:");
      Serial.print(spineTrajectory[trajectoryIndex]);
      Serial.println();
    }
  }
}

void initialization() {
  float initPos = spine.getPosition();
  Serial.print("Initializing actuator from position: ");
  Serial.println(initPos);
  spine.setPosition(0);

  Serial.println("Initializing leg0 and leg1 to mid angles...");
  leg0.sendAngle(900);
  leg1.sendAngle(900);

  delay(10000);
}

void triangleTrajectory(float legAmplitude, float spineAmplitude,
                        float leg0_offset, float leg1_offset, float spine_offset) {
    for (int i = 0; i < N_Sample; ++i) {
        float phase = (float)i / (float)(N_Sample - 1);  // [0, 1]
        float triangle = (phase < 0.5f)
                         ? (2.0f * phase)               // rising
                         : (2.0f * (1.0f - phase));     // falling

        // triangle ∈ [0,1], scale to amplitude and add offset
        leg0Trajectory[i] = triangle * 2.0f * legAmplitude ;
        leg1Trajectory[i] = triangle * 2.0f * legAmplitude ;
        spineTrajectory[i] = triangle * spineAmplitude + spine_offset;
    }
}

