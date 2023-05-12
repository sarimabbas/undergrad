#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>

#include "ldigraph.h"
#include "queue.h"

struct ldigraph
{
    int n;          // the number of vertices
    int *list_size; // the size of each adjacency list
    int *list_cap;  // the capacity of each adjacency list
    int **adj;      // the adjacency lists
};

struct ldig_search
{
    const ldigraph *g; // ptr to a digraph
    int from;          // vertex to start from
    int *color;
    int *dist;
    int *pred;
};

enum {WHITE, GRAY, BLACK};

#define LDIGRAPH_ADJ_LIST_INITIAL_CAPACITY 4

/**
 * Resizes the adjacency list for the given vertex in the given graph.
 *
 * @param g a pointer to a directed graph
 * @param from the index of a vertex in that graph
 */
void ldigraph_list_embiggen(ldigraph *g, int from);

/**
 * Visits the given vertex in the given search of the given graph.
 *
 * @param g a pointer to a directed graph
 * @param s a search in that graph
 * @param from a vertex in that graph
 */
void ldigraph_dfs_visit(const ldigraph* g, ldig_search *s, int from);

/**
 * Prepares a search result for the given graph starting from the given
 * vertex.  It is the responsibility of the caller to destroy the result.
 *
 * @param g a pointer to a directed graph
 * @param from the index of a vertex in that graph
 * @return a pointer to a search result
 */
ldig_search *ldig_search_create(const ldigraph *g, int from);

/**
 * Visits the given vertex in the given search of the given graph, prioritizing
 * outdegree
 *
 * @param g a pointer to a directed graph
 * @param s a search in that graph
 * @param from a vertex in that graph
 */
void ldigraph_ofs_visit(const ldigraph* g, ldig_search *s, int from);

/**
 * Comparator function for qsort to sort vertices by outdegree
 * @param  vertex_1 arbitrary first vertex (int*)
 * @param  vertex_2 arbitrary second vertex (int*)
 * @param  graph    a pointer to a directed graph
 * @return          1 if outdegree(vertex_1) < outdegree(vertex_2), -1 for flipcase
 */
int outdegree_compare(const void* vertex_1, const void* vertex_2, void* graph);

/**
 * debuggin: Slower bubble sort for sorting vertices by outdegree
 * @param g          a pointer to a directed graph
 * @param array      the array of vertices to sort
 * @param array_size the size of the array
 */
void outdegree_bubble_sort(ldigraph* g, int* array, int array_size);

/**
 * Compute the outdegree of a vertex
 * @param  g      a pointer to a directed graph
 * @param  vertex vertex to compute outdegree for
 * @return        the outdegree of the vertex
 */
int outdegree(ldigraph* g, int vertex);

/**
 * debugging: prints the color of the vertex
 * @param v a vertex in an ldigraph
 * @param s a search struct
 */
void printVertexColor(int v, ldig_search* s);

/**
 * debugging: prints the entirety of a search struct
 * @param s a search struct
 */
void printSearch(ldig_search* s);


ldigraph *ldigraph_create(int n)
{
    // if no vertices specified, then return the NULL graph
    if (n < 1)
    {
        return NULL;
    }

    // create space for the digraph
    ldigraph *g = malloc(sizeof(ldigraph));
    if (g != NULL)
    {
        g->n = n;
        g->list_size = malloc(sizeof(int) * n);
        g->list_cap = malloc(sizeof(int) * n);
        g->adj = malloc(sizeof(int *) * n);

        // if the graph creation failed
        if (g->list_size == NULL || g->list_cap == NULL || g->adj == NULL)
        {
            free(g->list_size);
            free(g->list_cap);
            free(g->adj);
            free(g);

            return NULL;
        }

        // if the malloc worked then
        // for the number of vertices
        for (int i = 0; i < n; i++)
        {
            // set the size of each list to 0
            g->list_size[i] = 0;
            // create each list of ints
            g->adj[i] = malloc(sizeof(int) * LDIGRAPH_ADJ_LIST_INITIAL_CAPACITY);
            // set the list capacity for each vertex as the initial cap if malloc succeeded
            g->list_cap[i] = g->adj[i] != NULL ? LDIGRAPH_ADJ_LIST_INITIAL_CAPACITY : 0;
        }
    }

    // return the digraph
    return g;
}

