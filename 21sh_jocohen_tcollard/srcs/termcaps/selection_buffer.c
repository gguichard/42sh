/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection_buffer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 14:24:09 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/21 16:05:36 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_buf	*selec_buffer(int t, t_buf *input, t_env **lst)
{
	static t_buf	selection;

	if (t)
		return (&selection);
	else if (selection.s)
	{
		reactualize_output(input, lst);
		ft_memdel((void **)&(selection.s));
	}
	return (0);
}

t_buf	*alloc_selec_buff(int dir)
{
	t_buf	*selec;

	selec = selec_buffer(1, 0, 0);
	if (!selec->s)
	{
		selec->buf_size = 64;
		if (!(selec->s = ft_memalloc(selec->buf_size)))
			ft_exit_malloc();
		selec->x = 0;
		selec->pos.l = 0;
	}
	else
		check_over_buffer(selec, 0);
	(!selec->s[0]) ? selec->pos.c = dir : 0;
	return (selec);
}

void	expand_selec(t_buf *selec, t_buf *input, t_env **lst, int dir)
{
	if (dir == -1 && selec->pos.l != 1)
	{
		ft_memmove(selec->s + 1, selec->s, selec->x);
		selec->s[0] = input->s[input->x];
		selec->x += 1;
		(!input->x) ? selec->pos.l = 1 : 0;
		(input->x) ? cursor_movement(input, -1) : 0;
	}
	else if (dir == 1)
	{
		selec->s[selec->x++] = input->s[input->x];
		cursor_movement(input, 1);
	}
	selec->pos.c = dir;
	redisplay_line_selec(selec, input, lst);
}

void	selection_init(t_buf *input, int dir, t_env **lst)
{
	t_buf	*selec;

	selec = alloc_selec_buff(dir);
	if ((size_t)dir != selec->pos.c)
	{
		(!selec->pos.l) ? cursor_movement(input, dir) : 0;
		selec->x -= 1;
		selec->pos.l = 0;
		(dir == -1) ? selec->s[selec->x] = 0 : 0;
		(dir == 1) ? ft_memmove(selec->s, selec->s + 1, selec->x + 1) : 0;
		(!selec->x) ? ft_memdel((void **)&(selec->s)) : 0;
		redisplay_line_selec(selec, input, lst);
		return ;
	}
	if (input->x == ft_strlen(input->s) || selec->pos.l == 1)
	{
		(input->x && dir == -1) ? cursor_movement(input, -1) : 0;
		return ;
	}
	expand_selec(selec, input, lst, dir);
}
