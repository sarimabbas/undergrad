#include "helper.hpp"
#include <Arduino.h>

//////////// * create, destroy and search functions

psm* psmCreate(void) {
    psm* manager = (psm*) malloc(sizeof(psm));
    if (manager == NULL) { return NULL; }
    manager->head = NULL;
    manager->count = 0;
    return manager;
}

void psmDestroy(psm** managerPtr) {
    psm* manager = *managerPtr;
    if (manager == NULL) { return; }
    // free each process
    process_t* curr = manager->head;
    process_t* tmp;
    while (curr != NULL) {
        tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    // free the manager
    free(manager);
    *managerPtr = NULL;
}

process_t* psmFind(psm* manager, unsigned int sp) {
    if (manager == NULL) { return NULL; }
    process_t* curr = manager->head;
    while (curr != NULL) {
        if (curr->sp == sp) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

//////////// * add and remove functions

int psmPushToFront(psm* manager, unsigned int sp) {
    if (manager == NULL) { return  -1; };

    // create new node
    process_t* node = (process_t*) malloc(sizeof(process_t));
    if (node == NULL) { return -1; };

    // initialize the node
    node->next = manager->head;
    node->sp = sp;

    // doesn't matter whatever the head is pointing to
    manager->head = node;
    (manager->count)++;

    return 0;
}

int psmPushToBack(psm* manager, unsigned int sp) {
    if (manager == NULL) { return -1; };

    // malloc new node
    process_t* node = (process_t*)malloc(sizeof(process_t));
    if (node == NULL) { return -1; };
    node->sp = sp;
    node->next = NULL;

    // first element
    if (manager->count == 0) {
        manager->head = node;

    // second or more element
    } else {
        process_t* curr = manager->head;
        process_t* prev = NULL;
        // iterate prev to pen ultimate node
        while (curr != NULL) {
            prev = curr;
            curr = curr->next;
        }
        prev->next = node;
    }

    // increment and return
    (manager->count)++;
    return 0;
}

unsigned int psmPop(psm* manager) {
    if (manager == NULL) { return  -1; };

    process_t* tmp = manager->head;

    // not empty queue
    if (tmp != NULL) {
        manager->head = tmp->next;
        unsigned int sp = tmp->sp;
        free(tmp);
        (manager->count)--;
        return sp;
    }

    // empty 
    return 0;
}

//////////// * print functions

void SerialPrintWrapper(char* msg) {
    if (msg == NULL) {
        Serial.println("SerialPrintWrapper: msg was NULL");
        return;
    }
    Serial.print(msg);
    return;
}

void psmPrint(psm* manager) {
    if (manager == NULL) {
        Serial.println("psmPrint: manager is NULL");
        return;
    }

    Serial.print("Queue (");
    Serial.print(manager->count);
    Serial.print("): ");

    // iterate through queue
    process_t* curr = manager->head;
    while (curr != NULL) {
        // print formatted data
        Serial.print("[sp: ");
        Serial.print(curr->sp);
        Serial.print("], ");

        // advance curr
        curr = curr->next;
    }
    // new line
    Serial.println("");
    Serial.flush();
}

void pPrint(process_t* process) {
    if (process == NULL) {
        Serial.println("pPrint: process is NULL");
        return;
    }
    Serial.print("pPrint: [sp: ");
    Serial.print(process->sp);
    Serial.println("]");
    Serial.flush();
}

void disableInterruptsWrapper(){
    noInterrupts();
}

void enableInterruptsWrapper() {
    interrupts();
}
