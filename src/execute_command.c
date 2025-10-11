/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 02:36:37 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/12 02:44:01 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	check_access(t_group *group, t_ast_node *node);
static char *get_full_cmd_path(t_group *group, char *cmd);

int	execute_command(t_group *group, t_ast_node *node, bool is_pipeline)
{
	int					status;
	pid_t				pid;
	struct s_command	*cmd;

	cmd = &(node -> data.exec);
	check_access(group, node);
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

static void	check_access(t_group *group, t_ast_node *node)
{
	int	access_ret;

	if (ft_strchr(node -> data.exec.commands, '/') == NULL)
		printf("Shorten commands!!!\n");
		// data -> command_path = get_full_command_path(data, data -> cmds_arg[0]);
	access_ret = access(node -> data.exec.commands, F_OK | X_OK);
	if (access_ret == -1)
		clear_and_exit(group, node, "access");
}

static char *get_full_cmd_path(t_group *group, char *cmd)
{
	return (NULL);
}