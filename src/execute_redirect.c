#include "execute.h"

void  redirect_input(t_group *group, t_ast_node *node) 
{
  int     infile_fd;
  t_list *redir_node;
  char  *filename;

  redir_node = node -> data.exec.redir_input;

  while (redir_node != NULL)
  {
    filename = ((t_redir *)(redir_node -> content)) -> filename;
    infile_fd = open(filename, O_RDONLY, 0644);
    if (infile_fd == -1)
      clear_and_exit(group, "open");
    close(STDIN_FILENO);
    dup(infile_fd);
    close(infile_fd);
    redir_node = redir_node -> next;
  }
  return ;
}
