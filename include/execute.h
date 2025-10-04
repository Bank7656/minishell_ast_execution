/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:35:01 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/04 16:19:12 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

# define STDIN_FILENO 0
# define STDOUT_FILENO 1
# define STDERR_FILENO 2
# define FILE_PERMISSION 0644
# define EXEC_CMD_CANNOT_EXECUTE 126
# define EXEC_CMD_NOT_FOUND 127
# define MAX_PROCESSES 256

# define TMP_FILE_PREFIX "/tmp/tempfile_"

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>

# include "../libft/libft.h"

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPELINE,
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
  int	fd;
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
      t_list  *redir;
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

//execute.c
void		execution(t_group *group, t_ast_node *node);
int    execute_ast(t_group *group, t_ast_node *node, bool is_pipeline);

//execute_pipeline.c
int  execute_pipeline(t_group *group, t_ast_node *node);

// execute_redirect.c
void  redirection(t_group *group, t_ast_node *node);

// execute_heredoc.c
void  prepare_heredoc(t_group *group, t_ast_node *node, int *run_id);
// execute_utils.c
void		*clear_ast(t_ast_node *node);

// Signal_handler.c
void  handle_signal(int signal);

// commands_utils.c
pid_t    ft_fork(t_group *group);
void     ft_pipe(t_group *group, int pipe_fd[2], int prev_fd);
void  ft_execve(t_group *group, struct s_command *cmd);
void     clear_and_exit(t_group *group, t_ast_node *node, char *cmd);
#endif
