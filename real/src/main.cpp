#include <LSS.h>
#include <EEPROM.h>

const int pwmPin = 9;  // Linear actuator PWM control pin

// LSS Motors
LSS motor1(0); // LSS ID = 0
LSS motor2(1); // LSS ID = 1

int phase = 0;  // Current movement phase
const int stepSize = 1800;  // Step-wise movement in degrees
unsigned long lastMoveTime = 0;
const int moveDelay = 8000;  // 8-second delay per phase
const int actuatorDelay = 500;  // 1-second delay between motor and actuator

// EEPROM Memory Addresses
#define EEPROM_PHASE_ADDR  0  // Address to store phase
#define EEPROM_TIME_ADDR   10 // Address to store lastMoveTime

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }  // Wait for Serial connection

    Serial.println("Arduino Started");
    pinMode(pwmPin, OUTPUT);  // Set PWM pin as output

    // Initialize LSS serial communication
    LSS::initBus(Serial, 115200);
    delay(1000);

    // Read last phase from EEPROM
    EEPROM.get(EEPROM_PHASE_ADDR, phase);
    EEPROM.get(EEPROM_TIME_ADDR, lastMoveTime);

    // Ensure phase is within valid range (0, 1, 2, 3)
    if (phase < 0 || phase > 3) {
        phase = 0; // Reset if invalid
    }

    Serial.print("Resuming from phase ");
    Serial.println(phase + 1);

    lastMoveTime = millis(); // Restart timer
}

void loop() {
    unsigned long currentTime = millis();

    if (currentTime - lastMoveTime >= moveDelay) {
        Serial.print("Phase ");
        Serial.print(phase + 1);
        Serial.println(": Executing action");

        switch (phase) {
            case 0:
                analogWrite(pwmPin, 0);  // Actuator at 100%
                Serial.println("Actuator set to 100% position");
                break;

            case 1:
                motor1.moveRelative(stepSize);
                motor2.moveRelative(stepSize);
                Serial.println("Motors rotated 1800 degrees");
                break;

            case 2:
                analogWrite(pwmPin, 179);  // Actuator at 0%
                Serial.println("Actuator set to 0% position");
                break;

            case 3:
                motor1.moveRelative(stepSize);
                motor2.moveRelative(stepSize);
                Serial.println("Motors rotated 1800 degrees");
                break;
        }

        lastMoveTime = currentTime;

        // Move to next phase in the loop (0 → 1 → 2 → 3 → 0)
        phase = (phase + 1) % 4;

        // Save new phase and time to EEPROM
        EEPROM.put(EEPROM_PHASE_ADDR, phase);
        EEPROM.put(EEPROM_TIME_ADDR, lastMoveTime);
    }
}
