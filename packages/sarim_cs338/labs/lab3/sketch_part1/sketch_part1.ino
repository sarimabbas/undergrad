// CITATION: linked example in the spec

int inputPin = A0;
int outputPin = 11;

void setup() {
    Serial.begin(9600);
    pinMode(inputPin, INPUT);
    pinMode(outputPin, OUTPUT);
}

void loop() {
    int reading = analogRead(inputPin);
    int mappedReading = map(reading, 0, 1023, 0, 255);
    Serial.println(mappedReading);
    analogWrite(outputPin, mappedReading);
    delay(1000);
}

// How to implement analogRead:
// Arduino has 10-bit ADT that maps voltages from 0-5V between 0-1023 (4.9 mV per unit)

// enable ADC using ADSCRA register (set bit position 7 / ADEN to HIGH)
// ADSCRA |= 0b10000000

// we need to set the ADMUX register
// 7        6       5       4   3       2       1       0
// REFS1    REFS0   ADLAR   -   MUX3    MUX2    MUX1    MUX0

// make sure ADMUX clear (ADLAR is 0 so result will be right adjusted)
// ADMUX &= 0b00000000

// set reference high and low voltage (from datasheet: use AVCC)
// ADMUX |= 0b01000000

// select ADC (corresponding to pin)
// ADMUX |= 0b00000001

// start conversion (set bit position 6 / ADSC)
// ADSCRA |= 0b01000000

// result will be contained within ADC9:0. ADC7:0 is found in ADCL register. ADC9:8 is found in ADCH register.
// there is helpful macro called ADC which you can use to return the value in one go
