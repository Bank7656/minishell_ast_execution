/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 00:39:17 by thacharo          #+#    #+#             */
/*   Updated: 2025/08/22 16:31:05 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	main(int argc, char **argv, char **envp)
{

	printf("Hello world\n");
	t_ast_node *root_node = create_dummy_ast(envp);

	execution(root_node);

	clear_ast(root_node);

	return (0);
}