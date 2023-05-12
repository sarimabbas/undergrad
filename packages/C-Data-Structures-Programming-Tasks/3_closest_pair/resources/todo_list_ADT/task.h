#ifndef __TASK_H__
#define __TASK_H__

#include <stdbool.h>

#define TASK_MAX_TEXT 40

typedef struct task {
  char description[TASK_MAX_TEXT + 1];
  int time;
  bool completed;
} task;

/**
 * Reads a task from standard input.  The format should be an integer
 * followed by whitespace followed by the task description, which is
 * terminated by an end-of-line or end-of-file.  For example
 *
 * 30 rewrite this code so it doesn't pass tasks by value all over the place
 * 10 fix this so it is more robust
 *
 * The terminator is not stored in the task.
 *
 * @return the task read
 */
bool task_read(task *t);


/**
 * Returns the time from the given task.
 */
int task_time(const task *t);


/**
 * Prints the given task to standard output.
 *
 * @param t a task
 */
void task_print(const task *t);

#endif
