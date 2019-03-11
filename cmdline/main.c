/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:39:44 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/11 17:12:51 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

int	main(int argc, char **argv)
{
	t_cmdline	cmdline;

	(void)argc;
	(void)argv;
	ft_memset(&cmdline, 0, sizeof(t_cmdline));
	setup_term();
	cmdline.prompt_type = PROMPT_DEFAULT;
	print_prompt(&cmdline);
	read_input(&cmdline);
	reset_term();
	return (0);
}
