
#include <Encoder.h>
#include "driver.hpp"

// encoder objects
Encoder frontBackEncoder(2, 3);
long frontBackEncoderPos;
Encoder upDownEncoder(4, 5);
long upDownEncoderPos;
Encoder gripperEncoder(6, 7);
long gripperEncoderPos;
Encoder baseEncoder(8, 9);
long baseEncoderPos;

void setup() {
    Serial.begin(9600);

    // initialize servos
    driverSetup(10, 11, 12, 13);

    // init encoder positions
    frontBackEncoderPos = NEUTRAL_POS;
    upDownEncoderPos = NEUTRAL_POS;
    gripperEncoderPos = NEUTRAL_POS;
    baseEncoderPos = NEUTRAL_POS;

    // set encoders start to neutral position
    frontBackEncoder.write(frontBackEncoderPos);
    upDownEncoder.write(upDownEncoderPos);
    gripperEncoder.write(gripperEncoderPos);
    baseEncoder.write(baseEncoderPos);
}

void loop() {
    // front back movement
    long readFBE = frontBackEncoder.read();
    if (readFBE > MAX_POS) {
        frontBackEncoder.write(MAX_POS);
    } else if (readFBE < MIN_POS) {
        frontBackEncoder.write(MIN_POS);
    } else {
        frontBackEncoderPos = readFBE;
        manipulateServo("FB", frontBackEncoderPos);
    }

    // up down movement
    long readUPE = upDownEncoder.read();
    if (readUPE > MAX_POS) {
        upDownEncoder.write(MAX_POS);
    } else if (readUPE < MIN_POS) {
        upDownEncoder.write(MIN_POS);
    } else {
        upDownEncoderPos = readUPE;
        manipulateServo("UP", upDownEncoderPos);
    }

    // gripper movement
    long readGRE = gripperEncoder.read();
    if (readGRE > MAX_POS) {
        gripperEncoder.write(MAX_POS);
    } else if (readGRE < MIN_POS) {
        gripperEncoder.write(MIN_POS);
    } else {
        gripperEncoderPos = readGRE;
        manipulateServo("GR", gripperEncoderPos);
    }

    // base movement
    long readBSE = baseEncoder.read();
    if (readBSE > MAX_POS) {
        baseEncoder.write(MAX_POS);
    } else if (readBSE < MIN_POS) {
        baseEncoder.write(MIN_POS);
    } else {
        baseEncoderPos = readBSE;
        manipulateServo("BS", baseEncoderPos);
    }
}
