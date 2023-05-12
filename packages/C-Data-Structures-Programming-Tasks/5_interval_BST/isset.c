#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "isset.h"

typedef struct _isset_node
{
  int start;
  int end;
  struct _isset_node *parent;
  struct _isset_node *left;
  struct _isset_node *right;
} isset_node;

typedef struct _isset_search_result
{
  isset_node *parent;
  isset_node *n;
  bool found;
  int dir;
  isset_node *last_left;
  isset_node *last_right;
} isset_search_result;

struct isset
{
  isset_node *root;
  int size;
  int nodes;
};

/**
 * Returns a pointer to a new node containing the interval [item, item].
 *
 * @param item an integer
 * @return a pointer to a new node containing the interval [item, item]
 */
isset_node *isset_create_node(int item);

/**
 * Finds the node containing the given integer in the given set's tree,
 * or, if the integer is not there, determines where it would be added.
 *
 * @param s a pointer to a set, non-NULL
 * @param item an integer
 * @param return a pointer to the struct in which to record the results
 * of the search
 */
void isset_find_node(const isset *s, int item, isset_search_result *result);

/**
 * Determines if the given integer comes before, within, or after the given
 * interval.
 *
 * @param s a pointer to a set, non-NULL
 * @param start an integer
 * @param end an integer greater than or equal to start
 * @param item an integer
 * @return a negative integer if item < start, a positive integer if
 * item > start, and 0 if start <= item <= end
 */
int isset_interval_compare(const isset *s, int start, int end, int item);

/**
 * Deletes all of the nodes in the tree rooted at the given node.
 * There is no effect if given pointer is NULL.
 *
 * @param n a pointer to a node
 */
void isset_destroy_subtree(isset_node *n);

/**
 * Merges nodes with overlapping intervals
 *
 * @param  s             a pointer to a set, non-NULL
 * @param  target        a pointer to the target node to merge with last_relevant
 * @param  last_relevant a pointer to the last_left or last_right node
 * @return               a pointer to the merged interval node
 */
isset_node* merge_intervals(isset* s, isset_node* target, isset_node* last_relevant);

/**
 * Splays a node to the root of the tree
 *
 * @param  s      a pointer to a set, non-NULL
 * @param  target a pointer to a node to splay to root
 * @return        a pointer to the root / target in its new position
 */
isset_node* splay(isset* s, isset_node* target);

/**
 * Rotates a target node around a pivot in a given direction
 *
 * @param  s      a pointer to a set, non-NULL
 * @param  target a pointer to the target node to rotate around pivot
 * @param  pivot  a pointer to the pivot node
 * @param  dir    the direction to rotate around pivot: 'l' for left, 'r' for right
 * @return        a pointer to the target in its new position
 */
isset_node* rotate(isset* s, isset_node* target, isset_node* pivot, char dir);

/**
 * Prints the contents of the subtree rooted at the given node,
 * or "null" if the given pointer is NULL.
 *
 * @param node a pointer to a node
 * @param level the depth of that node
 */
void isset_print_subtree(const isset_node *node, int level, char dir);

/**
 * An alternative tree visualisation utility
 *
 * @param root a pointer to the root
 */
void print2D(isset_node *root);

/**
 * A helper function for print2D
 *
 * @param root  a pointer to the root
 * @param space padding for display
 */
void print2DUtil(isset_node *root, int space);

isset *isset_create()
{
  isset *s = malloc(sizeof(isset));

  if (s != NULL)
    {
      s->root = NULL;
      s->size = 0;
      s->nodes = 0;
    }

  return s;
}

int isset_size(const isset *s)
{
    if (s != NULL)
    {
        return s->size;
    }
    else
    {
        return 0;
    }
}

int isset_count_intervals(const isset *s)
{
    if (s != NULL)
    {
        return s->nodes;
    }
    else
    {
        return 0;
    }
}

bool isset_contains(const isset *s, int item)
{
    if (s == NULL)
    {
        return false;
    }
    else
    {
        // search for the node
        isset_search_result result;
        isset_find_node(s, item, &result);

        // splay the found node to root
        if(result.found)
        {
            splay((isset*) s, result.n);
        }

        // return true if found, false if not
        return result.found;
    }
}

void isset_find_node(const isset *s, int item, isset_search_result *result)
{
  isset_node *parent = NULL;
  isset_node *curr = s->root;
  result->last_left = NULL;
  result->last_right = NULL;
  int dir = 0;


    while (curr != NULL && (dir = isset_interval_compare(s, curr->start, curr->end, item)) != 0)
    {
        parent = curr;
        if (dir < 0)
        {
            result->last_left = curr;
            curr = curr->left;
        }
        else
        {
            result->last_right = curr;
            curr = curr->right;
        }
    }

    result->parent = parent;
    result->n = curr;
    result->dir = dir;
    result->found = (curr != NULL);
}

