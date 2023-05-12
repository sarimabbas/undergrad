// Sarim Abbas, 2019
void displayNumber(const int x);
void displayNumberCore(const int x);
void buttonPressed();

const int dataPin = 6;
const int latchPin = 5;
const int clockPin =  4;
const int buttonPin = 2;

// Interrupt service routine
volatile int displayGlobal = 0;
volatile unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

const unsigned numbers[10][5] = {
    {124, 130, 130, 130, 124}, // 0111 1100, 1000 0010, 1000 0010, 1000 0010, 0111 1100 => 0
    {66, 130, 254, 2, 2}, // => 1
    {0, 70, 138, 146, 98}, // => 2
    {130, 146, 154, 108, 0}, // => 3
    {24, 40, 72, 190, 0}, // => 4
    {114, 82, 82, 76, 0}, // => 5
    {60, 82, 146, 140, 0}, // => 6
    {128, 144, 254, 16, 0}, // => 7
    {108, 146, 146, 146, 108}, // => 8 
    {96, 144, 144, 254, 0}, // => 9
};

const unsigned cathodes[] = {
    127, 191, 223, 239, 247
};

void displayNumberCore(const int x) {
    // display the number for 2 seconds before exiting
    for (int i = 0; i < 5; i++) {
        // prepare
        digitalWrite(latchPin, LOW);

        // set the cathodes
        shiftOut(dataPin, clockPin, LSBFIRST, cathodes[i]);

        // set the anodes
        shiftOut(dataPin, clockPin, LSBFIRST, numbers[x][i]);

        // store
        digitalWrite(latchPin, HIGH);
    }
}

void buttonPressed() {
    unsigned long interruptTime = millis();
    if (interruptTime - lastDebounceTime > debounceDelay) {
        displayGlobal = (displayGlobal + 1) % 10;
    }
    lastDebounceTime = interruptTime;
}

void setup() {
    Serial.begin(9600);
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);

    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, RISING);
}

void loop() {
    displayNumberCore(displayGlobal);
}
