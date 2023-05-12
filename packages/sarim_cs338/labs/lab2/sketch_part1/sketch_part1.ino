const int buttonPin = 2;
const int ledPin = 13;

int toggle = LOW;
int buttonPinState = LOW;
int lastButtonPinState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 20;

void setup() {
    Serial.begin(9600);
    
    // set buttonPin to input
    DDRD |= (0 << buttonPin);
    // set ledPin to output
    DDRB |= (1 << (ledPin - 8));

    // * initial LED state (testing)
    // digitalWrite(ledPin, toggle);
}

// I took the advice on Piazza and adapted the code from the Arduino website
void loop() {
    int reading = (PIND & ( 1 << buttonPin )) >> buttonPin;

    if (reading != lastButtonPinState) {
        
        // set to current time/reset
        lastDebounceTime = millis();
    }


    if((millis() - lastDebounceTime) > debounceDelay) {

        // if PIN2 state has changed
        if (reading != buttonPinState) {
            buttonPinState = reading;
            

            // toggle if PIN2 is LOW
            if (buttonPinState == LOW) {
                toggle = !toggle;
                Serial.println(toggle);
            }
        }
    }

    // * set LED (testing)
    // digitalWrite(ledPin, toggle);

    // save reading for next time
    lastButtonPinState = reading;
}
