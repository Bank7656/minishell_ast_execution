/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 22:50:19 by thacharo          #+#    #+#             */
/*   Updated: 2025/08/24 18:06:42 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

#include <string.h>
#include "../include/execute.h"

static void test_0(int *n, char **envp);
static void test_1(int *n, char **envp);
static void test_2(int *n, char **envp);
static void test_3(int *n, char **envp);
static void test_4(int *n, char **envp);
static void test_5(int *n, char **envp);

static void  print_test(int *n, char *cmd);
static void  print_catagory(char *cmd);

static char **get_dummy_args(char **args);
static t_ast_node	*create_ast_node(char *cmd, char **args, char **envp);
static t_ast_node	*create_pipeline_node(t_ast_node *left, t_ast_node *right);



void  test(char **envp)
{
  int n = 1;

  print_catagory("Simple Commands");
  test_0(&n, envp);
  test_1(&n, envp);

  print_catagory("Zombie process");
  test_2(&n, envp);
  test_3(&n, envp);
  test_4(&n, envp);

  print_catagory("Check Exit code");
  test_5(&n, envp);
}

static void test_0(int *n, char **envp)
{
  t_ast_node *node_1 = create_ast_node("/bin/ls", (char *[]){"ls", "-la", NULL}, envp);

  t_group *group = create_group(node_1);

  print_test(n, "ls -la");
  execution(group, node_1);
  printf("\n");

  clear_ast(group -> ast_root);
  free(group);

}
static void test_1(int *n, char **envp)
{
  t_ast_node *node_1 = create_ast_node("/bin/cat", (char *[]){"cat", "-e", "dockerfile", NULL}, envp);
	t_ast_node *node_2 = create_ast_node("/bin/cat", (char *[]){"cat", "-e", NULL}, envp);
	t_ast_node *node_3 = create_ast_node("/bin/cat", (char *[]){"cat", "-e", NULL}, envp);
	t_ast_node *pipe_node1 = create_pipeline_node(node_2, node_3);
	t_ast_node *pipe_node2 = create_pipeline_node(node_1, pipe_node1);

  t_group *group = create_group(pipe_node2);

  print_test(n, "cat -e dockerfile | cat -e | cat -e");
  execution(group, pipe_node2);
  printf("\n");

  clear_ast(group -> ast_root);
  free(group);

}

static void test_2(int *n, char **envp)
{
  t_ast_node *node_1 = create_ast_node("/usr/bin/sleep", (char *[]){"sleep", "1", NULL}, envp);
	t_ast_node *node_2 = create_ast_node("/usr/bin/sleep", (char *[]){"sleep", "3", NULL}, envp);
	t_ast_node *node_3 = create_ast_node("/usr/bin/sleep", (char *[]){"sleep", "5", NULL}, envp);
	t_ast_node *pipe_node1 = create_pipeline_node(node_2, node_3);
	t_ast_node *pipe_node2 = create_pipeline_node(node_1, pipe_node1);
   t_group *group = create_group(pipe_node2);

  print_test(n, "sleep 1 | sleep 3 | sleep 5");
  execution(group, pipe_node2);
  printf("\n");
  clear_ast(group -> ast_root);
  free(group);
}

static void test_3(int *n, char **envp)
{
  t_ast_node *node_1 = create_ast_node("/usr/bin/sleep", (char *[]){"sleep", "1", NULL}, envp);
	t_ast_node *node_2 = create_ast_node("/usr/bin/sleep", (char *[]){"sleep", "5", NULL}, envp);
	t_ast_node *node_3 = create_ast_node("/usr/bin/sleep", (char *[]){"sleep", "3", NULL}, envp);
	t_ast_node *pipe_node1 = create_pipeline_node(node_2, node_3);
	t_ast_node *pipe_node2 = create_pipeline_node(node_1, pipe_node1);
   t_group *group = create_group(pipe_node2);

  print_test(n, "sleep 1 | sleep 5 | sleep 3");
  execution(group, pipe_node2);
  printf("\n");
  clear_ast(group -> ast_root);
  free(group);
}

static void test_4(int *n, char **envp)
{
  t_ast_node *node_1 = create_ast_node("/usr/bin/sleep", (char *[]){"sleep", "10", NULL}, envp);
	t_ast_node *node_2 = create_ast_node("/usr/bin/sleep", (char *[]){"sleep", "5", NULL}, envp);
	t_ast_node *node_3 = create_ast_node("/usr/bin/sleep", (char *[]){"sleep", "1", NULL}, envp);
	t_ast_node *pipe_node1 = create_pipeline_node(node_2, node_3);
	t_ast_node *pipe_node2 = create_pipeline_node(node_1, pipe_node1);
   t_group *group = create_group(pipe_node2);

  print_test(n, "sleep 5 | sleep 3 | sleep 1");
  execution(group, pipe_node2);
  printf("\n");
  clear_ast(group -> ast_root);
  free(group);
}

static void test_5(int *n, char **envp)
{
  t_ast_node *node_1 = create_ast_node("/usr/bin/echo", (char *[]){"echo", "Hello", NULL}, envp);
	t_ast_node *node_2 = create_ast_node("/usr/bin/echo", (char *[]){"echo", "World", NULL}, envp);
	t_ast_node *node_3 = create_ast_node("/bin/cat", (char *[]){"cat", "-w", "Test", NULL}, envp);
	t_ast_node *pipe_node1 = create_pipeline_node(node_2, node_3);
	t_ast_node *pipe_node2 = create_pipeline_node(node_1, pipe_node1);
   t_group *group = create_group(pipe_node2);

  print_test(n, "echo Hello | echo World | cat -w");
  execution(group, pipe_node2);
  printf("\n");
  clear_ast(group -> ast_root);
  free(group);
}

t_group *create_group(t_ast_node *node)
{
  t_group *group;

  group = (t_group *)malloc(sizeof(t_group));
  group -> ast_root = node;

  return (group);
}

static t_ast_node	*create_ast_node(char *cmd, char **args, char **envp)
{
	t_ast_node *node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node -> type = NODE_COMMAND;
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
	node -> data.tree.left = left;
	node -> data.tree.right = right;
	return (node);
}

static void  print_test(int *n, char *cmd)
{
  printf("%s[Test %i]: %s%s\n", GREEN, *n, cmd, RESET);
  (*n)++;
  return ;
}

static void  print_catagory(char *cmd)
{
  printf("%s%12s%s test%s%s\n\n", GREEN, "[",cmd, "]", RESET);
  return ;
}
