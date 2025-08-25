/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:35:04 by thacharo          #+#    #+#             */
/*   Updated: 2025/08/25 09:49:00 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	execute_ast(t_ast_node *node);
void	execute_command(t_ast_node *node);
void	execute_pipeline(t_ast_node *node);

void	execution(t_ast_node *node)
{
	execute_ast(node);
	for (int i = 0; i < 1; i++)
		wait(NULL);
	printf("Finish AST\n");
	return ;
}

void	execute_ast(t_ast_node *node)
{
	if (node -> type == NODE_COMMAND)
		execute_command(node);
	else if (node -> type == NODE_PIPELINE)
		execute_pipeline(node);
	return ;
}



void	execute_command(t_ast_node *node)
{
	pid_t	pid;
	char	*pathname;
	char	**argv;
	char	**envp;

	pathname = node -> data.exec.commands;
	argv = node -> data.exec.arguments;
	envp = node -> data.exec.envp;

	dprintf(2, "Fork\n");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		dprintf(2, "execution\n");
		if (node -> data.exec.side == NODE_LEFT)
		{
			if (dup2((node -> current_pipe)[1], STDOUT_FILENO) == -1) 
				perror("dup2");
			close((node -> current_pipe)[0]);
			close((node -> current_pipe)[1]);
			close((node -> previous_pipe)[0]);
			close((node -> previous_pipe)[1]);
		}
		else if (node -> data.exec.side == NODE_RIGHT)
		{
			if (dup2((node -> current_pipe)[0], STDIN_FILENO) == -1) 
				perror("dup2");
			if (node -> previous_pipe != NULL)
			{
				if (dup2((node -> previous_pipe)[1], STDOUT_FILENO) == -1)
					perror("dup2");
			}
			close((node -> current_pipe)[0]);
			close((node -> current_pipe)[1]);
		}
		if (execve(pathname, argv, envp) == - 1)
			perror("execve");
	}

}

// void	execute_pipeline(t_ast_node *node)
// {
// 	int	pipefd[2];
// 	pid_t	pid_left;
// 	pid_t	pid_right;

// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}
	
// 	pid_left = fork();
// 	if (pid_left == 0)
// 	{
// 		close(pipefd[0]);
// 		dup2(pipefd[1], STDOUT_FILENO);
// 		close(pipefd[1]);
// 		execute_ast(node -> data.tree.left);
// 	}
	
// 	pid_right = fork();
// 	if (pid_right == 0)
// 	{
// 		close(pipefd[1]);
// 		dup2(pipefd[0], STDIN_FILENO);
// 		close(pipefd[0]);
// 		execute_ast(node -> data.tree.right);
// 	}
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	waitpid(pid_left, NULL, 0);
// 	waitpid(pid_right, NULL, 0);
// }

// void	execute_pipeline(t_ast_node *node)
// {
// 	int		pipe_fd[2];
// 	t_ast_node *left_node;
// 	t_ast_node *right_node;
// 	pid_t	pid_left;
// 	pid_t	pid_right;

// 	// Create Pipeline
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}
	
// 	// Left Child
// 	pid_left = fork();
// 	if (pid_left == -1)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid_left == 0)
// 	{
// 		left_node = node -> data.tree.left;	
// 		left_node -> root_pipe = &pipe_fd[1];
// 		dup2(*(left_node -> root_pipe), STDOUT_FILENO);
// 		execute_ast(left_node);
// 	}

// 	// Right Child
// 	pid_right = fork();
// 	if (pid_right == -1)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid_right == 0)
// 	{
// 		right_node = node -> data.tree.right;	
// 		right_node -> root_pipe = &pipe_fd[0];
// 		dup2(*(right_node -> root_pipe), STDIN_FILENO);
// 		execute_ast(right_node);
// 	}

// 	// Close parent process
// 	close(pipe_fd[0]);
// 	close(pipe_fd[1]);
// 	return ;
// }


void	execute_pipeline(t_ast_node *node)
{
	int			pipe_fd[2];
	t_ast_node	*left_node;
	t_ast_node	*right_node;
	
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	left_node = node -> data.tree.left;
	right_node = node -> data.tree.right;
	left_node -> data.exec.side = NODE_LEFT;
	right_node -> data.exec.side = NODE_RIGHT;

	if (node -> current_pipe != NULL)
	{
		left_node -> previous_pipe = (int *)malloc(sizeof(int) * 2);
		right_node -> previous_pipe = (int *)malloc(sizeof(int) * 2);
		(left_node -> previous_pipe)[0] = (node -> current_pipe)[0];
		(left_node -> previous_pipe)[1] = (node -> current_pipe)[1];
		(right_node -> previous_pipe)[0] = (node -> current_pipe)[0];
		(right_node -> previous_pipe)[1] = (node -> current_pipe)[1];
		// printf("previous = (%i %i)\n", (left_node -> previous_pipe)[0], (left_node -> previous_pipe)[1]);
	}
	left_node -> current_pipe = pipe_fd;
	right_node -> current_pipe = pipe_fd;
	// printf("current_pipe = (%i %i)\n", (left_node -> current_pipe)[0], (left_node -> current_pipe)[1]);
	execute_ast(right_node);
	execute_ast(left_node);
	return ;
}
