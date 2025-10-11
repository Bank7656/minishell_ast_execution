/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 21:13:09 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/05 22:40: by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static t_envp *create_env_node(t_group *group, char *env_var);

t_list *arr_to_lst(t_group *group, char **envp)
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
	return (NULL);
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