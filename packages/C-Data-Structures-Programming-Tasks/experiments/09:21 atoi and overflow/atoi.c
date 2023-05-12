#include <stdio.h>
#include <stdlib.h>
/* This program can be used to show how atoi etc. handle overflow. */
/* For example, try "overflow 1000000000000". */
int
main(int argc, char **argv)
{
        char c;
        int i;
        long l;
        long long ll;
        if(argc != 2) {
                fprintf(stderr, "Usage: %s n\n", argv[0]);
                return 1;
        }
        c = atoi(argv[1]);
        i = atoi(argv[1]);
        l = atol(argv[1]);
        ll = atoll(argv[1]);

        printf("Char is: %c\n", c);

        int test_char = 97;
        printf("Char is: %c\n", test_char);

        return 0;
}
