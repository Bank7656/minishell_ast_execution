/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 02:32:49 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/12 02:39:15 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static t_envp	*create_env_node(t_group *group, char *env_var);
static char		*get_env_var(t_group *group, t_envp *env);

t_list *env_to_lst(t_group *group, char **envp)
{
	int		i;
	t_list	*lst;
	t_envp	*node;

	if (envp == NULL)
		return (NULL);
	i = 0;
	while (envp[i] != NULL)	
	{
		node = create_env_node(group, envp[i]);
		lst = ft_lstnew((void *)node);
		if (lst == NULL)
		{
			free(node);
			clear_and_exit(group, NULL, "lstnew");
		}
		ft_lstadd_back(&group -> env_list, lst);
		i++;
	}
	return (group -> env_list);
}

char	**env_lst_to_arr(t_group *group, t_list *lst)
{
	int		i;
	int		len;
	char	**arr;
	t_list	*trav;
	
	if (lst == NULL)
		return (NULL);
	len = ft_lstsize(lst);
	arr = (char **)malloc(sizeof(char *) * (len + 1));
	if (arr == NULL)
		clear_and_exit(group, NULL, "Malloc");
	i = 0;
	while (i < len)
	{
		arr[i] = get_env_var(group, (t_envp*)lst -> content);
		i++;
		lst = lst -> next;
	}
	arr[i] = NULL;
	return (arr);
}

static t_envp *create_env_node(t_group *group, char *env_var)
{
	int		values_len;
	char	*key_value_sep;
	t_envp	*env_dict;

	env_dict = (t_envp *)malloc(sizeof(t_envp)); 
	if (env_dict == NULL)
		clear_and_exit(group, NULL, "malloc");
	key_value_sep = ft_strchr(env_var, '=');
	env_dict -> key = ft_substr(env_var, 0, key_value_sep - env_var);
	if (env_dict -> key == NULL)
	{
		free(env_dict);
		clear_and_exit(group, NULL, "substr");
	}
	env_dict -> values = ft_substr(env_var, key_value_sep - env_var + 1, -1);
	if (env_dict -> values == NULL)
	{
		free(env_dict -> key);
		free(env_dict);
		clear_and_exit(group, NULL, "substr");
	}
	return (env_dict);
}

static char *get_env_var(t_group *group, t_envp *env)
{
	char	*tmp_str;
	char	*env_var;

	tmp_str = ft_strjoin(env -> key, "=");
	if (tmp_str == NULL)
		clear_and_exit(group, NULL, "Malloc");
	env_var = ft_strjoin(tmp_str, env -> values);
	free(tmp_str);
	if (env_var == NULL)
		clear_and_exit(group, NULL, "Malloc");
	return (env_var);
}