int ldigraph_size(const ldigraph *g)
{
    // if the graph is not null
    if (g != NULL)
    {
        // return the size of the graph
        return g->n;
    }
    else
    {
        return 0;
    }
}

void ldigraph_list_embiggen(ldigraph *g, int from)
{
    // from: the index of a vertex in the given digraph
    // get the vertex at this index if it has a capacity for the adjacency list
    if (g->list_cap[from] != 0)
    {
        // access the adjacency list for that vertex
        // reallocate the adjacency list and double its capacity
        g->adj[from] = realloc(g->adj[from], sizeof(int*) * g->list_cap[from] * 2);
        // set the new capacity as the existing capacity * 2
        g->list_cap[from] = g->adj[from] != NULL ? g->list_cap[from] * 2 : 0;
    }
}

void ldigraph_add_edge(ldigraph *g, int from, int to)
{
    // if the digraph exists,
    // and the from and to vertices are inside the adj array
    if (g != NULL && from >= 0 && to >= 0 && from < g->n && to < g->n && from != to)
    {
        // make room if necessary
        // if the adjacency list is at capacity
        if (g->list_size[from] == g->list_cap[from])
        {
            ldigraph_list_embiggen(g, from);
        }

        // add to end of array if there is room
        // if the adjacency list is not at capacity
        if (g->list_size[from] < g->list_cap[from])
        {
            // go to the adjacency list at the from index
            // go to the available sub index as specified by the list_size
            // set it to equal "to"
            g->adj[from][g->list_size[from]++] = to;
            // also post-increment list_size instead of in a separate step
        }
    }
}

bool ldigraph_has_edge(const ldigraph *g, int from, int to)
{
    // from and to must be within the bounds of the adjacency list
    if (g != NULL && from >= 0 && to >= 0 && from < g->n && to < g->n && from != to)
    {
        // sequential search of from's adjacency list
        int i = 0;
        // while i is less than the size of from
        // and the ith subvalue is not to
        while (i < g->list_size[from] && g->adj[from][i] != to)
        {
            i++;
        }

        // i can be within the list_size, in which case you return true,
        // else return false
        return i < g->list_size[from];
    }
    else
    {
        return false;
    }
}

ldig_search *ldigraph_bfs(const ldigraph *g, int from)
{
    // make sure the digraph exists and that the vertex is inside the adjacency list
    if(g == NULL || from < 0 || from >= g->n)
    {
        return NULL;
    }

    // create a search struct, beginning at the from vertex
    ldig_search *s = ldig_search_create(g, from);

    // if the allocation was unsuccessful
    if(s == NULL)
    {
        return NULL;
    }

    // start at "from"
    s->color[from] = GRAY;
    s->dist[from] = 0;

    // create a integer queue and push "from" onto it
    queue* q = createQueue();
    addToBack(q, from);

    while(!isQueueEmpty(q))
    {
        // make a copy of the front of the queue
        int front = getFront(q);

        // remove the front from the queue
        removeFromFront(q);

        // check if the vertex lies in the bounds of our graph
        if(front >= 0 && front < g->n)
        {
            // get all neighbors of the front vertex
            int* neighbors = g->adj[front];
            int neighbors_len = g->list_size[front];

            // mark neighbors as explored and add them to the queue
            for(int i = 0; i < neighbors_len; i++)
            {
                // if the neighbors were unexplored
                if(s->color[neighbors[i]] == WHITE)
                {
                    // explore them
                    s->color[neighbors[i]] = GRAY;

                    // add them to the queue
                    addToBack(q, neighbors[i]);

                    // update their predecessors with the current front of queue
                    s->pred[neighbors[i]] = front;

                    // update distances to get to the neighbor
                    s->dist[neighbors[i]] = s->dist[front] + 1;
                }
            }

            // mark current vertex as complete / BLACK
            s->color[front] = BLACK;
        }
    }
    // clean up and return search
    destroyQueue(q);
    return s;
}

ldig_search *ldigraph_dfs(const ldigraph *g, int from)
{
    // make sure the digraph exists and that the vertex is inside the adjacency list
    if (g == NULL || from < 0 || from >= g->n)
    {
        return NULL;
    }

    // create a search struct, beginning at the from vertex
    ldig_search *s = ldig_search_create(g, from);

    // if the allocation was successful
    if (s != NULL)
    {
        // start at from
        s->color[from] = GRAY;
        s->dist[from] = 0;

        // pass the digraph, the search struct and the starting vertex to ldigraph_dfs_visit
        ldigraph_dfs_visit(g, s, from);
    }
    return s;
}

