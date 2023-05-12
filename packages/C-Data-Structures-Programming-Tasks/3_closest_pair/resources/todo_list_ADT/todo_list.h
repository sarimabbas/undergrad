#ifndef __TODO_LIST_H__
#define __TODO_LIST_H__

#include "task.h"

#define TODO_LIST_MAX_SIZE 10

typedef struct
{
  int size;
  int completed;
  task todo[TODO_LIST_MAX_SIZE];
} todo_list;

/**
 * Initializes the given list.
 *
 * @param list an uninitialized list, non-NULL
 */
void todo_list_init(todo_list *list);

/**
 * Adds the given task to the end of the given list.
 *
 * @param list a list, non-NULL
 * @param t a task, non-NULL
 */
void todo_list_add(todo_list *list, const task *t);

/**
 * Copies the current task into the given location.
 *
 * @param list an uncompleted list, non-NULL
 * @param t a task, non-NULL
 */
void todo_list_current(const todo_list *list, task *t);

/**
 * Determines if the given list is incomplete.
 *
 * @param list a list, non-NULL
 * @return true if and only if there are uncompleted tasks on the list
 */
bool todo_list_incomplete(const todo_list *list);

/**
 * Marks the current task as complete and makes the next task,
 * if there is one, current.
 *
 * @param list an uncompleted list, non-NULL
 */
void todo_list_complete_current(todo_list *list);

/**
 * Swaps the next two tasks on the given list.  There is no effect if
 * there are fewer than two uncompleted tasks on the list.
 *
 * @param list a list, non-NULL
 */
void todo_list_delay_current(todo_list *list);

/**
 * Moves the current task on the given list to the end of that list.
 * There is no effect if there are fewer than two uncompleted tasks
 * on the list.
 *
 * @param list a list, non-NULL
 */
void todo_list_postpone_current(todo_list *list);

/**
 * Returns the estimated time to complete all the uncompleted tasks
 * on the given list.
 *
 * @param list a list, non-NULL
 * @return the total estimated time of the uncompleted tasks on that list
 */
int todo_list_estimated_time(const todo_list *list);

/**
 * Returns the number of uncompleted tasks on the given list.
 *
 * @param list a list, non-NULL
 * @return the number of uncompleted tasks on that list
 */
int todo_list_incomplete_count(const todo_list *list);

#endif
