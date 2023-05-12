#include "testasm.h"

void setup() {
    // put your setup code here, to run once
    Serial.begin(9600);

    Serial.print("sumval(255, 240) + diffval(255, 240): ");
    Serial.println(testasm(255, 240));

    Serial.print("sumval(240, 255) + diffval(240, 255): ");
    Serial.println(testasm(240, 255));

    Serial.print("sumval(0, 0) + diffval(0, 0): ");
    Serial.println(testasm(0, 0));

    Serial.print("sumval(1, 0) + diffval(1, 0): ");
    Serial.println(testasm(1, 0));

    Serial.print("sumval(10, 10) + diffval(10, 10): ");
    Serial.println(testasm(10, 10));

    pinMode(13, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(13, HIGH);
    delay(400);
    digitalWrite(13, LOW);
    delay(400);
}
