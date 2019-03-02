/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_fit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 17:47:38 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/21 19:25:43 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	check_last_char_column(t_buf *input)
{
	if (input->pos.l && !input->pos.c && isatty(0))
	{
		put_term_rout("im");
		write_str("c", 0);
		input->pos.c += 1;
		cursor_movement(input, -2);
		put_term_rout("dc");
		put_term_rout("ei");
	}
}

void	reactualize_output(t_buf *input, t_env **lst)
{
	t_cursor	prev;

	if (!isatty(0))
		return ;
	prev.c = input->pos.c;
	prev.l = input->pos.l;
	check_last_char_column(input);
	delete_line_pos(input, lst);
	replace_cursor(input, prev.c, prev.l);
}

int		window_width_size(void)
{
	struct winsize	wind;

	if (ioctl(0, TIOCGWINSZ, &wind) == -1)
		return (1);
	if (!wind.ws_col)
		return (1);
	return ((int)wind.ws_col);
}

void	replace_cursor(t_buf *input, size_t c, size_t l)
{
	if (input->pos.l > l || (input->pos.l == l && input->pos.c > c))
	{
		while (input->pos.l != l || input->pos.c != c)
			cursor_movement(input, -2);
	}
	else if (input->pos.l < l || (input->pos.l == l && input->pos.c < c))
	{
		while (input->pos.l != l || input->pos.c != c)
			cursor_movement(input, 2);
	}
}

void	check_resize_curs_pos(t_buf *input)
{
	size_t		x;
	size_t		l;

	l = input->pos.c;
	x = display_sizing(0) + input->x;
	input->pos.l = x / window_width_size();
	input->pos.c = x % window_width_size();
	if ((l == (size_t)window_width_size() ||
		(input->pos.l && l == 1)) && !input->pos.c && !input->s[input->x])
	{
		write_str("c", 0);
		input->pos.c += 1;
		cursor_movement(input, -2);
		put_term_rout("dc");
	}
}
