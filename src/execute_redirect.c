/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 01:01:16 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/05 01:03:05 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	redirect_input(t_group *group, t_redir *redirect, int flags);
static void	redirect_output(t_group *group, t_redir *redirect, int flags);

void	redirection(t_group *group, t_ast_node *node)
{
	t_list	*lst;
	t_redir	*redirect;

	lst = node -> data.exec.redir;
	while (lst != NULL)
	{
		redirect = (t_redir *)(lst -> content);
		if (redirect -> type == REDIR_INPUT || redirect -> type == HEREDOC)
			redirect_input(group, redirect, O_RDONLY);
		else if (redirect -> type == REDIR_OUTPUT)
			redirect_output(group, redirect, O_CREAT | O_WRONLY | O_TRUNC);
		else if (redirect -> type == APPEND)
			redirect_output(group, redirect, O_CREAT | O_WRONLY | O_APPEND);
		lst = lst -> next;
	}
}

static void	redirect_input(t_group *group, t_redir *redirect, int flags)
{
	int		infile_fd;
	char	*filename;

	filename = redirect -> filename;
	infile_fd = open(filename, flags, FILE_PERMISSION);
	if (infile_fd == -1)
		clear_and_exit(group, NULL, "open");
	close(STDIN_FILENO);
	dup(infile_fd);
	close(infile_fd);
	return ;
}

static void	redirect_output(t_group *group, t_redir *redirect, int flags)
{
	int		outfile_fd;
	char	*filename;

	filename = redirect -> filename;
	outfile_fd = open(filename, flags, FILE_PERMISSION);
	if (outfile_fd == -1)
		clear_and_exit(group, NULL, "open");
	close(STDOUT_FILENO);
	dup(outfile_fd);
	close(outfile_fd);
	return ;
}
