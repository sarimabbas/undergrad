
#include <Encoder.h>
#include <Servo.h>

Servo frontBackServo;
Encoder frontBackEncoder(2, 3);
long frontBackPos = 90;

Servo upDownServo;
Encoder upDownEncoder(4, 5);
long upDownPos = 90;

Servo gripperServo;
Encoder gripperEncoder(6, 7);
long gripperPos = 90;

Servo baseServo;
Encoder baseEncoder(8, 9);
long basePos = 90;

const int UPPER_LIMIT = 180;
const int LOWER_LIMIT = 0;

void setup() {
    Serial.begin(9600);

    // initialize servos
    frontBackServo.attach(10);
    upDownServo.attach(11);
    gripperServo.attach(12);
    baseServo.attach(13);

    // set servos to neutral position
    frontBackServo.write(90);
    upDownServo.write(90);
    gripperServo.write(90);
    baseServo.write(90);

    // set encoders start to neutral position 
    frontBackEncoder.write(90);
    upDownEncoder.write(90);
    gripperEncoder.write(90);
    baseEncoder.write(90);
}

void loop() {
    long newFrontBackPos = frontBackEncoder.read();
    if (newFrontBackPos != frontBackPos) {
        if (newFrontBackPos <= UPPER_LIMIT && newFrontBackPos >= LOWER_LIMIT) {
            frontBackPos = newFrontBackPos;
            frontBackServo.write(frontBackPos);
        }
        Serial.print("frontBackPos = ");
        Serial.print(frontBackPos);
        Serial.println();
    }

    long newUpDownPos = upDownEncoder.read();
    if (newUpDownPos != upDownPos) {
        if (newUpDownPos <= UPPER_LIMIT && newUpDownPos >= LOWER_LIMIT) {
            upDownPos = newUpDownPos;
            upDownServo.write(upDownPos);
        }
        Serial.print("upDownPos = ");
        Serial.print(upDownPos);
        Serial.println();
    }

    long newGripperPos = gripperEncoder.read();
    if (newGripperPos != gripperPos) {
        if (newGripperPos <= UPPER_LIMIT && newGripperPos >= LOWER_LIMIT) {
            gripperPos = newGripperPos;
            gripperServo.write(gripperPos);
        }
        Serial.print("gripperPos = ");
        Serial.print(gripperPos);
        Serial.println();
    }

    long newBasePos = baseEncoder.read();
    if (newBasePos != basePos) {
        if (newBasePos <= UPPER_LIMIT && newBasePos >= LOWER_LIMIT) {
            basePos = newBasePos;
            baseServo.write(basePos);
        }
        Serial.print("basePos = ");
        Serial.print(basePos);
        Serial.println();
    }
}
