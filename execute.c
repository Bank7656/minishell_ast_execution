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

int  execute_ast(t_ast_node *node);
void execute_command(t_ast_node *node);
int  execute_pipeline(t_ast_node *node);

void	execution(t_ast_node *node)
{
  int   exit_code;
  pid_t pid;

  pid = ft_fork(node);
  if (pid == 0)
  {
    exit_code = execute_ast(node);
    printf("[Final Exit code %i]\n", exit_code);
    clear_and_exit(node, "NONE");
  }
  wait(NULL);

  return ;
}

int	execute_ast(t_ast_node *node)
{
  int exit_code;
  int is_right_most_child;

  exit_code = -1;
	if (node -> type == NODE_COMMAND)
		execute_command(node);
	else if (node -> type == NODE_PIPELINE)
		exit_code = execute_pipeline(node);
  return (exit_code);
}

void  execute_command(t_ast_node *node)
{
  pid_t pid;
  char  *path;
  char  **argv;
  char  **envp;

  path = node -> data.exec.commands;
  argv = node -> data.exec.arguments;
  envp = node -> data.exec.envp;
  execve(path, argv, envp);
  clear_and_exit(node, "execve");
}

