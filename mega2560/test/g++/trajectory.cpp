#include <iostream>
#include <cmath>

const float PI = 3.14159265358979f;
const float spineMin = 0.0f;
const float spineMax = 127.0f;
const int legAngleMin = 0;
const int legAngleMax = 1800;

const int Period = 30;
const float omega = 2 * PI / Period;
const float offset = PI / 2;
const int N_Sample = 100;

float t_k[N_Sample];
float leg0AngleTrajectory[N_Sample];
float leg1AngleTrajectory[N_Sample];
float spinePWMTrajectory[N_Sample];

void sinusoidal_trajectory_generator() {
  for (int i = 0; i < N_Sample; i++) {
    float t = (float)i / N_Sample * Period;
    t_k[i] = t;

    float pwm = ((spineMax - spineMin) / 2.0f) * sin(omega * t) + ((spineMax + spineMin) / 2.0f);
    spinePWMTrajectory[i] = pwm;

    float theta0 = acos(sin(omega * t - offset));
    float theta1 = acos(sin(omega * t - offset + PI));
    float leg0 = theta0 / PI * (legAngleMax - legAngleMin);
    float leg1 = theta1 / PI * (legAngleMax - legAngleMin);
    leg0AngleTrajectory[i] = leg0;
    leg1AngleTrajectory[i] = leg1;
  }
}

void print_trajectory() {
  std::cout << "t_k\tspinePWM\tleg0\tleg1\n";
  for (int i = 0; i < N_Sample; i++) {
    std::cout << t_k[i] << "\t"
              << spinePWMTrajectory[i] << "\t"
              << leg0AngleTrajectory[i] << "\t"
              << leg1AngleTrajectory[i] << "\n";
  }
}

int main() {
  sinusoidal_trajectory_generator();
  print_trajectory();
  return 0;
}
