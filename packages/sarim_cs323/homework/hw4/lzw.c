#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "helper.h"
#include "code.h"

void encode(int maxbits, int block, double ratio);
void decode();

#define START_BITS 5 // these are how many you need to send the start code (which could be 9-22)
#define DEFAULT_MAXBITS 12
#define PROTECT_SIGNAL 42

int main(int argc, char** argv) {

    // use small static buffers
    static char bin[64], bout[64];
    setvbuf(stdin, bin, _IOFBF, 64);
    setvbuf(stdout, bout, _IOFBF, 64);

    // parse command line arguments
    int m_value = -1;
    int r_block = -1;
    double r_ratio = -1.0;
    bool parsed = commandLineParse(argc, argv, &m_value, &r_block, &r_ratio);

    if (!parsed) {
        return 1;
    }

    // maxbits: the max size the hashtable can take on
    int maxbits = DEFAULT_MAXBITS;
    if (m_value >= ABSOLUTE_MINBITS && m_value <= ABSOLUTE_MAXBITS) {
        maxbits = m_value;
    }

    // printf("BLOCK: %i, RATIO: %lf\n", r_block, r_ratio);

    // TODO: STAGE SET :: remove for STAGE 3 and production
    // setenv("STAGE", "2", 1);

    // check encode or decode
    if (strstr(argv[0], "encode") != NULL) { 
        // TODO: what if the folder name is decode, and encode is inside?
        // TODO: probably do it based on last one found 
        encode(maxbits, r_block, r_ratio);
    } else {
        decode();
    }

    // success
    return 0;
}

void encode(int maxbits, int block, double ratio) {
    // FILE* f = fopen("tests/resize/test_in_2.txt", "r");
    // FILE* f = fopen("tests/resize/austin.txt", "r");

    // send protection code at start
    putBits(DEFAULT_MAXBITS, PROTECT_SIGNAL);

    // send signal for maxbits
    putBits(START_BITS, maxbits);

    // -r mode
    bool r_mode = true;
    int nsent = 0;
    int nread = 0;
    int ncodes = 0;
    if (block == -1 || ratio < 0) {
        r_mode = false;
    }

    // create hashtable
    hashtable* ht = htCreate(ABSOLUTE_MINBITS, maxbits);
    hashtable** htp = &ht;

    // initialize with RESIZE_SIGNAL and 255 ASCII chars
    htInitTable(htp);

    int C = EMPTY;
    int K;
    while ((K = getchar()) != EOF) {

        // fprintf(stderr, "Load average: %f\n", ht->size / (float)ht->capdec);

        // a character has been read so we can add it to nread
        if (r_mode) {
            nread += CHAR_BIT;
        }

        entry search;
        search.prefix = C;
        search.character = K;

        int find_result = htFind(htp, &search);

        // if the pair (C, K) is in the table
        if (find_result != -1) {

            // set C = code associated with the pair (C, K) in the table
            C = find_result;

        } else {
            // output code C
            putBits(ht->capbits, C);

            // a code has been output so we can add it to nsent
            if (r_mode) {
                nsent += ht->capbits;
                ncodes++;
            }

            // signal resize
            if (htGrowEminent(htp)) {

                // fprintf(stderr, "growing from %i to %i\n", ht->capdec, ht->capdec * GROW_FACTOR);
                // fprintf(stderr, "GROW: %i\n", ht->size);

                // fprintf(stderr, "LOAD AVERAGE: %f\n", ht->size / (float) ht->capdec);

                putBits(ht->capbits, RESIZE_SIGNAL);

                // htDump(htp, "");

                int* conversion_table = htGrow(htp);

                // fprintf(stderr, "GROW: %i\n", ht->size);

                // htDump(htp, "");

                // update any info
                search.prefix = conversion_table[search.prefix];

                // free table
                free(conversion_table);

            }

            // insert the pair (C, K) into the table
            // int pos = htAdd(htp, &search);
            if (htCanAdd(htp)) {
                htAdd(htp, &search);
            }

            // -r reset needed?
            if (r_mode) {
                if (ncodes >= block) {
                    if (nsent > ratio * nread) {
                        // fprintf(stderr, "NREAD: %i, NSENT: %i, RATIO: %lf,
                        // Resetting table\n", nread, nsent, ratio);
                        htReset(htp);
                        putBits(ht->capbits, RESET_SIGNAL);
                    }
                    nsent = 0;
                    nread = 0;
                    ncodes = 0;
                }
            }

            // fprintf(stderr, "LOAD AVERAGE: %f\n", ht->size / (float) ht->capdec);

            // set C = code associated with the pair (EMPTY, K) in the table
            entry empty_k;
            empty_k.prefix = EMPTY;
            empty_k.character = K;

            int find_result_2 = htFind(htp, &empty_k);
            C = find_result_2;
        }
    }

    // output code C (if C != EMPTY)
    if (C != EMPTY) {
        putBits(ht->capbits, C);
    }

    // flush any bits
    flushBits();

    // debug
    // htDump(htp, "");

    // destroy hashtable
    htDestroy(htp);

    // exit
    return;
}

