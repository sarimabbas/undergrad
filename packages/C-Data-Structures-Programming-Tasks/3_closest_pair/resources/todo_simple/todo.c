#include <stdio.h>
#include <string.h>

#include "string_util.h"
#include "task.h"

// see note in task.h about passing task structs by value

int main()
{
  // prompt for and read number of tasks
  int max_tasks;
  printf("Enter number of tasks:");
  scanf("%d", &max_tasks); // should add some error checking here

  int num_tasks; // the number of tasks read
    
  task todo[max_tasks]; // the to-do list

  // read individual tasks
  // we will write this loop more readably once we rewrite task_read
  for (num_tasks = 0;
       num_tasks < max_tasks && (todo[num_tasks] = task_read(),
				 strlen(todo[num_tasks].description) > 0);
       num_tasks++)
    {
    }

  // loop until no more input or all tasks complete
  int completed = 0;
  while (completed < num_tasks && !feof(stdin) && !ferror(stdin))
    {
      // print next task
      task_print(todo[completed]);
      printf("\n");

      // read user input (complete/delay/postpone)
      char command[2];
      command[0] = 'q';
      read_line(command, 1);

      // update list according to user input
      if (command[0] == 'c')
	{
	  todo[completed].completed = true;
	  completed++;
	}
      else if (command[0] == 'd')
	{
	  if (num_tasks - completed >= 2)
	    {
	      // swap current task and following one
	      task temp = todo[completed];
	      todo[completed] = todo[completed + 1];
	      todo[completed + 1] = temp;
	    }
	}
      else if (command[0] == 'p')
	{
	  // shift all following tasks back and put current one at end
	  task temp = todo[completed];
	  for (int i = completed; i < num_tasks - 1; i++)
	    {
	      todo[i] = todo[i + 1];
	    }
	  todo[num_tasks - 1] = temp;
	}
    }

  // determine if all tasks were complete
  if (completed == num_tasks)
    {
      // yes, all complete: good job, you can go on to the next stage now!
      printf("Congratulations!\n");
    }
  else
    {
      // no, not all complete: add up remaining time
      int tot = 0;
      for (int i = completed; i < num_tasks; i++)
	{
	  tot += task_time(todo[i]);
	}
      printf("%d tasks, estimated %d minutes remain\n",
	     num_tasks - completed,
	     tot);
    }
}
