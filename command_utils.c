#include "execute.h"


pid_t ft_fork(t_group *group, t_ast_node *node)
{
  pid_t pid;

  pid = fork();
  if (pid == -1)
   clear_and_exit(group -> ast_root, "fork");
  return pid;
}

