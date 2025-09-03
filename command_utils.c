#include "execute.h"


pid_t ft_fork(t_ast_node *node)
{
  pid_t pid;

  pid = fork();
  if (pid == -1)
   clear_and_exit(node, "fork");
  return pid;
}

