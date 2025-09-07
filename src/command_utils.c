#include "execute.h"


pid_t ft_fork(t_group *group)
{
  pid_t pid;

  pid = fork();
  if (pid == -1)
   clear_and_exit(group, "fork");
  return pid;
}

