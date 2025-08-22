/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:50:19 by thacharo          #+#    #+#             */
/*   Updated: 2025/08/22 16:48:21 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "execute.h"


static char **get_dummy_args(char **args);
static t_ast_node	*create_ast_node(char *cmd, char **args, char **envp);
static t_ast_node	*create_pipeline_node(t_ast_node *left, t_ast_node *right);

t_ast_node *create_dummy_ast(char **envp)
{
	t_ast_node *node_1 = create_ast_node("/bin/cat", (char *[]){"cat", "-e", "dockerfile", NULL}, envp);
	t_ast_node *node_2 = create_ast_node("/bin/cat", (char *[]){"cat", "-e", NULL}, envp);
	t_ast_node *pipe_node = create_pipeline_node(node_1, node_2);

	return (pipe_node);
}

static t_ast_node	*create_ast_node(char *cmd, char **args, char **envp)
{
	t_ast_node *node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node -> type = NODE_COMMAND;
	node -> root_pipe = NULL;
	// node -> data.exec.commands = (char *)malloc(sizeof(char) * strlen(cmd));
	// if (!node -> data.exec.commands)
	// 	return (clear_ast(node));
	node -> data.exec.commands = strdup(cmd);
	node -> data.exec.arguments = get_dummy_args(args);
	node -> data.exec.envp = envp;
	return (node);
}

static char **get_dummy_args(char **args)
{
	int		i;
	int		len;
	char	**lst;

	i = 0;
	len = 0;
	while (args[len] != NULL)
		len++;
	lst = (char **)malloc(sizeof(char *) * (len + 1));
	if (!lst)
		return (NULL);
	while (i < len)	
	{
		lst[i] = strdup(args[i]);
		i++;
	}
	lst[i] = NULL;
	return (lst);
}


static t_ast_node	*create_pipeline_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node *node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node -> type = NODE_PIPELINE;
	node -> root_pipe = NULL;
	node -> data.tree.left = left;
	node -> data.tree.right = right;
	return (node);
}