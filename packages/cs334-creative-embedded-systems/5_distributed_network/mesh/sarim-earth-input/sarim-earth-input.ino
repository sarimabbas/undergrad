#include <AxisJoystick.h>
#define VRX 15
#define VRY 2
#define JOY_LOW 0
#define JOY_HIGH 4095
#define NUM_LED 8
#define ACTIVATION_THRESHOLD 50
#define DANIEL_PIN 0

AxisJoystick joystick(-1, VRX, VRY);
int LEDPins[] = {26, 33, 27, 5, 17, 19, 18, 32};
int activations[] = {0, 0, 0, 0, 0, 0, 0, 0};

void checkComplete();
void activateLEDFromJoystick();
int getYAxis();
int getXAxis();

void setLowAllExcept(int LEDNotLow);
void animateComplete();

void animateComplete() {
    //  rotate circle 10 times, get faster over time
    int j;
    for (j = 0; j < NUM_LED * 10; j++) {
        digitalWrite(LEDPins[j % NUM_LED], HIGH);
        // get slower over time
        delay(2000 / (j + 1));
        digitalWrite(LEDPins[j % NUM_LED], LOW);
    }
    // keep the others on
    int keep_delay = 2000 / j;
    for (int k = j; k < j + 8; k++) {
        digitalWrite(LEDPins[k % NUM_LED], HIGH);
        delay(keep_delay);
    }
    // keep on for 5s
    delay(5000);
    // turn off
    setLowAllExcept(-1);
}

int getYAxis() {
    if (joystick.isUp()) {
        return -1;
    } else if (joystick.isDown()) {
        return +1;
    } else {
        return 0;
    }
}

int getXAxis() {
    if (joystick.isLeft()) {
        return -1;
    } else if (joystick.isRight()) {
        return +1;
    } else {
        return 0;
    }
}

void setLowAllExcept(int LEDNotLow) {
    for (int i = 0; i < NUM_LED; i++) {
        if (i == LEDNotLow) {
            continue;
        }
        digitalWrite(LEDPins[i], LOW);
    }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    joystick.calibrate(JOY_LOW, JOY_HIGH);

    for (int i = 0; i < NUM_LED; i++) {
        pinMode(LEDPins[i], OUTPUT);
        //        digitalWrite(LEDPins[i], HIGH);
    }

    // configure interface with daniel's mesh point
    pinMode(DANIEL_PIN, OUTPUT);

    //    animateComplete();
}

void activateLEDFromJoystick() {
    int x = getXAxis();
    int y = getYAxis();

    // top
    if (x == 1 && y == -1) {
        if (activations[0] == ACTIVATION_THRESHOLD) {
            digitalWrite(LEDPins[0], HIGH);
        } else {
            activations[0] += 1;
        }
    }

    // right
    if (x == 0 && y == -1) {
        if (activations[2] == ACTIVATION_THRESHOLD) {
            digitalWrite(LEDPins[2], HIGH);
        } else {
            activations[2] += 1;
        }
    }

    // bottom
    if (x == -1 && y == 0) {
        if (activations[4] == ACTIVATION_THRESHOLD) {
            digitalWrite(LEDPins[4], HIGH);
        } else {
            activations[4] += 1;
        }
    }

    // left
    if (x == 0 && y == 1) {
        if (activations[6] == ACTIVATION_THRESHOLD) {
            digitalWrite(LEDPins[6], HIGH);
        } else {
            activations[6] += 1;
        }
    }
}

void checkComplete() {
    if (activations[0] == ACTIVATION_THRESHOLD &&
        activations[2] == ACTIVATION_THRESHOLD &&
        activations[4] == ACTIVATION_THRESHOLD &&
        activations[6] == ACTIVATION_THRESHOLD) {
        // write high if not already
        digitalWrite(LEDPins[0], HIGH);
        digitalWrite(LEDPins[2], HIGH);
        digitalWrite(LEDPins[4], HIGH);
        digitalWrite(LEDPins[6], HIGH);

        // wait 2 seconds
        delay(2000);

        // reset all activations for next cycle
        activations[0] = 0;
        activations[2] = 0;
        activations[4] = 0;
        activations[6] = 0;

        // turn everything off
        setLowAllExcept(-1);

        // start sending mesh signal to daniel's
        digitalWrite(DANIEL_PIN, HIGH);

        // start completion animation
        animateComplete();

        // stop sending mesh signal to daniel's
        digitalWrite(DANIEL_PIN, LOW);
    }
}

void loop() {
    // put your main code here, to run repeatedly:
    joystick.multipleRead();

    // Serial.print(getXAxis());
    // Serial.print(" , ");
    // Serial.println(getYAxis());

    // check LED activations
    activateLEDFromJoystick();

    // check full completion
    checkComplete();
}
