#include "helper.h"

#define MAX_NBITS ((sizeof(long) - 1) * CHAR_BIT)

bool commandLineParse(int argc, char** argv, int* m_value, int* r_block, double* r_ratio) {
    char* m_arg = NULL;
    char* r_arg = NULL;

    int c;
    while ((c = getopt(argc, argv, "m:r:")) != -1)  {
        switch (c) {
            case 'm':
                m_arg = optarg;
                break;
            case 'r':
                r_arg = optarg;
                break;
            case '?':
                if (optopt == 'm' || optopt == 'r') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }
                else {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
                return false;
            default:
                return false;
        }
    }

    // for (int i = optind; i < argc; i++) {
    //     fprintf(stderr, "Non-option argument %s\n", argv[i]);
    //     return false;
    // }

    if (m_arg != NULL) {
        if (atoi(m_arg) <= 0) {
            fprintf(stderr, "-m cannot be less than or equal to 0\n");
            return false;
        } else {
            *m_value = atoi(m_arg);
        }
    }

    if (r_arg != NULL) {
        if (atoi(r_arg) <= 0) {
            fprintf(stderr, "-r cannot be less than or equal to 0\n");
            return false;
        } else {

            // split into block and ratio
            sscanf(r_arg, "%i%lf", r_block, r_ratio);

            // if either of them unparsed, then return false
            if (*r_block == -1 || *r_ratio < 0) {
                fprintf(stderr, "-r format: BLOCK.RATIO\n");
                return false;
            }

            // *r_value = atoi(r_arg);
        }
    }

    return true;
}

/**
 * @brief : basically does 2^x, where x = bits
 * 
 * @param bits : want 2^4? the function will do 2 << 5
 * @return int 
 */
int bit2Dec(int bits) {
    return (2 << (bits - 1));
}

int myStage(void) {
    char* s = getenv("STAGE");
    if (s && !strcmp(s, "1"))
        return 1;
    else if (s && !strcmp(s, "2"))
        return 2;
    else
        return 3;
}

int myGetBits(FILE* f, int nBits) {
    if (myStage() == 1) {
        int code;
        if (fscanf(f, "%d\n", &code) < 1) {  // No code?
            return EOF;

        } else if (code < 0 || (1 << nBits) <= code) {  // Not within bounds?
            fprintf(stderr, "nBits = %d, code = %d\n", nBits, code);
            return EOF;

        } else {
            return code;
        }

    } else if (myStage() == 2) {
        int code, nRcvd;
        if (fscanf(f, "%d:%d\n", &nRcvd, &code) < 2  // No NRCVD:CODE or
            || nRcvd < nBits) {                  //   too few bits?
            return EOF;

        } else if (nRcvd > nBits  // Too many bits or
                   || (code < 0   //   not within bounds?
                       || (1 << nBits) <= code)) {
            fprintf(stderr, "nBits = %d, nRcvd = %d, code = %d\n", nBits, nRcvd,
                    code);
            return EOF;

        } else {
            return code;
        }
    }

    int c;
    static int nExtra = 0;           // #bits from previous byte(s)
    static long unsigned extra = 0;  // Extra bits from previous byte(s)

    if (nBits > MAX_NBITS)
        exit(fprintf(stderr, "getBits: nBits = %d too large\n", nBits));

    // Read enough new bytes to have at least NBITS bits to extract code
    while (nExtra < nBits) {
        if ((c = getchar()) == EOF)  // Too few bits?
            return EOF;
        nExtra += CHAR_BIT;
        extra = (extra << CHAR_BIT) | c;
    }
    nExtra -= nBits;  // Return nBits bits
    c = extra >> nExtra;
    extra ^= c << nExtra;  // Save remainder
    return c;
}
