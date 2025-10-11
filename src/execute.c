/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:35:04 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/12 02:36:47 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	execute_command(t_group *group, t_ast_node *node, bool is_pipeline);
static void	check_access(t_group *group, t_ast_node *node);

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