int isset_interval_compare(const isset *s, int start, int end, int item)
{
    int left = item - start;
    int right = item - end;

    if (left >= 0 && right <= 0)
    {
        return 0; // within the interval
    }
    else if (left < 0)
    {
        return left; // i.e an int; lesser than the interval end, lesser than 0
    }
    else
    {
        return right; // greater than the interval end, greater than 0
    }
}

bool isset_add(isset *s, int item)
{
    if(s == NULL)
    {
        return false;
    }

    // check if item is already in the set
    isset_search_result result;
    isset_find_node(s, item, &result);
    if(result.found)
    {
        // in the set already; nothing more to do
        return false;
    }

    // a pointer to keep track of modified or added node (used near end of code)
    isset_node* monitor = NULL;

    // check if new item can be added to beginning/end of the node
    // where search fell off the tree
    bool simple_add = false;
    if(result.dir < 0) // dropped left
    {
        // moved left; see if can prepend to parent
        if(result.parent->start == item + 1) // i.e. if adding 1 to the item would make it merge with start
        {
            result.parent->start = item; // update the interval
            monitor = result.parent;
            simple_add = true;
        }
    }
    else if(result.dir > 0) // dropped right
    {
        // moved right; see if can append to parent
        if (result.parent->end == item - 1)
        {
            result.parent->end = item;
            monitor = result.parent;
            simple_add = true;
        }
    }

    // if one of the above operations was not executed, create a new node
    if(!simple_add)
    {
        isset_node **incoming;
        if(result.parent == NULL) { incoming = &s->root; }
        else if(result.dir < 0) { incoming = &result.parent->left; }
        else { incoming = &result.parent->right; }

        isset_node *new_node = isset_create_node(item);
        *incoming = new_node; // link the parent's child ptr to the new node

        if(new_node == NULL)
        {
            return false;
        }

        new_node->parent = result.parent;
        s->nodes++;

        monitor = new_node;
    }

    // increment size as a result of simple_add or !simple_add
    s->size++;

    // at this point the item has been added, whether simply or by creating a new node
    // now, monitor points to either a modified existing node, or the new node
    // either way, we need to check whether the monitor node can be merged higher up the tree

    // "dropped" right
    if(result.dir > 0 )
    {
        if(result.last_left != NULL)
        {
            // check if monitor can be merged with last_left
            if(monitor->end + 1 == result.last_left->start)
            {
                monitor = merge_intervals(s, monitor, result.last_left);
            }
        }
    }
    // "dropped" left
    else if(result.dir < 0)
    {
        if(result.last_right != NULL)
        {
            if(result.last_right->end + 1 == monitor->start)
            {
                monitor = merge_intervals(s, monitor, result.last_right);
            }
        }
    }

    // splay the monitored node to the root
    splay(s, monitor);

    return true;
}

isset_node *isset_create_node(int item)
{
    isset_node *result = (isset_node *)malloc(sizeof(isset_node));

    if (result != NULL)
    {
        result->right = NULL;
        result->left = NULL;
        result->start = item;
        result->end = item;
    }

    return result;
}

