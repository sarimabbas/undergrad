#include "testasm.h"

void setup() {
    // put your setup code here, to run once
    Serial.begin(9600);

    Serial.print("The sum of 3 and 4 is: ");
    Serial.println(testasm(3, 4));

    Serial.print("The sum of 4 and 3 is: ");
    Serial.println(testasm(4, 3));

    Serial.print("The sum of 0 and 0 is: ");
    Serial.println(testasm(0, 0));

    Serial.print("The sum of 250 and 250 is: ");
    Serial.println(testasm(250, 250));

    Serial.print("The sum of 255 and 1 is: ");
    Serial.println(testasm(255, 1));

    pinMode(13, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(13, HIGH);
    delay(400);
    digitalWrite(13, LOW);
    delay(400);
}
