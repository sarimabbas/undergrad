#ifndef __TODO_LIST_H__
#define __TODO_LIST_H__

#include "task.h"

#define TODO_LIST_INITIAL_SIZE 2

typedef struct
{
  int size;
  int capacity;
  int completed;
  task *todo;
} todo_list;

void todo_list_init(todo_list *list);

void todo_list_destroy(todo_list *list);

void todo_list_add(todo_list *list, const task *t);

void todo_list_current(const todo_list *list, task *t);

bool todo_list_incomplete(const todo_list *list);

void todo_list_complete_current(todo_list *list);

void todo_list_delay_current(todo_list *list);

void todo_list_postpone_current(todo_list *list);

int todo_list_estimated_time(const todo_list *list);

int todo_list_incomplete_count(const todo_list *list);

#endif
