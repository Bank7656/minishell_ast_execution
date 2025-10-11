/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_and_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 07:11:23 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/12 03:18:48 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	access_error_prompt(char *command);
static void	display_error_prompt(t_ast_node *node, char *cmd);

void	clear_and_exit(t_group *group, t_ast_node *node, char *cmd)
{
	display_error_prompt(node, cmd);	
	clear_env_list(group -> env_list);
	clear_ast(group -> ast_root);
	free(group);
	printf("[Exit %i]\n", errno);
	exit(errno);
}

static void	display_error_prompt(t_ast_node *node, char *cmd)
{
	if (ft_strncmp(cmd, "access", -1) == 0)
		access_error_prompt(node -> data.exec.commands);
	else if (errno == ENOENT)
		perror(cmd);
	else if (errno == EACCES)
		perror(cmd);
	else if (errno == ECHILD)
		perror(cmd);
	else if (errno == EINVAL)
		perror(cmd);
	else if (errno == EAGAIN)
		perror(cmd);
}

static void	access_error_prompt(char *command)
{
	if (errno == EACCES)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
}