void decode() {
    // file
    // FILE* f = fopen("tests/binary/encode.out", "r");

    // get protection code
    int protection_code = getBits(DEFAULT_MAXBITS);
    if (protection_code != PROTECT_SIGNAL) {
        fprintf(stderr, "could not decode: corrupted archive (protection code)\n");
        return;
    }

    // get maxbits
    // int maxbits = myGetBits(f, START_BITS);
    int maxbits = getBits(START_BITS);

    // create hashtable
    hashtable* ht = htCreate(ABSOLUTE_MINBITS, maxbits); // start at 2 ^ 9 and ceiling of maxbits;
    hashtable** htp = &ht;

    // initialize with RESIZE SIGNAL and the ASCII codes
    htInitTable(htp);

    int newC;
    int C;
    int finalK;

    stack* Kstack = sCreate();

    int oldC = EMPTY;
    // while((newC = C = getCode(NULL)) != EOF) {
    while((newC = C = getBits(ht->capbits)) != EOF) {


        if (C == RESET_SIGNAL) {

            htReset(htp);

            // update variables
            oldC = EMPTY;

            continue;

        }
        

        if (C == RESIZE_SIGNAL) {
            // fprintf(stderr, "GROW: %i\n", ht->size);

            int* conversion_table = htGrow(htp);

            // fprintf(stderr, "GROW: %i\n", ht->size);

            // update variables
            oldC = conversion_table[oldC];

            free(conversion_table);

            continue;
        }

        // if C is an unknown code
        if (!KNOWN(C, htp)) {
            
            // push finalK onto Kstack
            sPush(Kstack, finalK);
            C = oldC;
        }

        // while PREF(C) != EMPTY
        while(PREF(C, htp) != EMPTY) {

            int prefix =  PREF(C, htp);

            // error check
            if(prefix == -1) {
                fprintf(stderr, "could not decode: corrupted archive (prefix)\n");
                sDestroy(Kstack);
                htDestroy(htp);
                return;
            }

            // push CHAR(C) onto Kstack
            int character = CHAR(C, htp);
            sPush(Kstack, character);

            // C = PREF(C)
            C = prefix;
        }
        
        finalK = CHAR(C, htp);
        if (finalK < 0 || finalK > 255) {
            fprintf(stderr, "could not decode: corrupted archive (finalK)\n");
            sDestroy(Kstack);
            htDestroy(htp);
            return;
        }

        putchar(finalK);
        
        // while Kstack is nonempty
        while(Kstack->head != NULL) {
            // Pop K off Kstack
            node* pop = sPop(Kstack);
            putchar(pop->data);
            free(pop);
        }

        // If oldC != EMPTY
        if(oldC != EMPTY) {
            // insert the pair (oldC, finalK) into the table
            entry insert;
            insert.prefix = oldC;
            insert.character = finalK;

            // attempt adding 
            // if resize is needed, then the hashtable will grow (and there will be no addition to the table)
            //int result = htAdd(htp, &insert);

            // htAdd(htp, &insert);
            if (htCanAdd(htp)) {
                htAdd(htp, &insert);
            }

            // fprintf(stderr, "GROW: %i\n", ht->size);
        }

        oldC = newC;
    }

    // debug 
    // htDump(htp, "");

    // file (testing)
    // fclose(f);

    // free stack
    sDestroy(Kstack);

    // destroy hashtable and stack
    htDestroy(htp);
    
}
