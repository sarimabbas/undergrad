/**
 * A modified version of atoi.
 * @param  string to convert into integer
 * @param  store  to put the converted integer into
 * @return        'n' for negative number / 'e' for failure to convert / 's' for success
 */
char modAtoi(char* string, int* store);

/**
 * Turns {1, 2, 3} into {3, 2, 1}
 * @param  array      source array
 * @param  flip_array destination array with flipped numbers
 * @param  array_size size of source and destination arrays
 * @return            true for success / false for failure
 */
bool flipArray(int* array, int* flip_array, int array_size);

/**
 * Copy one array to another
 * @param  array_source      source array
 * @param  array_destination destination array
 * @param  array_size        size of source and destination arrays
 * @return                   true for success / false for failure
 */
bool arrayCopy(int* array_source, int* array_destination, int array_size);

/**
 * Bubble sort an array
 * @param  array      array to sort
 * @param  array_size size of the array
 * @return            true for success / false for failure
 */
bool bubbleSortArray(int* array, int array_size);

/**
 * Populates an array with numbers from 0 to array_size - 1
 * @param  array      pointer to an integer array
 * @param  array_size size of the array
 * @return            true for success / false for failure
 * example: ./NoAP 6 = [0, 1, 2, 3, 4, 5]
 */
bool arrayPopulate(int* array, int array_size);

/**
 * Creates an empty array of a specified size full of placeholder values
 * @param  array      array to modify
 * @param  array_size size of the array
 * @return            true for success / false for failure
 */
bool emptyArrayPopulate(int* array, int array_size);

/**
 * Prints the array according to specification
 * @param  array      array to print
 * @param  array_size size of array to print
 * @param  method     'g' for greedy, 'b' for backward, 's' for skip, 'o' for opt
 * @param  first      used for printing skip
 * @param  step       used for printing skip
 * @param  empty      if 'y' then will print -method: 0 []
 * @return            true for success / false for failure
 */
bool arrayPrint(int* array, int array_size, char method, int first, int step, char empty);

/**
 * Print an array in the form [x1, x2, x3]
 * @param  array      array to print
 * @param  array_size size of array
 * @return            true for success / false for failure
 */
bool debugTruePrint(int* array, int array_size);

/**
 * Modifies available numbers so that it has only choices > the largest must include
 * Modifies progression free so that it starts with must include numbers
 * @param  must_include           must include numbers
 * @param  must_include_size      size of the must include array
 * @param  available_numbers      available numbers array
 * @param  progression_free       array to start with
 * @return                        true for success / false for failure
 */
bool mustIncludeModifier(int* must_include, int must_include_size,
    int* available_numbers, int* progression_free);


/**
 * A wrapper for makeProgressionFree using greedy approach
 * @param  progression_free       array to work on
 * @param  progression_free_size  size of array
 * @param  available_numbers      available choices
 * @param  available_numbers_size size of array
 * @return                        true for success / false for failure
 */
bool greedy(int* progression_free, int progression_free_size,
    int* available_numbers, int available_numbers_size);

/**
 * A wrapper for makeProgressionFree using backward approach
 * @param  progression_free       array to work on
 * @param  progression_free_size  size of array
 * @param  available_numbers      available choices
 * @param  available_numbers_size size of array
 * @return                        true for success / false for failure
 */
bool backward(int* progression_free, int progression_free_size,
    int* available_numbers, int available_numbers_size);


/**
 * A wrapper for makeProgressionFree using skip approach
 * @param  progression_free       array to work on
 * @param  progression_free_size  size of array
 * @param  available_numbers      available choices
 * @param  available_numbers_size size of array
 * @param  start                  index to begin in the choices
 * @param  step                   step to take between choices
 * @return                        true for success / false for failure
 */
bool skip(int* progression_free, int progression_free_size,
    int* available_numbers, int available_numbers_size,
    int start, int step);

/**
 * Finds a set free of arithmetic progressions
 * @param  progression_free       array to work on
 * @param  progression_free_size  size of array
 * @param  available_numbers      available choices
 * @param  available_numbers_size size of array
 * @param  start                  index to begin in the choices
 * @param  step                   step to take between choices
 * @param  wrap                   'y'/'n' for wrapping around choices
 * @return                        true for success / false for failure
 */
bool makeProgressionFree(int* progression_free, int progression_free_size,
int* available_numbers, int available_numbers_size,
int start, int step, char wrap);

/**
 * Checks for arithmetic progressions amongst numbers in array
 * @param  array      array to check
 * @param  array_size size of array
 * @return            true for success / false for failure
 */
bool hasProgression(int* array, int array_size);
