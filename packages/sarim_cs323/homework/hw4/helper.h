#define _GNU_SOURCE

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

/**
 * @brief : parse command line arguments
 * 
 * @param argc 
 * @param argv 
 * @return true : if parsed
 * @return false 
 */
bool commandLineParse(int argc, char** argv, int* m_value, int* r_block, double* r_ratio);

int bit2Dec(int bits);

int myGetBits(FILE* f, int nBits);

int stage(void);
