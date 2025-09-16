#include "execute.h"

static void  check_heredoc(t_group *group, t_list *lst);
static void  create_tmp_file(t_group *group, t_redir *redir);
static char *get_tmpfile_name(t_group *group, int n);

void  prepare_heredoc(t_group *group, t_ast_node *node)
{
  if (node -> type == NODE_COMMAND)
    check_heredoc(group, node -> data.exec.redir);
  else if (node -> type == NODE_PIPELINE)
  {
    prepare_heredoc(group, node -> data.tree.left);
    prepare_heredoc(group, node -> data.tree.right);
  }
  return ;
}

static void  check_heredoc(t_group *group, t_list *lst)
{
  t_redir *trav;
  int   run_id;
  char  *filename;

  if (lst == NULL)
    return ;
  run_id = 1;
  while (lst != NULL)
  {
    trav = (t_redir *)(lst -> content);
    if (trav -> type == HEREDOC)
    {
      filename = get_tmpfile_name(group, run_id);
      trav -> filename = filename;
      create_tmp_file(group, trav);
      run_id++;
    }
    lst = lst -> next;
  }
}

static void  create_tmp_file(t_group *group, t_redir *redir)
{
  int temp_fd;
  char  *line;

  temp_fd = open(redir -> filename, O_CREAT | O_WRONLY, 0644);
  if (temp_fd == -1)
    clear_and_exit(group, "open");
  while (true)
  {
    ft_putstr_fd("> ", STDOUT_FILENO);
    line = get_next_line(STDIN_FILENO);
    if (line == NULL)
      clear_and_exit(group, "malloc");
		if (ft_strncmp(line, redir -> delimeter, ft_strlen(line) - 1) == 0)
		{
			free(line);
			break ;
		}
    ft_putstr_fd(line, temp_fd);
    free(line);
  }
  close(temp_fd);
}

static char *get_tmpfile_name(t_group *group, int n)  
{
  char  *filename;
  char  *run_number;
  
  run_number = ft_itoa(n);
  if (run_number == NULL)
    clear_and_exit(group, "malloc");
  filename = ft_strjoin(TMP_FILE, run_number);    
  free(run_number);
  if (filename == NULL)
    clear_and_exit(group, "malloc");
  return (filename);  
}
