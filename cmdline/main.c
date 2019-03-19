/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:39:44 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/19 10:13:51 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <term.h>
#include <signal.h>
#include "cmdline.h"

t_cmdline	*g_cmdline = NULL;

int			main(int argc, char **argv)
{
	t_cmdline	cmdline;

	(void)argc;
	(void)argv;
	tgetent(NULL, "xterm-256color");
	ft_memset(&cmdline, 0, sizeof(t_cmdline));
	g_cmdline = &cmdline;
	setup_term();
	signal(SIGWINCH, handle_sigwinch);
	update_winsize(&cmdline);
	ft_printf("blabla");
	cmdline.prompt.type = PROMPT_DEFAULT;
	read_input(&cmdline, get_prompt(&cmdline));
	reset_term();
	free(cmdline.input.buffer);
	free(cmdline.visual.clipboard);
	return (0);
}
