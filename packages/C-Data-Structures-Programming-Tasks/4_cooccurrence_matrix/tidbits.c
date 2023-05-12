// Copying command line arguments

int key_count = argc - 1;
char** keys = malloc(sizeof(char*) * key_count);
int key_advance = 0;
for(int i = 1; i < argc; i++)
{
    char* key = argv[i];
    keys[key_advance] = key;
    key_advance = key_advance + 1;
}

qsort(keys, key_count, sizeof(char*), normal_string_compare);

for(int i = 0; i < key_count; i++)
{
    printf("key[%i] is %s\n", i, keys[i]);
}


/*
int n = 9;
while(n != 0)
{
    int stripped_size;
    char** stripped = cooccur_read_context(my_cooccur, stdin, &stripped_size);
    if(stripped != NULL)
    {
        printf("the stripped size is %i\n", stripped_size);
        for(int i = 0; i < stripped_size; i++)
        {
            printf("%s\n", stripped[i]);
        }
        cooccur_update(my_cooccur, stripped, stripped_size);
        free(stripped);
    }
    n = n - 1;
}
*/
*
// int stripped_size;;
// char** stripped = cooccur_read_context(my_cooccur, stdin, &stripped_size);
// printf("the stripped size is %i\n", stripped_size);
// for(int i = 0; i < stripped_size; i++)
// {
//     printf("%s\n", stripped[i]);
// }

// cooccur_update(my_cooccur, stripped, stripped_size);
// free(stripped);


// sort the keys
qsort(keywords, keyword_count, sizeof(char*), normal_string_compare);

/*for(int i = 0; i < keyword_count; i++)
{
    printf("keywords[%i] is %s\n", i, keywords[i]);
}*/

// remove duplicates
char** temp = malloc(sizeof(char*) * keyword_count);
temp[0] = keywords[0];
int temp_advance = 1;
for(int i = 1; i < keyword_count; i++)
{
    if(strcmp(keywords[i], keywords[i - 1]) != 0)
    {
        temp[temp_advance] = keywords[i];
        temp_advance = temp_advance + 1;
    }
}
keywords = realloc(temp, sizeof(char*) * temp_advance);
keyword_count = temp_advance;


// remove duplicates
char** temp = malloc(sizeof(char*) * keyword_count);
bool found = false;
int temp_advance = 0;
for(int i = 0; i < keyword_count; i++)
{
    for(int j = i + 1; j < keyword_count; j++)
    {
        if(strcmp(keywordz[i], keywordz[j]) == 0)
        {
            found = true;
        }
    }

    if(!found)
    {
        temp[temp_advance] = keywordz[i];
        temp_advance = temp_advance + 1;
    }
    else
    {
        found = false;
    }
}
char** keywords = realloc(temp, sizeof(char*) * (temp_advance + 1));
keyword_count = temp_advance;


/// read context backup

/**
 * Reads keywords from the given matrix from the current line of the given stream.
 * A keyword is considered to have been read if it appears on the stream before
 * the next newline surrounded by whitespace, the beginning of input (as seen by
 * this function), or EOF.  The newline is read and removed from the stream.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 * @param stream a stream, non-NULL
 * @param n a pointer to an int where the size of the returned array will be written
 * @return an array of unique non-NULL strings containing all the keywords read;
 * the caller is responsible for deallocating the array and the strings it contains
 *
 * cooccur_read_context: O(m) where m is the number of words in the context
 */
char **cooccur_read_context(cooccurrence_matrix *mat, FILE *stream, int *n)
{
    // the magic starts here
    smap* keyword_orders = mat->keyword_orders;

    // read the line
    size_t len = 0;
    char* line = NULL; // maybe try setting this to NULL?
    int result = getline(&line, &len, stream);
    if(result != -1)
    {
        // get length of string
        len = strlen(line);

        // remove the \n char at the end
        if(line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        // prepare an array of substrings
        char* substring[len];
        int index = -1;

        // tokenize by space
        // char* tok_ptr = line;
        substring[++index] = strtok(line, " ");
        while((substring[++index] = strtok(NULL, " ")) != NULL) { }

        // printf("line is now pointing to %s\n", line);

        /*
        for(int i = 0; i < index; i++)
        {
            printf("substring[%i]: %s\n", i, substring[i]);
        }
        */


        // begin checking the substrings to remove duplicates
        int stripped_size = 0;

        // a new array of strings as big as the tokens
        char** stripped = malloc(sizeof(char*) * index);

        // iterate through all the tokens/substrings
        for(int j = 0; j < index; j++)
        {
            // check if the substring is in the keywords
            bool check = smap_contains_key(keyword_orders, substring[j]);
            if(check)
            {
                // printf("%s is inside the map\n", substring[j]);
                int* order = smap_get(keyword_orders, substring[j]);
                // printf("ORDER IS %i\n", order[0]);
                // ask if this word has been seen before
                if(order[0] == 0)
                {
                    // printf("the found substring is %s\n", substring[j]);
                    stripped[stripped_size] = strdup(substring[j]);
                    // printf("Address of substring[%i] %i, Address of stripped[%i] %i\n", j, substring[j], stripped_size, stripped[stripped_size]);

                    stripped_size = stripped_size + 1;
                    order[0] = order[0] + 1;
                }
            }
            else
            {
                // printf("the unfound substring is %s\n", substring[j]);
            }
            // free(substring[j]);
        }

        /*// free substrings
        for(int i = 0; i < index; i++)
        {
            free(substring[i]);
        }*/

        // if no words have been added, just return NULL
        if(stripped_size == 0)
        {
            *n = 0;
            free(stripped);
            free(line);
            return NULL;
        }

        /*// set the rest to NULL
        for(int k = stripped_size; k < index; k++)
        {
            stripped[k] = NULL;
        }*/

        // truncate the size of stripped if needed
        stripped = realloc(stripped, sizeof(char*) * stripped_size);

        /*printf("[READ_CONTEXT] PRINTING STRIPPED\n");
        for(int i = 0; i < stripped_size; i++)
        {
            printf("%s\n", stripped[i]);
        }
        printf("made it so far\n");
        printf("the first one is %s\n", stripped[0]);*/

        // update the size
        *n = stripped_size;

        // reset orders for next line
        smap_for_each(keyword_orders, reset_order);

        // free(line); this is making stuff break!

        // remember to free stripped in main!
        return stripped;
    }
    else
    {
        // if getline did not work
        *n = -1;
        return NULL;
    }
}
