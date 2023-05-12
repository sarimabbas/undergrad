#include <ArduinoJson.h>
#include "driver.hpp"

const size_t capacity = JSON_OBJECT_SIZE(5) + 30;
DynamicJsonDocument doc(capacity);

int llr, lup, rlr, rup;
bool gr;
long readBasePos, readFrontBackPos, readUpDownPos, readGripperPos;
bool gripperClosed; 

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(10);
    driverSetup(10, 11, 12, 13);
    int llr, lup, rlr, rup = 0;
    bool gr, gripperClosed = false;
    long readBasePos, readFrontBackPos, readUpDownPos, readGripperPos = NEUTRAL_POS;
}

void loop() {
    // wait until data becomes available
    if (Serial.available()) {
        // parse gamepad information json
        deserializeJson(doc, Serial);

        llr = doc["llr"];  // -1
        lup = doc["lup"];  // -1
        rlr = doc["rlr"];  // -1
        rup = doc["rup"];  // -1
        gr = doc["gr"];    // false

        readBasePos = getServoPos("BS");
        if (llr == -1) {
            manipulateServo("BS", readBasePos + 5);
        } else if (llr == 1) {
            manipulateServo("BS", readBasePos - 5);
        }

        readFrontBackPos = getServoPos("FB");
        if (lup == -1) {
            manipulateServo("FB", readFrontBackPos + 5);
        } else if (lup == 1) {
            manipulateServo("FB", readFrontBackPos - 5);
        }

        readUpDownPos = getServoPos("UP");
        if (rup == -1) {
            manipulateServo("UP", readUpDownPos + 5);
        } else if (rup == 1) {
            manipulateServo("UP", readUpDownPos - 5);
        }

        readGripperPos = getServoPos("GR");
        // don't open too wide
        if (gr) {
            if (readGripperPos < MAX_POS - 70) {
                manipulateServo("GR", readGripperPos + 10);
            }
        // close all the way
        } else {
            manipulateServo("GR", readGripperPos - 10);
        }
    
    }
}
