/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_analysis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 11:18:48 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/22 21:47:27 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	input_visual_char(t_alloc *al, char k)
{
	size_t			prev_line;

	if (k > 31 && k < 127)
	{
		prev_line = al->input->pos.l;
		(isatty(0)) ? input_character(al->input, k) : 0;
		ft_memmove(al->input->s + al->input->x + 1, al->input->s + al->input->x,
			ft_strlen(al->input->s + al->input->x) + 1);
		al->input->s[al->input->x++] = k;
		if (((display_sizing(0) + ft_strlen_u8(al->input->s))
			/ window_width_size()) != prev_line)
			reactualize_output(al->input, al->env);
	}
	else if (g_u8 && (unsigned char)k >= 0xC0 && (unsigned char)k <= 0xF4)
		input_u8(al, (unsigned char)k, lenbyte(k));
}

void	analyse_input(t_alloc *al, char k)
{
	if (k == 12)
	{
		put_term_rout("cl");
		caller_display(*al->env, al->input, 0);
		reactualize_output(al->input, al->env);
	}
	else if (k == 27)
		escape_analysis(al);
	else if (k == 4 || (k == 127 && al->input->x))
	{
		(k == 4) ? del_char(al->input, 1, al->env) : 0;
		(k == 127) ? del_char(al->input, 0, al->env) : 0;
	}
	else
		input_visual_char(al, k);
}
