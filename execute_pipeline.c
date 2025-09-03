#include "execute.h"

static pid_t execute_left_child(t_ast_node *node, int pipe_fd[2]);
static pid_t execute_right_child(t_ast_node *node, int pipe_fd[2]);

int  execute_pipeline(t_ast_node *node)
{
  int pipe_fd[2];
  pid_t left_fork;
  pid_t right_fork;

  if (pipe(pipe_fd) == -1)
    clear_and_exit(node, "pipe");

  left_fork = execute_left_child(node, pipe_fd);
  right_fork = execute_right_child(node, pipe_fd);

  close(pipe_fd[0]);
  close(pipe_fd[1]);
  wait(NULL);
  wait(NULL);
  return ;
}

static pid_t execute_left_child(t_ast_node *node, int pipe_fd[2])
{
  pid_t left_pid;

  left_pid = ft_fork(node);
  if (left_pid == 0)
  {
    close(STDOUT_FILENO);
    dup(pipe_fd[1]);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    execute_ast(node -> data.tree.left);
  }
  return (left_pid); 
}

static pid_t execute_right_child(t_ast_node *node, int pipe_fd[2])
{
  pid_t right_pid;

  right_pid = ft_fork(node);
    if (right_pid == 0)
    {
      close(STDIN_FILENO);
      dup(pipe_fd[0]);
      close(pipe_fd[0]);
      close(pipe_fd[1]);
      execute_ast(node -> data.tree.right);
    }
  return (right_pid); 
}


