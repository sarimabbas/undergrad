#include <stdbool.h>

#include "todo_list.h"
#include "task.h"

void todo_list_init(todo_list *list)
{
  list->completed = 0;
  list->size = 0;
}

void todo_list_add(todo_list *list, const task *t)
{
  if (list->size < TODO_LIST_MAX_SIZE)
    {
      list->todo[list->size] = *t;
      list->size++;
    }
}

void todo_list_current(const todo_list *list, task *t)
{
  *t = list->todo[list->completed];
}

bool todo_list_incomplete(const todo_list *list)
{
  return list->size > list->completed;
}

void todo_list_complete_current(todo_list *list)
{
  list->todo[list->completed].completed = true;
  list->completed++;
}


void todo_list_delay_current(todo_list *list)
{
  if (list->size - list->completed >= 2)
    {
      task temp = list->todo[list->completed];
      list->todo[list->completed] = list->todo[list->completed + 1];
      list->todo[list->completed + 1] = temp;
    }
}

void todo_list_postpone_current(todo_list *list)
{
  task temp = list->todo[list->completed];
  for (int i = list->completed; i < list->size - 1; i++)
    {
      list->todo[i] = list->todo[i + 1];
    }
  list->todo[list->size - 1] = temp;
}

int todo_list_estimated_time(const todo_list *list)
{
  int tot = 0;
  for (int i = list->completed; i < list->size; i++)
    {
      tot += list->todo[i].time;
    }
  return tot;
}

int todo_list_incomplete_count(const todo_list *list)
{
  return list->size - list->completed;
}