void ldigraph_dfs_visit(const ldigraph* g, ldig_search *s, int from)
{
    // get vertices we can reach in one edge from from
    const int *neighbors = g->adj[from];

    // iterate over outgoing edges
    for (int i = 0; i < g->list_size[from]; i++)
    {
        int to = neighbors[i];
        if (s->color[to] == WHITE)
        {
            // found an edge to a new vertex -- explore it
            s->color[to] = GRAY;
            s->dist[to] = s->dist[from] + 1;
            s->pred[to] = from;

            ldigraph_dfs_visit(g, s, to);
        }
    }

    // mark current vertex finished
    s->color[from] = BLACK;
}

ldig_search *ldigraph_ofs(const ldigraph *g, int from)
{
    // make sure the digraph exists and that the vertex is inside the adjacency list
    if(g == NULL || from < 0 || from >= g->n)
    {
        return NULL;
    }

    // create a search struct, beginning at the from vertex
    ldig_search *s = ldig_search_create(g, from);

    // if the allocation was successful
    if (s != NULL)
    {
        // start at from
        s->color[from] = GRAY;
        s->dist[from] = 0;

        // pass the digraph, the search struct and the starting vertex to ldigraph_ofs_visit
        ldigraph_ofs_visit(g, s, from);
    }
    return s;
}

void ldigraph_ofs_visit(const ldigraph* g, ldig_search *s, int from)
{
    // get vertices we can reach in one edge from from
    int *neighbors_copy = g->adj[from];
    int neighbors_len = g->list_size[from];

    int neighbors[neighbors_len];
    for(int i = 0; i < neighbors_len; i ++)
    {
        neighbors[i] = neighbors_copy[i];
    }

    // reorder the vertices in "neighbors" by their outdegree in memory
    // vertices with higher outdegrees considered first
    // suppose we had neighbors = [3, 1], we'd want neighbors = [1, 3] for graph_1.in
    // Note to instructor: please note the function signatures for qsort_r() vary between macOS
    // and Linux. I have implemented in for Zoo / Linux.
    qsort_r(neighbors, neighbors_len, sizeof(int), (int (*) (const void*,const void*, void*)) outdegree_compare, (void*) g);

    // iterate over outgoing edges in the order they appeared in the input file
    for (int i = 0; i < g->list_size[from]; i++)
    {
        // this is the current to vertex we are looking at
        int to = neighbors[i];
        if(s->color[to] == WHITE)
        {
            // found an edge to a new vertex -- explore it
            s->color[to] = GRAY;
            s->dist[to] = s->dist[from] + 1;
            s->pred[to] = from;

            ldigraph_ofs_visit(g, s, to);
        }
    }

    // mark current vertex finished
    s->color[from] = BLACK;
}

void ldigraph_destroy(ldigraph *g)
{
  if (g != NULL)
    {
      for (int i = 0; i < g->n; i++)
	{
	  free(g->adj[i]);
	}
      free(g->adj);
      free(g->list_cap);
      free(g->list_size);
      free(g);
    }
}

ldig_search *ldig_search_create(const ldigraph *g, int from)
{
    // if the from vertex lies inside the adjacency list
    if (g != NULL && from >= 0 && from < g->n)
    {
        // create a new search struct
        ldig_search *s = malloc(sizeof(ldig_search));

        // if the mallocing succeeded
        if (s != NULL)
        {
            // set the digraph for the search struct
            s->g = g;
            // begin search at the from vertex
            s->from = from;
            // create an array of ints, one for each vertex, to hold the color
            s->color = malloc(sizeof(int) * g->n);
            // create an array of ints, one for each vertex, to hold the distance
            s->dist = malloc(sizeof(int) * g->n);
            // create an array of ints, one for each vertex, to hold the predecessor
            s->pred = malloc(sizeof(int) * g->n);

            // if allocations were successful
            if (s->color != NULL && s->dist != NULL && s->pred != NULL)
            {
                // initialize all vertices to unseen
                for (int i = 0; i < g->n; i++)
                {
                    s->color[i] = WHITE;
                    s->dist[i] = -1;
                    s->pred[i] = -1;
                }
            }
            else
            {
                free(s->pred);
                free(s->dist);
                free(s->color);
                free(s);
                return NULL;
            }
        }

        return s;
    }
    else
    {
        return NULL;
    }
}

