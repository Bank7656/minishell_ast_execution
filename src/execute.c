/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:35:04 by thacharo          #+#    #+#             */
/*   Updated: 2025/08/25 13:51:12 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int  execute_ase(t_group *group, t_ast_node *node, bool is_pipeline);
int  execute_command(t_group *group, t_ast_node *node, bool is_pipeline);



void  prepare_heredoc(t_group *group, t_ast_node *node);
void  check_heredoc(t_group *group, t_ast_node *node);
void  create_tmp_file(t_group *group, t_redir *redir);
char *get_tmpfile_name(t_group *group, int n);  

void		execution(t_group *group, t_ast_node *node)
{
  int   exit_code;

  prepare_heredoc(group, node);
  exit_code = execute_ast(group, node, false);
  printf("[Final Exit code %i]\n", exit_code);

  return ;
}


void  prepare_heredoc(t_group *group, t_ast_node *node)
{
  if (node -> type == NODE_COMMAND)
    check_heredoc(group, node);
  else if (node -> type == NODE_PIPELINE)
  {
    prepare_heredoc(group, node -> data.tree.left);
    prepare_heredoc(group, node -> data.tree.right);
  }
  return ;
}

void  check_heredoc(t_group *group, t_ast_node *node)
{
  t_list *lst;
  t_redir *trav;
  int   run_id;
  char  *filename;

  lst = node -> data.exec.redir_input;
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
      printf("[%s]\n", trav -> filename);
      run_id++;
    }
    lst = lst -> next;
  }
}

void  create_tmp_file(t_group *group, t_redir *redir)
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

char *get_tmpfile_name(t_group *group, int n)  
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


int	execute_ast(t_group *group, t_ast_node *node, bool is_pipeline)
{
  int exit_code;

  exit_code = -1;
	if (node -> type == NODE_COMMAND)
		exit_code = execute_command(group, node, is_pipeline);
	else if (node -> type == NODE_PIPELINE)
		exit_code = execute_pipeline(group, node);
  return (exit_code);
}

int  execute_command(t_group *group, t_ast_node *node, bool is_pipeline)
{
  int   status;
  pid_t pid;
  struct s_command  *cmd;

  cmd = &(node -> data.exec);
  redirect_input(group, node);
  redirect_output(group, node);
  if (!is_pipeline)
  {
    pid = ft_fork(group);
    if (pid == 0)
    {
      execve(cmd -> commands, cmd -> arguments, cmd -> envp);
    }
    waitpid(pid, &status, 0);
    return (WEXITSTATUS(status));
  }
  execve(cmd -> commands, cmd -> arguments, cmd -> envp);
  clear_and_exit(group, "execve");
  return (EXIT_FAILURE);
}

