#include "testasm.h"

void setup() {
    // put your setup code here, to run once
    Serial.begin(9600);

    Serial.print("fib(12) is: ");
    Serial.println(testasm(12, 0));

    Serial.print("fib(11) is: ");
    Serial.println(testasm(11, 1));

    Serial.print("fib(10) is: ");
    Serial.println(testasm(10, 2));

    Serial.print("fib(5) is: ");
    Serial.println(testasm(5, 3));

    Serial.print("fib(1) is: ");
    Serial.println(testasm(1, 4));

    Serial.print("fib(0) is: ");
    Serial.println(testasm(0, 5));

    pinMode(13, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(13, HIGH);
    delay(400);
    digitalWrite(13, LOW);
    delay(400);
}
