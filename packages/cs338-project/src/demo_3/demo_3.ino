#include <hcsr04.h>
#include "driver.hpp"

#define SENSOR_MIN 50
#define SENSOR_MAX 500
const int triggerPin = A0;
const int echoPin = A1;
HCSR04 hcsr04(triggerPin, echoPin, SENSOR_MIN, SENSOR_MAX);
void smoothMovement(long from, long to);

void smoothMovement(long from, long to) {
    if (from > to) {
        for (long i = from; i >= to; i--) {
            manipulateServo("FB", i);
            delay(15);
        }
    // from <= to
    } else {
        for (long i = from; i < to; i++) {
            manipulateServo("FB", i);
            delay(15);
        }
    }
}

void setup() {
    driverSetup(10, 11, 12, 13);
    pinMode(echoPin, INPUT);
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);
    Serial.begin(9600); 
}

void loop() {
    // get sensor reading
    int reading = hcsr04.distanceInMillimeters();

    // get existing position
    long currentPos = getServoPos("FB");

    // ranges
    if (reading <= 500 && reading > 400) {
        smoothMovement(currentPos, 120);
    } else if (reading <= 400 && reading > 200) {
        smoothMovement(currentPos, 110);
    } else if (reading <= 200 && reading > 100) {
        smoothMovement(currentPos, 90);
    } else if (reading <= 100 && reading > 50) {
        smoothMovement(currentPos, 70);
    }

    delay(250);
}