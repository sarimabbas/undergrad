#include <stdio.h>
int
main(int argc, char **argv)
{
        int c;
        while((c = getchar()) != EOF) {
                switch(c) {
                case 'e':
                case 'E':
                        printf("Exiting...\n");
                        return 0;
                        break;
                default:
                        putchar(c);
                        break;
                }
        }
        return 0;
}

/*

Ways to use:
1) Open program using ./a.out. Then start entering keys one by one.
2) Open program with predetermined input using ./a.out < input.txt.

*/