bool isset_remove(isset *s, int item)
{
    // check if item is in the set
    isset_search_result result;
    isset_find_node(s, item, &result);
    if(!result.found)
    {
        return false;
    }

    // splay the found node to root
    splay(s, result.n);

    // case 1: removal of the entire root e.g. 19-19
    if(s->root->start == s->root->end && s->root->start == item)
    {
        // make a left tree and a right tree
        isset_node* root = s->root;
        isset_node* left_tree = s->root->left;
        isset_node* right_tree = s->root->right;

        // if no left and right trees
        if(left_tree == NULL && right_tree == NULL)
        {
            s->root = NULL;
            free(root);
        }
        // if no left tree, make right tree the root
        else if(left_tree == NULL)
        {
            s->root = right_tree;
            right_tree->parent = NULL;;
            free(root);
        }
        // if no right tree, make left tree the root
        else if(right_tree == NULL)
        {
            s->root = left_tree;
            left_tree->parent = NULL;
            free(root);
        }
        // if both trees exist, default to making the left tree the root
        else
        {
            s->root = left_tree;
            left_tree->parent = NULL;
            free(root);

            // find the largest node in the left tree
            isset_node* current = s->root;
            isset_node* largest = current;
            while(current != NULL)
            {
                largest = current;
                current = current->right;
            }
            // we break out of the loop with current = NULL and largest as the largest node

            // splay the largest element to root
            splay(s, largest);

            // now s->root has no right subtree, so we can connect it to the original right tree
            s->root->right = right_tree;
            right_tree->parent = s->root;
        }
        // in all the above subcases, an entire node was removed
        s->nodes--;
    }
    // case 2: simple decrement on edges
    else if(s->root->start == item)
    {
        s->root->start++;
    }
    else if(s->root->end == item)
    {
        s->root->end--;
    }
    // case 3: splitting of a node if item is between the two ends
    else if(item > s->root->start && item < s->root->end)
    {
        // compose new intervals
        int first_half_start = s->root->start;
        int first_half_end = item - 1;
        int second_half_start = item + 1;
        int second_half_end = s->root->end;

        // we can make optimisations so we only have to traverse trees when necessary
        isset_node* new_node = isset_create_node(0);

        // if the root has no children, default to keep the right half as the root
        if(s->root->left == NULL && s->root->right == NULL)
        {
            s->root->start = second_half_start;
            s->root->end = second_half_end;
            new_node->start = first_half_start;
            new_node->end = first_half_end;

            s->root->left = new_node;
            new_node->parent = s->root;
        }
        // if the root has no left child
        else if(s->root->left == NULL)
        {
            s->root->start = second_half_start;
            s->root->end = second_half_end;
            s->root->left = new_node;
            new_node->start = first_half_start;
            new_node->end = first_half_end;
            new_node->parent = s->root;
        }
        // if the root has no right child
        else if(s->root->right == NULL)
        {
            s->root->start = first_half_start;
            s->root->end = first_half_end;
            s->root->right = new_node;
            new_node->start = second_half_start;
            new_node->end = second_half_end;
            new_node->parent = s->root;
        }
        // if the root has both children, default to keep the right half as the root
        else
        {
            s->root->start = second_half_start;
            s->root->end = second_half_end;
            new_node->start = first_half_start;
            new_node->end = first_half_end;

            // traverse the root's left subtree to find the greatest node
            isset_node* current = s->root->left;
            isset_node* largest = current;
            while(current != NULL)
            {
                largest = current;
                current = current->right;
            }
            // we break out of the loop with current = NULL and largest as the largest node

            // the largest is guaranteed to have no right child
            largest->right = new_node;
            new_node->parent = largest;
        }
        s->nodes++;
    }
    s->size--;
    return true;
}

int isset_next_excluded(const isset *s, int item)
{
    isset_search_result result;
    isset_find_node(s, item, &result);

    // if no result was found, return the item itself
    if(!result.found)
    {
        return item;
    }

    // we have implemented interval merging behavior
    // all intervals are therefore distinct and disjoint
    return result.n->end + 1;
}

void isset_destroy(isset *s)
{
    isset_destroy_subtree(s->root);
    s->size = 0;
    s->nodes = 0;
    s->root = NULL;
    free(s);
}

void isset_destroy_subtree(isset_node *n)
{
    if (n != NULL)
    {
        isset_destroy_subtree(n->left);
        isset_destroy_subtree(n->right);
        free(n);
    }
}

isset_node* merge_intervals(isset* s, isset_node* target, isset_node* last_relevant)
{
    // find new interval formed by the target and the last_relevant nodes
    int new_start;
    int new_end;
    if(last_relevant->start < target->start) { new_start = last_relevant->start; }
    else { new_start = target->start; }
    if(last_relevant->end > target->end) { new_end = last_relevant->end; }
    else { new_end = target->end; }

    // merge the intervals
    last_relevant->start = new_start;
    last_relevant->end = new_end;

    // figure out if the target is a root, left or right child
    isset_node** incoming_target;
    if(target->parent == NULL) { incoming_target = &s->root; }
    else if(target->parent->left == target) { incoming_target = &target->parent->left; }
    else { incoming_target = &target->parent->right; }

    // if the target has no children
    if(target->left == NULL && target->right == NULL)
    {
        *incoming_target = NULL;
    }
    // if the target has no left child / a right child only
    else if(target->left == NULL)
    {
        *incoming_target = target->right;
        target->right->parent = target->parent;
    }
    // if the target has no right child / a left child only
    else if(target->right == NULL)
    {
        *incoming_target = target->left;
        target->left->parent = target->parent;
    }
    // if the target has both children
    else
    {
        printf("[isset/merge_intervals] undefined case\n");
    }

    // free the unneeded target
    free(target);

    // decrease the number of nodes because we just merged an interval
    s->nodes--;

    // return a pointer to the merged interval
    return last_relevant;
}

