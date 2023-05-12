#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "ldigraph.h"

/**
 * imports a file with graph data to an ldigraph struct
 * @param  path the path to the location of the file
 * @return      a pointer to an ldigraph
 */
ldigraph* file_to_ldigraph(char* path);

/**
 * checks if a -flag is one of -breadth, -depth or -degree
 * @param  flag a string
 * @return      true if valid, false if not
 */
bool valid_method(char* flag);

/**
 * checks if an arg is an integer
 * @param  flag a string
 * @return      true if valid, false if not
 */
bool valid_vertex(char* flag);

/**
 * a wrapper function to search a direct graph using a method
 * @param g      pointer to a directed graph
 * @param method a function ptr which can be bfs, dfs or ofs
 */
void search(const ldigraph *g, ldig_search *(method)(const ldigraph *, int), int from, int to, char type);


int main(int argc, char **argv)
{
    if(argc < 5)
    {
        printf("Usage: ./Paths path_to_file -flag from_vertex to_vertex\n");
        return 1;
    }

    // read in the graph
    char* graph_data_path = argv[1];
    ldigraph* g = file_to_ldigraph(graph_data_path);
    if(g == NULL) { printf("Paths/main: could not read graph\n"); return 1; }

    // start reading in flags from the third flag onwards
    for(int i = 2; i < argc; i++)
    {
        // check if a flag exists
        if(valid_method(argv[i]))
        {
            // there must be atleast two more args following the flag
            if(i + 2 < argc)
            {
                // get the from vertex
                int from = 0;
                if(valid_vertex(argv[i + 1]))
                {
                    from = atoi(argv[i + 1]);
                }
                else
                {
                    // if the next argument is not a valid vertex
                    // that means you got a flag with no arguments,
                    // so just skip to the next argument
                    continue;
                }

                // get all the to vertices
                int j = i + 2;
                while(j < argc && !valid_method(argv[j]) && valid_vertex(argv[j]))
                {
                    // convert the string to an integer
                    int to = atoi(argv[j]);

                    // depending on what flag was preceding, run the search
                    if(strcmp(argv[i], "-breadth") == 0)
                    {
                        search(g, ldigraph_bfs, from, to, 'b');
                    }
                    else if(strcmp(argv[i], "-depth") == 0)
                    {
                        search(g, ldigraph_dfs, from, to, 'd');
                    }
                    else if(strcmp(argv[i], "-degree") == 0)
                    {
                        search(g, ldigraph_ofs, from, to, 'o');
                    }

                    // increment j for next iteration
                    j++;
                }
            }
            else
            {
                printf("Paths: must be atleast two args following a method\n");
                ldigraph_destroy(g);
                return 1;
            }
        }
    }

    // clean up and return
    ldigraph_destroy(g);
    return 0;
}

ldigraph* file_to_ldigraph(char* path)
{
    FILE* graph_data_file = fopen(path, "r");
    if(graph_data_file == NULL)
    {
        printf("Paths/file_to_ldigraph: Could not create file ptr\n");
        return NULL;
    }

    // prepare a digraph struct
    ldigraph* g = NULL;

    // simple way to accumulate edges
    bool first = false;
    int edge[2] = {0, 0};

    // read line by line
    bool number_seen = false;
    int number_vertices = 0;

    size_t len = 0;
    char* line = NULL;
    while(getline(&line, &len, graph_data_file) != -1)
    {
        // remove the \n char at the end
        len = strlen(line);
        if(line[len - 1] == '\n') { line[len - 1] = '\0'; }

        // tokenize by space
        char* tok_ptr = strtok(line, " ");
        while(tok_ptr != NULL)
        {
            // printf("tok_ptr is: %s\n", tok_ptr);

            if(!number_seen)
            {
                number_vertices = atoi(tok_ptr);
                g = ldigraph_create(number_vertices);
                if(g == NULL) { printf("Paths/file_to_ldigraph: could not allocate ldigraph \n"); return NULL; }

                // graph created
                number_seen = true;
            }
            // if the first edge has not been added already
            else if(!first)
            {
                edge[0] = atoi(tok_ptr);
                first = true;
            }
            // if the first edge has been added already
            else
            {
                edge[1] = atoi(tok_ptr);
                ldigraph_add_edge(g, edge[0], edge[1]);

                first = false;
            }

            // get the next token
            tok_ptr = strtok(NULL, " ");
        }
    }
    fclose(graph_data_file);
    free(line);
    return g;
}

bool valid_method(char* flag)
{
    if(strcmp(flag, "-breadth") == 0 || strcmp(flag, "-depth") == 0 || strcmp(flag, "-degree") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool valid_vertex(char* flag)
{
    if(strcmp(flag, "0") == 0 || atoi(flag) != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void search(const ldigraph *g, ldig_search *(method)(const ldigraph *, int), int from, int to, char type)
{
    // ldigraph_bfs/dfs all return a pointer to a search struct
    ldig_search *s = method(g, from);

    // if the search result is not NULL
    if(s != NULL)
    {
        if(type == 'b')
        {
            printf("-breadth ");
        }
        else if(type == 'd')
        {
            printf("-depth ");
        }
        else if(type == 'o')
        {
            printf("-degree ");
        }
        else
        {
            printf("-error: ");
        }

        int len;
        int *path = ldig_search_path(s, to, &len);
        if(path != NULL)
        {
            printf("%d->%d: [", from, to);
            for (int i = 0; i <= len; i++)
            {
                if (i > 0)
                {
                    printf(", ");
                }
                printf("%d", path[i]);
            }
            printf("]\n");
            free(path);
        }
        else
        {
            printf("%d->%d: no path\n", from, to);
        }

        ldig_search_destroy(s);
    }
}
