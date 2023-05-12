#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "isset.h"

void generic_error(void);

int main(int argc, char **argv)
{
    system("rm -r awesome_tests");
    printf("Welcome to the AwesomeTest, an arbitrarily named unit test...\n");

    // test 1
    printf("[UNIT/1] Testing Grundy\n");
    char* expected_unit_1 = "[0, 0, 0, 1, 0, 2, 1, 0, 2, 1, 0, 2, 1, 3, 2, 1, 3, 2, 4, 3, 0, 4, 3, 0, 4, 3, 0, 4, 1, 2, 3, 1, 2, 4, 1, 2, 4, 1, 2, 4, 1, 5, 4]\n";
    system("mkdir awesome_tests");
    system("./Grundy 42 > awesome_tests/unit1.txt");
    FILE *unit_1_file;
    unit_1_file=fopen("awesome_tests/unit1.txt", "r");
    if(unit_1_file == NULL) { generic_error(); }
    size_t unit_1_len = 0;
    char* unit_1_ptr = NULL;
    getline(&unit_1_ptr, &unit_1_len, unit_1_file);
    assert(strcmp(unit_1_ptr, expected_unit_1) == 0);
    printf("[UNIT/1] PASSED\n");
    free(unit_1_ptr);
    fclose(unit_1_file);

    // end all tests
    system("rm -r awesome_tests");




}

void generic_error(void)
{
    printf("GENERIC ERROR. EXITING...\n");
}
