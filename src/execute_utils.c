/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 07:11:23 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/05 01:31:53 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	free_command_node(t_ast_node *node);
static void	free_redirect_node(t_list *redir_lst);

void	clear_and_exit(t_group *group, t_ast_node *node, char *cmd)
{
	clear_ast(group -> ast_root);
	free(group);
	if (ft_strncmp(cmd, "access", -1) == 0)
	{
		exit(EXEC_CMD_NOT_FOUND);
	}
	if (errno == ENOENT)
		perror(cmd);
	else if (errno == EACCES)
		perror(cmd);
	else if (errno == ECHILD)
		perror(cmd);
	else if (errno == EINVAL)
		perror(cmd);
	else if (errno == EAGAIN)
		perror(cmd);
	printf("[Exit %i]\n", errno);
	exit(errno);
}

void	*clear_ast(t_ast_node *node)
{
	if (node -> type == NODE_COMMAND)
		free_command_node(node);
	else if (node -> type == NODE_PIPELINE)
	{
		clear_ast(node -> data.tree.left);
		clear_ast(node -> data.tree.right);
		free(node);
	}
	return (NULL);
}

static void	free_command_node(t_ast_node *node)
{
	char	**trav;
	t_list	*redir_trav1;
	t_list	*redir_trav2;

	if (node -> data.exec.commands)
	{
		free(node -> data.exec.commands);
		node -> data.exec.commands = NULL;
	}
	if (node -> data.exec.arguments)
	{
		trav = node -> data.exec.arguments;
		while (*trav != NULL)
		{
			free(*trav);
			*trav = NULL;
			trav++;
		}
		free(node -> data.exec.arguments);
	}
	free_redirect_node(node -> data.exec.redir);
	free(node);
	return ;
}

static void	free_redirect_node(t_list *redir_lst)
{
	t_redir		*redir;
	t_list		*trav;
	struct stat	st;

	while (redir_lst != NULL)
	{
		redir = (t_redir *)(redir_lst -> content);
		if (redir -> type == HEREDOC)
		{
			if (redir -> fd != -1)
			{
				if (fstat(redir -> fd, &st) != -1)
					close(redir -> fd);
			}
			if (redir -> filename != NULL)
				unlink(redir -> filename);
			if (redir -> delimeter != NULL)
				free(redir -> delimeter);
		}
		free(redir -> filename);
		free(redir);
		trav = redir_lst;
		redir_lst = redir_lst -> next;
		free(trav);
	}
}
