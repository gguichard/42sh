/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 11:18:48 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/22 21:47:33 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*kill_or_give(t_alloc *al, char k)
{
	(k != 27 && k != -1) ? selec_buffer(0, al->input, al->env) : 0;
	if (k == 4 && !al->input->s[0] && !recall_prompt(al, -1))
	{
		put_term_rout("do");
		put_term_rout("cr");
		(!isatty(0)) ? write_str("\n", 0) : 0;
		ft_memdel((void **)&(al->input->s));
		return (0);
	}
	if (k == 10)
	{
		if (recall_prompt(al, -1))
			return (enter_section(al, -1));
		enter_section(al, 0);
	}
	return (al->input->s);
}

char	*read_and_sig(t_alloc *al, char *k, int stdin_cpy)
{
	check_over_buffer(al->input, 0);
	if (read(0, k, 1) <= 0)
	{
		if (!g_pid && !g_resize)
			return (0);
		*k = -1;
		dup2(stdin_cpy, 0);
		if (g_pid == -1 && recall_prompt(al, -1))
		{
			ft_memdel((void **)&(al->input->s));
			return (enter_section(al, -1));
		}
		(g_pid == -1) ? enter_section(al, 1) : 0;
		(g_resize == 1) ? check_resize_curs_pos(al->input) : 0;
		g_pid = 0;
		g_resize = 0;
	}
	check_prev_read(*k);
	return (k);
}

char	*read_input(t_alloc *al)
{
	char		k;
	t_buf		input;
	int			stdin_cpy;

	al->input = &input;
	al->input->buf_size = 128;
	(!(al->input->s = ft_memalloc(al->input->buf_size))) ? ft_exit_malloc() : 0;
	al->input->x = 0;
	caller_display(*al->env, al->input, 1);
	stdin_cpy = dup(0);
	while (1)
	{
		if (!read_and_sig(al, &k, stdin_cpy))
		{
			ft_memdel((void **)&(al->input->s));
			return (0);
		}
		if (!kill_or_give(al, k))
			break ;
		else if (k == 10 && recall_prompt(al, -1))
			return (al->input->s);
		(k != 10) ? analyse_input(al, k) : 0;
	}
	ft_memdel((void **)&(al->input->s));
	return (0);
}

void	prompt(t_alloc *al)
{
	t_historic	*history;

	init_hist(&history, *al->env);
	al->history = &history;
	set_terminal(0);
	read_input(al);
	ft_del_hist(&history);
	set_terminal(-1);
}
