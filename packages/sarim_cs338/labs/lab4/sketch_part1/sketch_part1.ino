#include "concurrency.h"
#include "helper.hpp"

// run two LEDs at same time
int concurrencyTest1();
void p3();
void p4();

// run two LEDs on and off after a delay
int concurrencyTest2();
void p1();
void p2();

// check the push, pop, etc operations
void psmTestDataStructure();

int ledOne = 4;
int ledTwo = 5;
unsigned long startMillis;
unsigned long startMillis2;
long interval = 10000;  // 10 seconds

void setup() {
    pinMode(ledOne, OUTPUT);
    pinMode(ledTwo, OUTPUT);

    global_manager = NULL;
    current_process = NULL;

    if (concurrencyTest2() < 0 ) { return; }
    // if (concurrencyTest1() < 0 ) { return; }
    
    // yields are disabled because current_process == NULL until process_start() is called
}

void loop() {
    process_start();
    /* if you get here, then all processes are either finished or
       there is deadlock */
    while (1)
        ;
}

int concurrencyTest1() {
    if (process_create(p3, 64) < 0) { return -1; }
    if (process_create(p4, 64) < 0) { return -1; }
    return 0;
}
void p3(void) { digitalWrite(ledOne, HIGH); }
void p4(void) { digitalWrite(ledOne, HIGH); }

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
