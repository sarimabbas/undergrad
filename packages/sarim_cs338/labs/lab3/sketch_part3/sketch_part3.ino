// CITATION: the tutorials on the linked SparkFun website:
// https://www.sparkfun.com/products/9117
// http://bildr.org/2012/08/rotary-encoder-arduino/
// https://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino/
// http://w264779-www.fnweb.no/rotary_encoder/

#include <Adafruit_SSD1306.h>

// prototypes
void encoderPin1ISR();
void encoderPin2ISR();
void moveBall();
void mapEncoderToVelocity();
int maintainVelocityDirection(int velocity, int transform);

// CS : this is the chip select signal
// DC : this is a mode signal (data/control) used for commands to the SSD1306 controller used by the display
// RES : reset signal, used for initialization
// D0 : this is the SPI clock (called CLK below)
// D1 : this is the SPI data (from the microcontroller to the display, called SI below)
// VCC : power supply
// GND : ground

// display IO pins and thresholds
const int displayD0 = 6;
const int displayD1 = 7;
const int displayRES = 8;
const int displayDC = 9;
const int displayCS = 10;
const int MAX_WIDTH = 128;
const int MIN_WIDTH = 0;
const int MAX_HEIGHT = 64;
const int MIN_HEIGHT = 0;

// ball initialization
const int RIGHT_DOWN = 1; // (right or down)
const int LEFT_UP = -1; // (left or up)
const int MAX_VELOCITY = 50;
const int MIN_VELOCITY = -50;
int ballX = MAX_WIDTH / 2;
int ballY = MAX_HEIGHT / 2;
volatile int ballVelocityX = 0;
volatile int ballVelocityY = 0;

// Adafruit_SSD1306 disp(128, 64, OLED_SI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);
Adafruit_SSD1306 disp(MAX_WIDTH, MAX_HEIGHT, displayD1, displayD0, displayDC, displayRES, displayCS);

// encoder IO pins and thresholds
const int encoderPin1 = 2;
const int encoderPin2 = 3;
const int UPPER_LIMIT = 255;
const int LOWER_LIMIT = 0;
const int MIDDLE = 128;

// interrupts shared variabels
volatile int encoderValue = MIDDLE;  
int prevEncoderValue = encoderValue;
volatile boolean halfLeft = false;  
volatile boolean halfRight = false;

void setup() {
    Serial.begin(9600);
    pinMode(encoderPin1, INPUT);
    pinMode(encoderPin2, INPUT);
    digitalWrite(encoderPin1, HIGH);
    digitalWrite(encoderPin2, HIGH);

    attachInterrupt(0, encoderPin1ISR, FALLING);
    attachInterrupt(1, encoderPin2ISR, FALLING);

    // display initialization
    disp.begin(SSD1306_SWITCHCAPVCC);
}

void loop() {
    // refresh display
    disp.clearDisplay();
    disp.fillCircle(ballX, ballY, 4, WHITE);
    disp.display();

    // move ball
    moveBall();

    // monitor changes in rotary encoder value
    if (encoderValue != prevEncoderValue) {
        Serial.println(encoderValue);
        prevEncoderValue = encoderValue;
    }
}

void moveBall() {
    // recompute velocity
    mapEncoderToVelocity();

    // ball displacement
    ballX += ballVelocityX * 1;
    ballY += ballVelocityY * 1;

    // elastic collision 
    if (ballX > MAX_WIDTH - 1 || ballX < MIN_WIDTH) {
        ballVelocityX = -ballVelocityX;
    }

    if (ballY > MAX_HEIGHT - 1 || ballY < MIN_HEIGHT) {
        ballVelocityY = -ballVelocityY;
    }
}

// rotary pin 2 goes LOW
void encoderPin1ISR() {
    // debounce time
    delay(1);                                                

    // pin 2 still low
    if (digitalRead(encoderPin1) == LOW) {                             
        if (digitalRead(encoderPin2) == HIGH && halfRight == false) {
            halfRight = true;
        }
        if (digitalRead(encoderPin2) == LOW && halfLeft == true) {
            halfLeft = false;
            if (encoderValue != LOWER_LIMIT) {
                encoderValue--;
            }
        }
    }
}
void encoderPin2ISR() {
    delay(1);
    if (digitalRead(encoderPin2) == LOW) {
        if (digitalRead(encoderPin1) == HIGH && halfLeft == false) {
            halfLeft = true;  
        }
        if (digitalRead(encoderPin1) == LOW && halfRight == true) {
            halfRight = false;
            if (encoderValue != UPPER_LIMIT) {
                encoderValue++;
            }
        }
    }
}

void mapEncoderToVelocity() {
    int mappedValue = map(prevEncoderValue, 0, 255, MIN_VELOCITY, MAX_VELOCITY);
    ballVelocityX = maintainVelocityDirection(ballVelocityX, mappedValue);
    ballVelocityY = maintainVelocityDirection(ballVelocityY, mappedValue);
}   

int maintainVelocityDirection(int velocity, int transform) {
    if (velocity >= 0) {
        if (transform >= 0) {
            velocity = transform;
        } else {
            velocity = -transform;
        }
    } else {
        if (transform >= 0) {
            velocity = -transform;
        } else {
            velocity = transform;
        }
    }
    return velocity;
}