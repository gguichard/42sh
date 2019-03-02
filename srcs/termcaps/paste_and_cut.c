/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste_and_cut.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 14:24:09 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/15 10:52:44 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	cut_selection(t_buf *input, t_buf *selec, t_env **lst)
{
	size_t	x;

	x = 0;
	if (!selec->s || !selec->s[0])
		return ;
	if (!selec->pos.l && (int)selec->pos.c == -1)
		x = input->x + 1;
	else if (selec->pos.c == 1)
		x = input->x - selec->x;
	ft_memmove(input->s + x, input->s + x + selec->x,
		(ft_strlen(input->s) - (x + selec->x) + 1));
	if (selec->pos.c == 1)
	{
		while (selec->x)
		{
			cursor_movement(input, -1);
			selec->x -= 1;
		}
	}
	(input->s[input->x]) ? cursor_movement(input, 1) : 0;
	selec_buffer(0, input, lst);
}

void	paste_intra_clip(t_buf *input, t_env **lst)
{
	size_t	x;

	if (!g_clip)
		return ;
	while (input->buf_size - 1 < ft_strlen(input->s) + ft_strlen(g_clip))
	{
		if (!(input->s = ft_realloc(input->s, input->buf_size + 128,
			input->buf_size)))
			ft_exit(0);
		input->buf_size += 128;
	}
	ft_memmove(input->s + input->x + ft_strlen(g_clip), input->s + input->x,
		ft_strlen(input->s) - input->x);
	ft_memcpy(input->s + input->x, g_clip, ft_strlen(g_clip));
	x = ft_strlen(g_clip) + 1;
	while (--x)
		cursor_movement(input, 1);
	reactualize_output(input, lst);
}
