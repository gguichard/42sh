/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 10:23:34 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/09 22:21:22 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <term.h>
#include <unistd.h>
#include "shell.h"
#include "vars.h"
#include "cmdline.h"

void	reset_cmdline(t_cmdline *cmdline, const char *prompt, size_t offset)
{
	cmdline->saved_col = -1;
	cmdline->konami_code = 0;
	ft_memset(cmdline->input.buffer, 0, cmdline->input.capacity);
	cmdline->input.offset = 0;
	cmdline->input.size = 0;
	cmdline->input.reading = RSTATE_READING;
	cmdline->prompt.str = prompt;
	cmdline->prompt.offset = offset;
	print_prompt_and_cmdline(cmdline);
}

int		init_cmdline(t_alloc *alloc, t_cmdline *cmdline)
{
	const char			*term;

	g_cmdline = cmdline;
	cmdline->alloc = alloc;
	term = get_var_value(alloc->vars, "TERM");
	cmdline->stdin_dup = dup(STDIN_FILENO);
	if (term == NULL || term[0] == '\0')
		term = "xterm-256color";
	if (tgetent(NULL, term) == -1)
		return (0);
	ft_memset(cmdline->input.buffer, 0, sizeof(cmdline->input.buffer));
	cmdline->input.capacity = sizeof(cmdline->input.buffer) - 1;
	return (1);
}
