/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 01:03:24 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/05 01:05:02 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

pid_t	ft_fork(t_group *group)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		clear_and_exit(group, NULL, "fork");
	return (pid);
}

void	ft_pipe(t_group *group, int pipe_fd[2], int prev_fd)
{
	if (pipe(pipe_fd) == -1)
	{
		if (prev_fd != -1)
			close(prev_fd);
		clear_and_exit(group, NULL, "pipe");
	}
}

void	ft_execve(t_group *group, struct s_command *cmd)
{
	if (execve(cmd -> commands, cmd -> arguments, cmd -> envp) == -1)
		clear_and_exit(group, NULL, "execve");
}
