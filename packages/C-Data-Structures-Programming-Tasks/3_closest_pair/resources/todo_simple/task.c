#include <stdio.h>

#include "string_util.h"
#include "task.h"

task task_read()
{
  task t;
  if (scanf("%d ", &t.time) > 0)
    {
      read_line(t.description, TASK_MAX_TEXT);
    }
  else
    {
      t.description[0] = '\0';
    }
  return t;
}

int task_time(task t)
{
  return t.time;
}

void task_print(task t)
{
  printf("%d %s", t.time, t.description);
}
