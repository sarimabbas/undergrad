// CITATION: linked example in the spec

// constants
const int triggerPin = 7;
const int echoPin = 8;
const int photocellPin = A0;
const int ledPin = 11;
const int ledRPin = 3;
const int ledGPin = 4;
const int ledBPin = 5;

// Anything over 400 cm/23200 us is out of range
const unsigned int MAX_DIST = 23200;

void setup() {
    pinMode(photocellPin, INPUT);
    pinMode(echoPin, INPUT);
    // trigger pin will tell the sensor to range find
    pinMode(triggerPin, OUTPUT);
    digitalWrite(triggerPin, LOW);

    pinMode(ledPin, OUTPUT);
    pinMode(ledRPin, OUTPUT);
    pinMode(ledGPin, OUTPUT);
    pinMode(ledBPin, OUTPUT);

    digitalWrite(ledRPin, HIGH);
    digitalWrite(ledGPin, HIGH);
    digitalWrite(ledBPin, HIGH);

    Serial.begin(9600);
}

void loop() {
    unsigned long t1;
    unsigned long t2;
    unsigned long pulseWidth;

    // Hold the trigger pin high for at least 10 us
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Wait for pulse on echo pin
    while (digitalRead(echoPin) == 0)
        ;

    // Measure how long the echo pin was held high (pulse width)
    t1 = micros();
    while (digitalRead(echoPin) == 1)
        ;
    t2 = micros();
    pulseWidth = t2 - t1;

    int mappedRangeReading = map(pulseWidth, 0, MAX_DIST, 0, 255);
    if (pulseWidth > MAX_DIST) {
        Serial.println("Out of range");
    } else {
        Serial.println(mappedRangeReading);
    }

    // set color
    if (mappedRangeReading < 40) {
        digitalWrite(ledRPin, HIGH);
        digitalWrite(ledGPin, LOW);
        digitalWrite(ledBPin, HIGH);
    } else if (mappedRangeReading >= 40 && mappedRangeReading < 120) {
        digitalWrite(ledRPin, HIGH);
        digitalWrite(ledGPin, HIGH);
        digitalWrite(ledBPin, LOW);
    } else if (mappedRangeReading >= 120) {
        digitalWrite(ledRPin, LOW);
        digitalWrite(ledGPin, HIGH);
        digitalWrite(ledBPin, HIGH);
    }

    // Note:
    // The specification and Piazza answers were a bit confusing
    // The spec: "Combine this with part 1 and use the photocell to set the brightness."
    // Piazza: "The ultrasonic range finder value should be used to set the brightness."

    // Currently I am only using the photocell to set the brightness
    int photocellReading = analogRead(photocellPin);
    int mappedPhotocellReading = map(photocellReading, 0, 1023, 0, 255);
    Serial.println(mappedPhotocellReading);
    analogWrite(ledPin, mappedPhotocellReading);

    // However, if setting the brightness due to distance is required, I am
    // also writing the code for that below. Thanks
    // if (mappedRangeReading < 40) {
    //     analogWrite(ledPin, 40);
    // } else if (mappedRangeReading >= 40 && mappedRangeReading < 120) {
    //     analogWrite(ledPin, 120);
    // } else if (mappedRangeReading >= 120) {
    //     analogWrite(ledPin, 250);
    // }

    // Wait at least 60ms before next measurement
    delay(1000);
}