isset_node* splay(isset* s, isset_node* target)
{
    if(target->parent == NULL)
    {
        return target;
    }

    // is the target a left or right child?
    char target_is_what_child = 'e';
    if(target->parent->left == target) { target_is_what_child = 'l'; }
    else { target_is_what_child = 'r'; }

    // is the parent a root, or left or right child?
    char parent_is_what_child = 'e';
    if(target->parent->parent == NULL) { parent_is_what_child = 'x'; }
    else if(target->parent->parent->left == target->parent) { parent_is_what_child = 'l'; }
    else { parent_is_what_child = 'r'; }

    // zig situation 1
    if(parent_is_what_child == 'x' && target_is_what_child == 'l')
    {
        return splay(s, rotate(s, target, target->parent, 'r')); // rotate target around parent
    }
    // zig situation 2
    else if(parent_is_what_child == 'x' && target_is_what_child == 'r')
    {
        return splay(s, rotate(s, target, target->parent, 'l')); // rotate target around parent
    }
    // zig-zag situation 1
    else if(parent_is_what_child == 'l' && target_is_what_child == 'r')
    {
        return splay(s, rotate(s, target, target->parent, 'l')); // rotate target around parent
    }
    // zig-zag situation 2
    else if(parent_is_what_child == 'r' && target_is_what_child == 'l')
    {
        return splay(s, rotate(s, target, target->parent, 'r')); // rotate target around parent
    }
    // zig-zig situation 1
    else if(parent_is_what_child == 'l' && target_is_what_child == 'l')
    {
        rotate(s, target->parent, target->parent->parent, 'r'); // rotate parent around grandfather
        return splay(s, rotate(s, target, target->parent, 'r')); // rotate target around parent
    }
    // zig-zig situation 2
    else if(parent_is_what_child == 'r' && target_is_what_child == 'r')
    {
        rotate(s, target->parent, target->parent->parent, 'l'); // rotate parent around grandfather
        return splay(s, rotate(s, target, target->parent, 'l')); // rotate target around parent
    }
    else
    {
        printf("[isset/splay]: undefined case\n");
        return NULL;
    }
}

isset_node* rotate(isset* s, isset_node* target, isset_node* pivot, char dir)
{
    // figure out if the target is a root, left or right child
    isset_node** incoming_target;
    if(target->parent == NULL) { incoming_target = &s->root; }
    else if(target->parent->left == target) { incoming_target = &target->parent->left; }
    else { incoming_target = &target->parent->right; }

    // figure out if the pivot is a root, left or right child
    isset_node** incoming_pivot;
    if(pivot->parent == NULL) { incoming_pivot = &s->root; }
    else if(pivot->parent->left == pivot) { incoming_pivot = &pivot->parent->left; }
    else { incoming_pivot = &pivot->parent->right; }

    // do the rotation in the specified direction
    // e.g. if dir == r, then rotate target around pivot,
    // such that that the target is the new parent
    if(dir == 'r')
    {
        // target is now the new child of the pivot's parent
        *incoming_pivot = target;
        target->parent = pivot->parent;

        // one of target's children is now the pivot's child
        *incoming_target = target->right;
        if(target->right != NULL) { target->right->parent = pivot; }

        // target is now the parent of the pivot
        target->right = pivot;
        pivot->parent = target;
    }
    else if(dir == 'l')
    {
        // target is now the new child of the pivot's parent
        *incoming_pivot = target;
        target->parent = pivot->parent;

        // one of target's children is now the pivot's child
        *incoming_target = target->left;
        if(target->left != NULL) { target->left->parent = pivot; }

        // target is now the parent of the pivot
        target->left = pivot;
        pivot->parent = target;
    }
    else
    {
        printf("[isset/rotate]: invalid direction for rotate\n");
    }

    return target;
}

void isset_print_subtree(const isset_node *node, int level, char dir)
{
  for (int i = 0; i < level; i++)
    {
      printf(" ");
    }
  if (node != NULL)
    {
        if(dir == 'l')
        {
            printf("l: ");
        }
        else if(dir == 'r')
        {
            printf("r: ");
        }
      printf("[%d-%d]\n", node->start, node->end);
      isset_print_subtree(node->left, level + 1, 'l');
      isset_print_subtree(node->right, level + 1, 'r');
    }
  else
    {
        if(dir == 'l')
        {
            printf("l: ");
        }
        else if(dir == 'r')
        {
            printf("r: ");
        }
      printf("null\n");
    }
}

void print2D(isset_node *root)
{
   // Pass initial space count as 0
   print2DUtil(root, 0);
}

void print2DUtil(isset_node *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%d-%d\n", root->start, root->end);

    // Process left child
    print2DUtil(root->left, space);
}
