// Sarim Abbas, 2019

// Notes: I have used myPinMode(), myShiftOut() and myDigitalWrite(), which are custom functions
void myShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val); // assume LSBFIRST
void myPinMode(uint8_t pin, uint8_t mode); // assume PIND
void myDigitalWrite(uint8_t pin, uint8_t mode); // assume PORTD

void displayNumber(const int x);
void displayNumberCore(const int x);

const unsigned long displayNumberInterval = 2000;
unsigned long previousMillis = 0;

const int dataPin = 6;
const int latchPin = 5;
const int clockPin =  4;

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

void displayNumber(const int x) {
    unsigned long currentMillis;
    while(true) {
        // get current time
        currentMillis = millis();
        // if the number has been displayed for the given interval
        if (currentMillis - previousMillis >= displayNumberInterval) {
            // record new start time and return
            previousMillis = currentMillis;
            return;
        // else display the number
        } else {
            displayNumberCore(x);
        }
    }
}

void displayNumberCore(const int x) {
    // display the number for 2 seconds before exiting
    for (int i = 0; i < 5; i++) {
        // prepare
        myDigitalWrite(latchPin, LOW);

        // set the cathodes
        myShiftOut(dataPin, clockPin, cathodes[i]);

        // set the anodes
        myShiftOut(dataPin, clockPin, numbers[x][i]);

        // store
        myDigitalWrite(latchPin, HIGH);
    }
}

void myShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val) {
    for (uint8_t i = 0; i < 8; i++)  {
        myDigitalWrite(dataPin, !!(val & (1 << i)));
        myDigitalWrite(clockPin, HIGH);
        myDigitalWrite(clockPin, LOW);          
    }
}

void myPinMode(uint8_t pin, uint8_t mode) {
    DDRD |= (mode << pin);
}

void myDigitalWrite(uint8_t pin, uint8_t mode) {
    if (mode == HIGH) {
        PORTD |= (1 << pin); // set bit
    } else {
        PORTD &= ~(1 << pin); // clear bit
    }
}

void setup() {
    Serial.begin(9600);
    myPinMode(dataPin, OUTPUT);
    myPinMode(latchPin, OUTPUT);
    myPinMode(clockPin, OUTPUT);

    // 5 cathodes
    // cathodes: [x, x, x,  x, x, x, x, x]
    // cathodes: [a, b, c,  d, e, f, g, h]
    // cathodes: [1, 3, 10, 7, 8, x, x, x]
    // ACTIVATE with 0

    // 7 anodes
    // anodes: [x,  x,  x, x, x, x, x, x]
    // anodes: [a,  b,  c, d, e, f, g, h]
    // anodes: [12, 11, 2, 9, 4, 5, 6, x]
    // ACTIVATE with 1
}

void loop() {
    for(int i = 0; i < 10; i++) {
        displayNumber(i);
    }
}
