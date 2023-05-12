#include "concurrency.h"
#include "helper.hpp"
#include <Adafruit_SSD1306.h>

// run two LEDs at same time
int concurrencyTest1();
void p3();
void p4();

// run two LEDs on and off after a delay
int concurrencyTest2();
void p1();
void p2();

// test one-sided locks
int concurrencyTest3();
void p5();
void p6();

// test locks with display
int concurrencyTest4();
void p7();
void p8();

// check the push, pop, etc operations
void psmTestDataStructure();

int ledOne = 4;
int ledTwo = 5;
unsigned long startMillis;
unsigned long startMillis2;
long interval = 10000;  // 10 seconds

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
const int RIGHT_DOWN = 1;  // (right or down)
const int LEFT_UP = -1;    // (left or up)
const int MAX_VELOCITY = 50;
const int MIN_VELOCITY = -50;

// Adafruit_SSD1306 disp(128, 64, OLED_SI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);
Adafruit_SSD1306 disp(MAX_WIDTH, MAX_HEIGHT, displayD1, displayD0, displayDC, displayRES, displayCS);

void setup() {
    // LED test cases
    pinMode(ledOne, OUTPUT);
    pinMode(ledTwo, OUTPUT);

    // display test case
    disp.begin(SSD1306_SWITCHCAPVCC);
    disp.clearDisplay();
    disp.display();

    // concurrency global init
    global_manager = NULL;
    current_process = NULL;

    // * replace this with whatever test you want to run (make sure to change the pins!)
    if (concurrencyTest4() < 0 ) { return; }
    
    // yields are disabled because current_process == NULL until process_start() is called
}

void loop() {
    process_start();
    /* if you get here, then all processes are either finished or
       there is deadlock */
    while (1)
        ;
}

// * concurrency test 1: simple LED

int concurrencyTest1() {
    if (process_create(p3, 64) < 0) { return -1; }
    if (process_create(p4, 64) < 0) { return -1; }
    return 0;
}
void p3(void) { digitalWrite(ledOne, HIGH); }
void p4(void) { digitalWrite(ledTwo, HIGH); }

// * concurrency test 2: LED on and off after delay 

int concurrencyTest2() {
    if (process_create(p1, 64) < 0) { return -1; }
    if (process_create(p2, 64) < 0) { return -1; }
    return 0;
}

void p1(void) {
    digitalWrite(ledOne, HIGH);

    // delay
    startMillis = millis();
    while (true) {
        if (millis() - startMillis > interval) {
            digitalWrite(ledOne, LOW);
            break;
        }
    }

    return;
}

void p2(void) {
    digitalWrite(ledTwo, HIGH);

    // delay
    startMillis2 = millis();
    while (true) {
        if (millis() - startMillis2 > interval) {
            digitalWrite(ledTwo, LOW);
            break;
        }
    }

    return;
}

// * concurrency test 3: one sided lock

int concurrencyTest3() {
    if (process_create(p5, 64) < 0) { return -1; }
    if (process_create(p6, 64) < 0) { return -1; }
    return 0;
}

void p5(void) {
    digitalWrite(ledOne, LOW);
    lock_acquire(global_lock);
    int x = 4 + 5;
    lock_release(global_lock);
    digitalWrite(ledOne, HIGH);
}

void p6(void) {
    digitalWrite(ledTwo, LOW);
    digitalWrite(ledTwo, HIGH);
}

// * concurrency test 4: proper lock display

int concurrencyTest4() {
    if (process_create(p7, 100) < 0) { return -1; }
    if (process_create(p8, 100) < 0) { return -1; }
    return 0;
}

