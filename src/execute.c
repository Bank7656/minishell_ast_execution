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

void		execution(t_group *group, t_ast_node *node)
{
  int   exit_code;

  prepare_heredoc(group, node);
  exit_code = execute_ast(group, node, false);
  printf("[Final Exit code %i]\n", exit_code);

  return ;
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
  redirection(group, node);
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

