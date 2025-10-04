/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thacharo <thacharo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 01:19:24 by thacharo          #+#    #+#             */
/*   Updated: 2025/10/05 01:20:05 by thacharo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	handle_signal(int signal)
{
	if (signal == SIGINT)
		printf("Bank got SIGINT\n");
	if (signal == SIGQUIT)
		printf("Bank got SIGQUIT\n");
}

void	signal_init(t_group *group)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		clear_and_exit(group, NULL, "SIGINT");
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		clear_and_exit(group, NULL, "SIGQUIT");
}
