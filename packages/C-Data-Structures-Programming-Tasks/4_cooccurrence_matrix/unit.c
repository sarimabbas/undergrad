#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "smap.h"
#include "cooccur.h"

int djb2(const char *key);
char *random_string(int len);
void for_each_test(const char* key, int* value);
void for_each_test_r(const char* key, int* value, void* arg);

int main(void)
{
    // test random_string() and djb2()
    char* test_random = random_string(10);
    printf("[UNIT/RANDOM_STRING] RANDOM STRING: %s\n", test_random);
    int test_hash = djb2(test_random);
    printf("[UNIT/DJB2] RANDOM STRING HASH: %i\n", test_hash);
    free(test_random);

    // testing smap.c
    smap* test_map = smap_create(djb2);
    if(test_map != NULL) { printf("[UNIT] MAP CREATED SUCCESSFULLY\n"); }
    printf("[UNIT] MAP SIZE: %i\n", smap_size(test_map));

    // add a value
    char* test_key = "sarim"; // hash: 274146753 mod 100 = 53
    int test_value = 20;
    bool put_result = smap_put(test_map, test_key, &test_value);
    if(put_result) { printf("[UNIT] SMAP_PUT RETURNED TRUE\n");}
    printf("[UNIT] MAP SIZE: %i\n", smap_size(test_map));
    bool found_1 = smap_contains_key(test_map, "sarim");
    if(found_1) { printf("[UNIT] KEY WAS FOUND\n"); }

    // add a value
    char* test_key_2 = "abbas";
    int test_value_2 = 10;
    bool put_result_2 = smap_put(test_map, test_key_2, &test_value_2);
    if(put_result_2) { printf("[UNIT] SMAP_PUT RETURNED TRUE\n");}
    printf("[UNIT] MAP SIZE: %i\n", smap_size(test_map));
    bool found_2 = smap_contains_key(test_map, "abbas");
    if(found_2) { printf("[UNIT] KEY WAS FOUND\n"); }

    // add another value
    // in this case, we are using djb2 to generate some garbage value
    char* random_string_1 = random_string(5);
    int garbage_value = djb2(random_string_1);
    char* random_key = random_string(10);
    printf("[UNIT] GENERATED KEY IS %s\n", random_key);
    smap_put(test_map, random_key, &garbage_value);
    printf("[UNIT] MAP SIZE: %i\n", smap_size(test_map));

    printf("/////////////\n");

    // for each test
    smap_for_each(test_map, for_each_test);

    // for each arg test
    int arg_value = 3;
    smap_for_each_r(test_map, for_each_test_r, (void*) &arg_value);

    // destroy test map
    smap_destroy(test_map);

    // free random strings / key
    free(random_string_1);
    free(random_key);

    return 0;
}

void for_each_test(const char* key, int* value)
{
    printf("THE KEY IS: %s\n", key);
    printf("THE VALUE IS: %i\n", *value);
}

void for_each_test_r(const char* key, int* value, void* arg)
{
    int* retrieve_arg = (int*) arg;
    printf("THE KEY IS: %s\n", key);
    printf("THE VALUE IS: %i\n", *value);
    printf("THE ARG IS: %i\n", *retrieve_arg);
}

int djb2(const char *key)
{
    int hash = 5381;
    int c;
    while ((c = *key++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

char *random_string(int len)
{
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    char *rstr = malloc((len + 1) * sizeof(char));
    int i;
    srand ( time(NULL) );
    for (i = 0; i < len; i++)
    {
        rstr[i] = alphabet[(rand() % (strlen(alphabet)))];
    }
    rstr[len] = '\0';
    return rstr;
}
