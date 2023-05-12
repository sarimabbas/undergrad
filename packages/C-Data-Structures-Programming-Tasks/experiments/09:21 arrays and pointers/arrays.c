#include <stdlib.h>
#include <stdio.h>

int main(void)
{

    int i = 42;

    int* p1;
    p1 = &i;

    int* p2 = &i;
    int* p3 = p2;
    
    printf("%i, %i, %i\n", *p1, *p2, *p3);

    // array with array style initialisation (size is optional)
    int int_array1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // regular array access
    printf("%i\n", int_array1[2]);
    // treating the variable as a pointer and doing pointer arithmetic
    printf("%i\n", *(int_array1 + 1));

    // pointer with array style initialisation DOES NOT WORK
    // int* int_array2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // and printing fails
    // printf("%i\n", *(int_array2 + 1));

    // array with array style initialisation
    char char_array1[5] = {'J', 'e', 'l', 'l', 'y'};

    // array with string literal initialisation
    char char_array2[] = "Jelly";

    // pointer with string literal initialisation
    char* char_array3 = "Jelly";

    // pointer with array style initialisation DOES NOT WORK
    // char* char_array4 = {'J', 'e', 'l', 'l', 'y'};

    printf("%c\n", *(char_array1 + 1));
    printf("%c\n", *(char_array2 + 1));
    printf("%c\n", *(char_array3 + 1));
    // and printing for char_array4 fails
    // printf("%c\n", *(char_array4 + 1));
}
