/**
 * @file 1_esp32_sketch.ino
 * @authors Sarim Abbas, Felicia Chang
 * @brief Software to control motors for kinetic sculpture project
 * @version 0.1
 * @date 2019-11-04
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <ESP32Servo.h>
#include <Stepper.h>

// Note: although theoretically the ESP32 should be able to support multiple
// steppers, in practice it cannot and overheats. Therefore, we used two ESP32s
// to power one stepper and one servo each.

Servo servoOne;
const int stepsPerRevolution = 64;
Stepper myStepper(stepsPerRevolution, 32, 33, 25, 26);

int servoPos = 0;
int rotationCounter = 0;
bool moveForward = true;

void setup() {
    servoOne.attach(13);
    myStepper.setSpeed(60);
}

void loop() {
    myStepper.step(stepsPerRevolution);
    rotationCounter += stepsPerRevolution;

    if (rotationCounter % 64 == 0) rotate10();
}

void rotate10() {
    if (servoPos == 10) moveForward = false;
    if (servoPos == 0) moveForward = true;

    if (moveForward)
        servoPos += 10;
    else
        servoPos -= 10;
    servoOne.write(servoPos);
}