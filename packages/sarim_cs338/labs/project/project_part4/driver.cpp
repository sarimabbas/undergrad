#include "driver.hpp"
#include <Arduino.h>
#include <string.h>

Servo frontBackServo, upDownServo, gripperServo, baseServo;
long frontBackPos, upDownPos, gripperPos, basePos;

int driverSetup(int frontBackPin, int upDownPin, int gripperPin, int basePin) {
    // attach to pins
    frontBackServo.attach(frontBackPin);
    upDownServo.attach(upDownPin);
    gripperServo.attach(gripperPin);
    baseServo.attach(basePin);
    // initialize position variables
    frontBackPos = NEUTRAL_POS;
    upDownPos = NEUTRAL_POS;
    gripperPos = NEUTRAL_POS;
    basePos = NEUTRAL_POS;
    // set to neutral position
    frontBackServo.write(frontBackPos);
    upDownServo.write(upDownPos);
    gripperServo.write(gripperPos);
    baseServo.write(basePos);
    return 0;
}

int manipulateServo(const char* servo, int pos) {
    // only update if within bounds
    if (!(pos >= MIN_POS && pos <= MAX_POS)) {
        return -1;
    }
    // only update if != prev position
    if (strcmp("FB", servo) == 0) {
        if (pos != frontBackPos) {
            frontBackPos = pos;
            frontBackServo.write(frontBackPos);
        }
    } else if (strcmp("UP", servo) == 0) {
        if (pos != upDownPos) {
            upDownPos = pos;
            upDownServo.write(upDownPos);
        }
    } else if (strcmp("GR", servo) == 0) {
        if (pos != gripperPos) {
            gripperPos = pos;
            gripperServo.write(gripperPos);
        }
    } else if (strcmp("BS", servo) == 0) {
        if (pos != basePos) {
            basePos = pos;
            baseServo.write(basePos);
        }
    } else {
        // invalid servo
        return -1;
    }
    return 0;
}

int gripperOpen(void) {
    for (int pos = gripperPos; pos <= MAX_POS; pos += 1) {
        manipulateServo("GR", pos);
        delay(15);
    }
}

int gripperClose(void) {
    for (int pos = gripperPos; pos >= MIN_POS; pos -= 1) {
        manipulateServo("GR", pos);
        delay(15);
    }
}

int moveLeft(void) {
    for (int pos = basePos; pos <= MAX_POS - MOVE_PADDING_SMALL; pos += 1) {
        manipulateServo("BS", pos);
        delay(15);
    }
}

int moveRight(void) {
    for (int pos = basePos; pos >= MIN_POS + MOVE_PADDING_SMALL; pos -= 1) {
        manipulateServo("BS", pos);
        delay(15);
    }
}

int moveUp(void) {
    manipulateNeutral("UP");
    manipulateNeutral("FB");
}

int moveDown(void) {
    for (int pos = upDownPos; pos >= MIN_POS + MOVE_PADDING_LARGE; pos -= 1) {
        manipulateServo("UP", pos);
        delay(15);
    }
    for (int pos = frontBackPos; pos <= MAX_POS - MOVE_PADDING_SMALL; pos += 1) {
        manipulateServo("FB", pos);
        delay(15);
    }
}

int manipulateNeutral(const char* servo) {
    int pos;
    if (strcmp("FB", servo) == 0) {
        if (frontBackPos < NEUTRAL_POS) {
            for (pos = frontBackPos; pos <= NEUTRAL_POS; pos += 1) {
                manipulateServo("FB", pos);
                delay(15);
            }
        } else {
            for (pos = frontBackPos; pos >= NEUTRAL_POS; pos -= 1) {
                manipulateServo("FB", pos);
                delay(15);
            }
        }
    } else if (strcmp("UP", servo) == 0) {
        if (upDownPos < NEUTRAL_POS) {
            for (pos = upDownPos; pos <= NEUTRAL_POS; pos += 1) {
                manipulateServo("UP", pos);
                delay(15);
            }
        } else {
            for (pos = upDownPos; pos >= NEUTRAL_POS; pos -= 1) {
                manipulateServo("UP", pos);
                delay(15);
            }
        }
    } else if (strcmp("GR", servo) == 0) {
        if (gripperPos < NEUTRAL_POS) {
            for (pos = gripperPos; pos <= NEUTRAL_POS; pos += 1) {
                manipulateServo("GR", pos);
                delay(15);
            }
        } else {
            for (pos = gripperPos; pos >= NEUTRAL_POS; pos -= 1) {
                manipulateServo("GR", pos);
                delay(15);
            }
        }
    } else if (strcmp("BS", servo) == 0) {
        if (basePos < NEUTRAL_POS) {
            for (pos = basePos; pos <= NEUTRAL_POS; pos += 1) {
                manipulateServo("BS", pos);
                delay(15);
            }
        } else {
            for (pos = basePos; pos >= NEUTRAL_POS; pos -= 1) {
                manipulateServo("BS", pos);
                delay(15);
            }
        }
    } else {
        return -1;
    }
    return 0;
}

long getServoPos(const char* servo) {
    if (strcmp("FB", servo) == 0) {
       return frontBackPos;
    } else if (strcmp("UP", servo) == 0) {
        return upDownPos;
    } else if (strcmp("GR", servo) == 0) {
        return gripperPos;
    } else if (strcmp("BS", servo) == 0) {
        return basePos;
    } else {
        return -1;
    }
}