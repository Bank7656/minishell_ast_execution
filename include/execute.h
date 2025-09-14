/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:35:01 by thacharo          #+#    #+#             */
/*   Updated: 2025/08/25 10:31:41 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define STDIN_FILENO 0
# define STDOUT_FILENO 1
# define STDERR_FILENO 2
# define MAX_PROCESSES 256

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>

# include "../libft/libft.h"

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPELINE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
}	t_node_type;

typedef enum e_redir_type
{
  REDIR_INPUT,
  REDIR_OUTPUT,
  APPEND,
  HEREDOC
} t_redir_type;

typedef struct s_redir
{
  t_redir_type type; 
  char  *filename;
  char  *delimeter;
  int   mode;
  struct s_redir *next;
} t_redir;

typedef struct s_ast_node
{
	t_node_type	type;
	union u_node_data
	{
		struct s_command
		{
			char		*commands;
			char		**arguments;
			char		**envp;
      t_list *redir_input;
      t_list *redir_output;
		} exec;
		struct s_operator
		{
			struct s_ast_node	*left;	
			struct s_ast_node	*right;	
		} tree;
	} data;
}	t_ast_node;

typedef struct s_group {
  t_ast_node *ast_root;
} t_group;

// Delete After finishing prototype
void  test(char **envp);
t_ast_node	*create_dummy_ast(char **envp);
t_group *create_group(t_ast_node *node);
// Delete After finishing prototype

//eexecute.c
void		execution(t_group *group, t_ast_node *node);
int    execute_ast(t_group *group, t_ast_node *node, bool is_pipeline);

//execute_pipeline.c
int  execute_pipeline(t_group *group, t_ast_node *node);

// execute_redirect.c
void  redirect_input(t_group *group, t_ast_node *node); 
void  redirect_output(t_group *group, t_ast_node *node); 

// execute_utils.c
void		*clear_ast(t_ast_node *node);

// commands_utils.c
pid_t    ft_fork(t_group *group);
void     ft_pipe(t_group *group, int pipe_fd[2], int prev_fd);
void     clear_and_exit(t_group *group, char *cmd);
#endif
