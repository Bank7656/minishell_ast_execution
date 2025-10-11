/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 01:03:24 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/12 03:15:47 by thacharo         ###   ########.fr       */
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
	char	**envp;

	envp = env_lst_to_arr(group, group -> env_list);
	if (execve(cmd -> commands, cmd -> arguments, envp) == -1)
		clear_and_exit(group, NULL, "execve");
}





