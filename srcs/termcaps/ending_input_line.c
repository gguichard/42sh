/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ending_input_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 17:47:38 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/27 12:20:04 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*recall_prompt(t_alloc *al, int type)
{
	t_alloc			alloc;
	static char		*input;
	char			*out;

	if (type == -1)
		return (input);
	else if (type == 1)
		input = "heredoc";
	else if (type == 2)
		input = "dquote";
	else if (type == 3)
		input = "quote";
	else if (type == 4)
		input = "bquote";
	else if (type == 5)
		input = "pipe";
	else if (type == 6)
		input = "cmdand";
	(type == 7) ? input = "cmdor" : 0;
	alloc.history = al->history;
	alloc.env = al->env;
	out = read_input(&alloc);
	input = 0;
	(!out) ? g_pid = 0 : 0;
	return (out);
}

char	*enter_section(t_alloc *al, int read)
{
	size_t		x;
	char		*s;

	end_hist(al->history);
	x = (((display_sizing(0) + ft_strlen_u8(al->input->s) - 1)
		/ window_width_size()) - (al->input->pos.l - 1)) + 1;
	while (isatty(0) && --x)
		put_term_rout("do");
	put_term_rout("cr");
	if (read == -1)
		return (al->input->s);
	if (!read)
	{
		if (!(s = ft_strdup(al->input->s)))
			ft_exit_malloc();
		lexer(s, al->env, al);
		if (g_pid == -1)
			write_str("\n", 0);
		g_pid = 0;
	}
	al->input->x = 0;
	ft_bzero(al->input->s, al->input->buf_size);
	caller_display(*al->env, al->input, 1);
	return (0);
}
