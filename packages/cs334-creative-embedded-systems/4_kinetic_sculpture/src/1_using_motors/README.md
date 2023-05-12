# Module 4, Task 1: Using motors

![Cover](./doc.jpg)

## Overview

The task is primarily a technical exercise to help gain familiarity with the hardware for the module. Using the following actuators:

-   28BYJ-48 Step Motor
-   SG90 Tower Pro MicroServo

Connect the servo and the stepper motor to the ESP32. Write code so that each time the stepper motor rotates 360°, the servo motor rotates 10 degrees. At 180 degrees, the servo should return to 0 degrees.

## Implementation

Code is in the `1_using_motors.ino` file.

### Notable challenges

-   Two pins (34 and 35) were unsuitable for the stepper motor, but 25 and 26 worked perfectly.
