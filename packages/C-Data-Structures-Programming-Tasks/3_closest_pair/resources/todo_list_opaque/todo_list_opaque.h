#ifndef __TODO_LIST_H__
#define __TODO_LIST_H__

#include "task.h"

struct todo_list;
typedef struct todo_list todo_list;

todo_list *todo_list_create();

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
