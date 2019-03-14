/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:39:44 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 13:40:39 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"
#include <term.h>

int	main(int argc, char **argv)
{
	t_cmdline	cmdline;

	(void)argc;
	(void)argv;
	tgetent(NULL, "xterm-256color");
	ft_memset(&cmdline, 0, sizeof(t_cmdline));
	cmdline.saved_col = -1;
	setup_term();
	update_winsize(&cmdline);
	cmdline.prompt.type = PROMPT_DEFAULT;
	ft_printf("blabla");
	print_prompt(&cmdline);
	set_cursor_pos(&cmdline.cursor);
	cmdline.prompt.offset = cmdline.cursor.x;
	read_input(&cmdline);
	reset_term();
	return (0);
}
