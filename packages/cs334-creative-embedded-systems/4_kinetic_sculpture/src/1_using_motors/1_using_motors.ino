/**
 * @file 1_using_motors.ino
 * @author Sarim Abbas, Cynthia Vu, Daniel Taylor
 * @brief Created during lab
 * @version 0.1
 * @date 2019-10-24
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <ESP32Servo.h>
#include <Stepper.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

const int stepsPerRevolution = 64;
Stepper myStepper(stepsPerRevolution, 32, 33, 25, 26);

int servoPos = 0;  // variable to store the servo position
int degree = 0;
int rotationCounter = 0;
bool moveForward = true;
int increment = 10;

void setup() {
    myservo.attach(13);  // attaches the servo on pin 13 to the servo object
    myStepper.setSpeed(60);
}

void loop() {
    myStepper.step(stepsPerRevolution);
    rotationCounter += stepsPerRevolution;

    if (rotationCounter % 64 == 0) rotate10();
}

void rotate10() {
    if (servoPos == 180) moveForward = false;
    if (servoPos == 0) moveForward = true;

    if (moveForward)
        servoPos += 10;
    else
        servoPos -= 10;
    myservo.write(servoPos);
}
