#include "hashtable.h"
#include "helper.h"
#include "code.h"
#define ABSOLUTE_MAXBITS 22

int main(void) {
    hashtable* ht = htCreate(3, ABSOLUTE_MAXBITS);
    hashtable** htp = &ht;

    entry e1;
    e1.character = 'a';
    e1.prefix = EMPTY;
    htAdd(htp, &e1);

    entry e2;
    e2.character = 'b';
    e2.prefix = EMPTY;
    htAdd(htp, &e2);
    
    htDump(htp, "");

    htDestroy(htp);

    // hashtable 2
    // store 255 ASCII chars in only a ht of 128
    ht = htCreate(7, ABSOLUTE_MAXBITS);
    htp = &ht;
    for (int i = 0; i < 256; i++) {
        if (i == 125) {
            entry init;
            init.character = i;
            init.prefix = EMPTY;
            htAdd(htp, &init);
        } else {
            entry init;
            init.character = i;
            init.prefix = EMPTY;
            htAdd(htp, &init);
        }
    }
    htDestroy(htp);

    // stack
    stack* s = sCreate();
    sDump(s);
    sPush(s, 43);
    sDump(s);
    sPush(s, 71);
    sDump(s);
    sDestroy(s);


    return 0;
}
