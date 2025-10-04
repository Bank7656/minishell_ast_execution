/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:35:04 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/05 01:31:01 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	execute_command(t_group *group, t_ast_node *node, bool is_pipeline);

void	execution(t_group *group, t_ast_node *node)
{
	int	exit_code;
	int	temp_file_id;

	temp_file_id = 1;
	signal_init(group);
	prepare_heredoc(group, node, &temp_file_id);
	exit_code = execute_ast(group, node, false);
	return ;
}

int	execute_ast(t_group *group, t_ast_node *node, bool is_pipeline)
{
	int	exit_code;

	exit_code = -1;
	if (node -> type == NODE_COMMAND)
		exit_code = execute_command(group, node, is_pipeline);
	else if (node -> type == NODE_PIPELINE)
		exit_code = execute_pipeline(group, node);
	return (exit_code);
}

int	execute_command(t_group *group, t_ast_node *node, bool is_pipeline)
{
	int					status;
	pid_t				pid;
	struct s_command	*cmd;

	cmd = &(node -> data.exec);
	//check_access(group, node);
	if (!is_pipeline)
	{
		pid = ft_fork(group);
		if (pid == 0)
		{
			redirection(group, node);
			ft_execve(group, cmd);
		}
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	redirection(group, node);
	ft_execve(group, cmd);
	clear_and_exit(group, node, "execve");
	return (EXIT_FAILURE);
}

void	check_access(t_group *group, t_ast_node *node)
{
	int	access_ret;

	if (access(node -> data.exec.commands, F_OK | X_OK) == -1)
		clear_and_exit(group, node, "access");
}
