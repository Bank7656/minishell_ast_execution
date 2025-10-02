/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 07:11:23 by thacharo          #+#    #+#             */
/*   Updated: 2025/08/22 21:01:41 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	free_command_node(t_ast_node *node);

void  clear_and_exit(t_group *group, t_ast_node *node, char *cmd)
{
  clear_ast(group -> ast_root);
  free(group);
  if (ft_strncmp(cmd, "access", -1) == 0)
  {
    //if ()
    exit(COMMAND_NOT_FOUND);
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
	char **trav;

  t_list *redir_trav1;
  t_list *redir_trav2;

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

  t_list *temp_lst;
  t_list *temp_trav;

  t_redir *redir;
  temp_lst = node -> data.exec.redir;
  while (temp_lst != NULL)
  {
      redir = (t_redir *)(temp_lst -> content); 
      if (redir -> type == HEREDOC)
      {
        printf("%s\n", redir -> filename);
        unlink(redir -> filename);
        perror("unlink");
      }
      free(redir -> filename);
      if (redir -> delimeter != NULL)
        free(redir -> delimeter);
      free(redir);
      temp_trav = temp_lst;
      temp_lst = temp_lst -> next;
      free(temp_trav);
  }

	free(node);
	return ;
}
