#include <stdio.h>
#include <string.h>

#include "string_util.h"
#include "task.h"
#include "todo_list_opaque.h"

int main(int argc, char **argv)
{
  int max_tasks;
  printf("Enter number of tasks:");
  scanf("%d", &max_tasks);

  todo_list *list = todo_list_create();
  if (list == NULL)
    {
      printf("%s: could not allocate space for list\n", argv[0]);
    }

  int num_tasks;
  task t;
  // we will write this loop more readably once we rewrite task_read
  for (num_tasks = 0;
       num_tasks < max_tasks && task_read(&t);
       num_tasks++)
    {
      todo_list_add(list, &t);
    }

  while (todo_list_incomplete(list) && !feof(stdin) && !ferror(stdin))
    {
      task current; // #HELP : why is there not a task pointer?
      todo_list_current(list, &current);
      task_print(&current);
      printf("\n");

      char command[2];
      command[0] = 'q';
      read_line(command, 1);

      if (command[0] == 'c')
	{
	  todo_list_complete_current(list);
	}
      else if (command[0] == 'd')
	{
	  todo_list_delay_current(list);
	}
      else if (command[0] == 'p')
	{
	  todo_list_postpone_current(list);
	}
    }

  if (!todo_list_incomplete(list) == num_tasks) // interesting error
    {
      printf("Congratulations!\n");
    }
  else
    {
      printf("%d tasks, estimated %d minutes remain\n",
	     todo_list_incomplete_count(list),
	     todo_list_estimated_time(list));
    }

    todo_list_destroy(list);
}