int *ldig_search_path(const ldig_search *s, int to, int *len)
{
    // make sure the search exists and that the to vertex is inside the adjacency list
    if (s == NULL || to < 0 || to >= s->g->n)
    {
        return NULL;
    }

    // find the distance to the "to" vertex
    int dist_to = s->dist[to];

    // check if path exists
    if(dist_to == -1)
    {
        return NULL;
    }
    else
    {
        // update len of path
        *len = dist_to;

        // create a path to hold the vertices
        int* path = malloc(sizeof(int) * (dist_to + 1));

        // run a loop to store the predecessors of "to"
        int current = to;
        for(int i = dist_to; i >= 0; i--)
        {
            path[i] = current;
            current = s->pred[current];
        }

        return path;
    }
}

void ldig_search_destroy(ldig_search *s)
{
    if (s != NULL)
    {
        free(s->color);
        free(s->dist);
        free(s->pred);
        free(s);
    }
}

int outdegree_compare(const void* vertex_1, const void* vertex_2, void* graph)
{
    // convert to the appropriate types
    ldigraph* g = (ldigraph*) graph;
    int v1 = *(int*) vertex_1;
    int v2 = *(int*) vertex_2;

	if(outdegree(g, v1) < outdegree(g, v2))
	{
        // this will allow v1 to come after v2 in the list
        // backward sort
		return 1;
	}
	else if(outdegree(g, v1) > outdegree(g, v2))
	{
        // this will allow v1 to come before v2 in the list
        // backward sort
		return -1;
	}
	else // if the outdegrees are equal
	{
        // in tie-breakers, we want to peek the vertices
        if(v1 < v2)
        {
            // we want v1 to come before v2 in the list
            return -1;
        }
        else if(v1 > v2)
        {
            // we want v1 to come after v2 in the list
            return 1;
        }
        else // let qsort decide
        {
            return 0;
        }
	}
}


void outdegree_bubble_sort(ldigraph* g, int* array, int array_size)
{
	for(int x = 0; x < array_size; x++)
	{
		for(int y = 0; y < array_size - 1; y++)
		{
			if(outdegree(g, array[y]) < outdegree(g, array[y + 1]))
			{
				int temp = array[y + 1];
				array[y + 1] = array[y];
				array[y] = temp;
			}
            else if(outdegree(g, array[y]) == outdegree(g, array[y + 1]))
            {
                if(array[y] > array[y + 1])
                {
                    int temp = array[y + 1];
    				array[y + 1] = array[y];
    				array[y] = temp;
                }
            }
		}
	}
}

int outdegree(ldigraph* g, int vertex)
{
    return (g->list_size[vertex]);
}

void printVertexColor(int v, ldig_search* s)
{
    if(s->color[v] == GRAY)
    {
        printf("COLOR: GRAY\n");
    }
    else if(s->color[v] == WHITE)
    {
        printf("COLOR: WHITE\n");
    }
    else if(s->color[v] == BLACK)
    {
        printf("COLOR: BLACK\n");
    }
    else
    {
        printf("COLOR: ERROR\n");
    }
}

void printSearch(ldig_search* s)
{
    printf("VERTICES in g: ");
    for(int i = 0; i < s->g->n; i++)
    {
        printf("%i, ", i);
    }
    printf("\n");
    printf("FROM: %i\n", s->from);
    printf("COLORS: ");
    for(int i = 0; i < s->g->n; i++)
    {
        if(s->color[i] == GRAY)
        {
            printf("gray, ");
        }
        else if(s->color[i] == BLACK)
        {
            printf("black, ");
        }
        else
        {
            printf("white, ");
        }
    }
    printf("\n");
    printf("DIST: ");
    for(int i = 0; i < s->g->n; i++)
    {
        printf("%i, ", s->dist[i]);
    }
    printf("\n");
    printf("PRED: ");
    for(int i = 0; i < s->g->n; i++)
    {
        printf("%i, ", s->pred[i]);
    }
    printf("\n");
}
