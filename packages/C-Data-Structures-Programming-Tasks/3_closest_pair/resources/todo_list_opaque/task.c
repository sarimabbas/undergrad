#include <stdio.h>

#include "string_util.h"
#include "task.h"

bool task_read(task *t)
{
  if (scanf("%d ", &t->time) > 0)
    {
      read_line(t->description, TASK_MAX_TEXT);
      return !ferror(stdin);
    }
  else
    {
      t->description[0] = '\0';
      return false;
    }
}

int task_time(const task *t)
{
  return t->time;
}

void task_print(const task *t)
{
  printf("%d %s", t->time, t->description);
}
