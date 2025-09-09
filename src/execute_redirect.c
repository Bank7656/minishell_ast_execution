#include "execute.h"

void  redirect_input(t_group *group, t_ast_node *node) 
{
  int     infile_fd;
  t_redir *redir_node;

  redir_node = node -> data.redir_input;
  while (redir_node != NULL)
  {
    infile_fd = open(redir_node -> filename, O_RDONLY, 0644);
    if (infile_fd == -1)
      clear_and_exit(group, "open");
    dup2(infile_fd, STDIN_FILENO);
    redir_node = redir_node -> next;
  }
  return ;
}