void p7(void) {
    int ballVelocity1X = 2;
    int ballVelocity1Y = 2;
    int ball1X = (MAX_WIDTH / 2) - 10;
    int ball1Y = MAX_HEIGHT / 2;
    while(true) {
        // draw on display
        lock_acquire(global_lock);
        disp.clearDisplay();
        disp.fillCircle(ball1X, ball1Y, 4, WHITE);
        disp.display();
        lock_release(global_lock);

        // move ball
        ball1X += ballVelocity1X * 1;
        ball1Y += ballVelocity1Y * 1;

        // elastic collision
        if (ball1X > MAX_WIDTH - 1 || ball1X < MIN_WIDTH) {
            ballVelocity1X = -ballVelocity1X;
        }

        if (ball1Y > MAX_HEIGHT - 1 || ball1Y < MIN_HEIGHT) {
            ballVelocity1Y = -ballVelocity1Y;
        }
    }
    return;
}

void p8(void) {
    int ballVelocity2X = -2;
    int ballVelocity2Y = -2;
    int ball2X = (MAX_WIDTH / 2) - 10;
    int ball2Y = MAX_HEIGHT / 2;
    while (true) {
        // draw on display
        lock_acquire(global_lock);
        disp.clearDisplay();
        disp.fillCircle(ball2X, ball2Y, 4, WHITE);
        disp.display();
        lock_release(global_lock);

        // move ball
        ball2X += ballVelocity2X * 1;
        ball2Y += ballVelocity2Y * 1;

        // elastic collision
        if (ball2X > MAX_WIDTH - 1 || ball2X < MIN_WIDTH) {
            ballVelocity2X = -ballVelocity2X;
        }

        if (ball2Y > MAX_HEIGHT - 1 || ball2Y < MIN_HEIGHT) {
            ballVelocity2Y = -ballVelocity2Y;
        }
    }
    return;
}

// * data structure test

void psmTestDataStructure() {
    unsigned int sp1, sp2, sp3;
    
    // test 1: push to back
    global_manager = psmCreate();
    psmPrint(global_manager);
    psmPushToBack(global_manager, 123);
    psmPushToBack(global_manager, 456);
    psmPushToBack(global_manager, 789);
    psmPrint(global_manager);
    psmDestroy(&global_manager);
    psmPrint(global_manager);

    // test 2: push to front
    global_manager = psmCreate();
    psmPushToFront(global_manager, 123);
    psmPushToFront(global_manager, 456);
    psmPushToFront(global_manager, 789);
    psmPrint(global_manager);
    psmDestroy(&global_manager);
    psmPrint(global_manager);

    // test 3: pop all
    global_manager = psmCreate();
    psmPushToFront(global_manager, 123);
    psmPushToFront(global_manager, 456);
    psmPushToFront(global_manager, 789);
    psmPrint(global_manager);
    psmPop(global_manager);
    psmPop(global_manager);
    psmPop(global_manager);
    psmPop(global_manager);
    psmPrint(global_manager);
    psmDestroy(&global_manager);
    psmPrint(global_manager);

    // test 4: process create (uses push to front)
    process_create(p1, 64);
    process_create(p2, 64);
    process_create(p2, 64);
    psmPrint(global_manager);
    sp1 = psmPop(global_manager);
    psmPrint(global_manager);
    sp2 = psmPop(global_manager);
    sp3 = psmPop(global_manager);
    psmPrint(global_manager);
    psmPop(global_manager);
    psmPrint(global_manager);
    psmPushToBack(global_manager, sp1);
    psmPushToBack(global_manager, sp2);
    psmPushToBack(global_manager, sp3);
    psmPrint(global_manager);
    psmDestroy(&global_manager);
    psmPrint(global_manager);

    // test 5: psmFind
    global_manager = psmCreate();
    psmPrint(global_manager);
    pPrint(psmFind(global_manager, 123));
    psmPushToBack(global_manager, 123);
    psmPushToBack(global_manager, 456);
    psmPushToBack(global_manager, 789);
    psmPushToFront(global_manager, 00000);
    psmPushToFront(global_manager, 11111);
    psmPrint(global_manager);
    pPrint(psmFind(global_manager, 123));
    pPrint(psmFind(global_manager, 456));
    pPrint(psmFind(global_manager, 789));
    pPrint(psmFind(global_manager, 11111));
    psmDestroy(&global_manager);
    psmPrint(global_manager);
}
