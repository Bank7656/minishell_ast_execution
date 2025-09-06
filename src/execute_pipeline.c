#include "execute.h"


static void  close_pipe(int pipe_fd[2]);
static pid_t execute_left_child(t_group *group, t_ast_node *node, int pipe_fd[2], int prev_fd);
static pid_t execute_right_child(t_group *group, t_ast_node *node, int pipe_fd[2]);
static int wait_all_child(pid_t last_pid);

int execute_pipeline(t_group *group, t_ast_node *node)
{
  int        prev_fd;
  int        pipe_fd[2];
  pid_t      left_pid;
  pid_t      last_pid;
  t_ast_node *right_node;
  t_ast_node *left_node;

  prev_fd = -1;
  while (true)
  {
    left_node = node -> data.tree.left;
    right_node = node -> data.tree.right;

    if (pipe(pipe_fd) == -1)
        clear_and_exit(group, "pipe");

    left_pid = execute_left_child(group, left_node, pipe_fd, prev_fd);

    if (prev_fd != -1)
    {
      close(prev_fd);
      prev_fd = -1;
    }
    close(pipe_fd[1]);
    if (right_node -> type != NODE_PIPELINE)
        break;
    prev_fd = pipe_fd[0];

    node = node -> data.tree.right;
  }
  last_pid = execute_right_child(group, right_node, pipe_fd);  
  close(pipe_fd[0]);
  return (wait_all_child(last_pid));
}

static int wait_all_child(pid_t last_pid)
{
  int status;
  int last_status;  
  pid_t pid;

  while (true)
  {
    pid = wait(&status);
    if (pid == last_pid)
    {
      if (WIFEXITED(status))
        last_status = WEXITSTATUS(status);
      break;
    }
  }
  return (last_status);
}

static pid_t execute_right_child(t_group *group, t_ast_node *node, int pipe_fd[2])
{
  pid_t right_pid;

  right_pid = ft_fork(group, node);
  if (right_pid == 0)
  {
    close(STDIN_FILENO);
    dup(pipe_fd[0]);
    execute_ast(group, node, true);
  }
  return (right_pid);
}

static pid_t execute_left_child(t_group *group, t_ast_node *node, int pipe_fd[2], int prev_fd)
{
  pid_t left_pid;

  left_pid = ft_fork(group, node);
  if (left_pid == 0)
  {
    if (prev_fd != -1)
    {
      close(STDIN_FILENO);
      dup(prev_fd);
    }
    close(STDOUT_FILENO);
    dup(pipe_fd[1]);
    close_pipe(pipe_fd);
    execute_ast(group, node, true);
  }
  return (left_pid); 
}



//static pid_t execute_left_child(t_ast_node *node, int pipe_fd[2])
//{
//  pid_t left_pid;
//
//  left_pid = ft_fork(node);
//  if (left_pid == 0)
//  {
//    close(STDOUT_FILENO);
//    dup(pipe_fd[1]);
//    close_pipe(pipe_fd);
//    execute_ast(node -> data.tree.left);
//  }
//  return (left_pid); 
//}

//static pid_t execute_right_child(t_ast_node *node, int pipe_fd[2])
//{
//  pid_t right_pid;
//
//  right_pid = ft_fork(node);
//  if (right_pid == 0)
//  {
//    close(STDIN_FILENO);
//    dup(pipe_fd[0]);
//    close_pipe(pipe_fd);
//    execute_ast(node -> data.tree.right);
//  }
//  return (right_pid); 
//}

static void  close_pipe(int pipe_fd[2])
{
  close(pipe_fd[0]);
  close(pipe_fd[1]);
}

//static int  wait_all_child(int last_pid)
//{
//  
//}
