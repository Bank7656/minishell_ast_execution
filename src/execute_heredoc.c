/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 01:07:10 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/05 01:11:32 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	check_heredoc(t_group *group, t_list *lst, int *run_id);
static void	create_tmp_file(t_group *group, t_redir *redir);
static char	*get_tmpfile_name(t_group *group, int n);

void	prepare_heredoc(t_group *group, t_ast_node *node, int *run_id)
{
	if (node -> type == NODE_COMMAND)
		check_heredoc(group, node -> data.exec.redir, run_id);
	else if (node -> type == NODE_PIPELINE)
	{
		prepare_heredoc(group, node -> data.tree.left, run_id);
		prepare_heredoc(group, node -> data.tree.right, run_id);
	}
	return ;
}

static void	check_heredoc(t_group *group, t_list *lst, int *run_id)
{
	char	*filename;
	t_redir	*trav;

	if (lst == NULL)
		return ;
	while (lst != NULL)
	{
		trav = (t_redir *)(lst -> content);
		if (trav -> type == HEREDOC)
		{
			filename = get_tmpfile_name(group, *run_id);
			trav -> filename = filename;
			create_tmp_file(group, trav);
			(*run_id)++;
		}
		lst = lst -> next;
	}
}

static void	create_tmp_file(t_group *group, t_redir *redir)
{
	int		length;
	char	*line;

	redir -> fd = open(redir -> filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (redir -> fd == -1)
		clear_and_exit(group, NULL, "open");
	while (true)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			clear_and_exit(group, NULL, "malloc");
		length = ft_strlen(redir -> delimeter);
		if ((ft_strncmp(line, redir -> delimeter, length) == 0))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, redir -> fd);
		free(line);
	}
	close(redir -> fd);
}

static char	*get_tmpfile_name(t_group *group, int n)
{
	char	*filename;
	char	*run_number;

	run_number = ft_itoa(n);
	if (run_number == NULL)
		clear_and_exit(group, NULL, "malloc");
	filename = ft_strjoin(TMP_FILE_PREFIX, run_number);
	free(run_number);
	if (filename == NULL)
		clear_and_exit(group, NULL, "malloc");
	return (filename);
}
