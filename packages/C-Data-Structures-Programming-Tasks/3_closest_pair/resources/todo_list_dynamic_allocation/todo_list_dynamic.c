#include <stdbool.h>
#include <stdlib.h>

#include "todo_list_dynamic.h"
#include "task.h"

void todo_list_embiggen(todo_list *list);

void todo_list_init(todo_list *list)
{
  list->completed = 0;
  list->size = 0;
  list->todo = malloc(sizeof(task) * TODO_LIST_INITIAL_SIZE);
  list->capacity = (list->todo != NULL ? TODO_LIST_INITIAL_SIZE : 0);
}

void todo_list_destroy(todo_list *list)
{
  free(list->todo);
}

void todo_list_add(todo_list *list, const task *t)
{
  if (list->size == list->capacity)
    {
      todo_list_embiggen(list);
    }
  
  if (list->size < list->capacity)
    {
      list->todo[list->size] = *t;
      list->size++;
    }
}

void todo_list_embiggen(todo_list *list)
{
  int largerCap = (list->capacity * 2 > TODO_LIST_INITIAL_SIZE ? list->capacity * 2 : TODO_LIST_INITIAL_SIZE);
  task *larger = realloc(list->todo, largerCap * sizeof(task));

  if (larger != NULL)
    {
      list->todo = larger;
      list->capacity = largerCap;
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
