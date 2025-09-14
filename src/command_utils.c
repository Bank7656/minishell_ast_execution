#include "execute.h"


pid_t ft_fork(t_group *group)
{
  pid_t pid;

  pid = fork();
  if (pid == -1)
   clear_and_exit(group, "fork");
  return pid;
}

void ft_pipe(t_group *group, int pipe_fd[2], int prev_fd)
{

  if (pipe(pipe_fd) == -1)
  {
    if (prev_fd != -1)
      close(prev_fd);
    clear_and_exit(group, "pipe");
  }

}

