#include "execute.h"

void  redirect_input(t_group *group, t_ast_node *node) 
{
  int     infile_fd;
  t_list *redir_lst;
  t_redir *redirect;
  char  *filename;

  redir_lst = node -> data.exec.redir_input;
  while (redir_lst != NULL)
  {
    redirect = (t_redir *)(redir_lst -> content);
    filename = redirect -> filename;
    infile_fd = open(filename, redirect -> mode, 0644);
    if (infile_fd == -1)
      clear_and_exit(group, "open");
    close(STDIN_FILENO);
    dup(infile_fd);
    close(infile_fd);
    redir_lst = redir_lst -> next;
  }
  return ;
}

void  redirect_output(t_group *group, t_ast_node *node) 
{
  int     outfile_fd;
  t_list  *redir_lst;
  t_redir *redirect;
  char    *filename;

  redir_lst = node -> data.exec.redir_output;
  while (redir_lst != NULL)
  {
    redirect = (t_redir *)(redir_lst -> content);
    filename = redirect -> filename;
    outfile_fd = open(filename, redirect -> mode, 0644);
    if (outfile_fd == -1)
      clear_and_exit(group, "open");
    close(STDOUT_FILENO);
    dup(outfile_fd);
    close(outfile_fd);
    redir_lst = redir_lst -> next;
  }
  return ;
}
