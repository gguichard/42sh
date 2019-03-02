/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utf8.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/21 11:48:28 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/22 17:31:16 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

size_t	check_prev_char(t_buf *input)
{
	int				x;
	size_t			size;

	x = input->x - 4;
	while (x < (int)input->x)
	{
		if (x >= 0 && (size = lenbyte(input->s[x])) == input->x - x)
			return (size);
		x += 1;
	}
	return (1);
}

size_t	lenbyte(char k)
{
	if ((unsigned char)k <= 127)
		return (1);
	else if ((unsigned char)k >= 0xC0 && (unsigned char)k <= 0xDF)
		return (2);
	else if ((unsigned char)k >= 0xE0 && (unsigned char)k <= 0xEF)
		return (3);
	else if ((unsigned char)k >= 0xF0 && (unsigned char)k <= 0xF4)
		return (4);
	return (0);
}

size_t	ft_strlen_u8(const char *s)
{
	size_t	x;
	size_t	size;
	size_t	byte;

	if (!s)
		return (0);
	x = 0;
	size = 0;
	while (s[x])
	{
		if ((byte = lenbyte(s[x])) > 1)
		{
			while (byte && s[x])
			{
				x += 1;
				byte -= 1;
			}
		}
		else
			x += 1;
		size += 1;
	}
	return (size);
}

void	add_u8_buff(t_alloc *al, size_t nb_byte, unsigned char *uc,
						size_t prev_line)
{
	size_t	x;
	char	*s;

	x = 0;
	(!(s = ft_strjoin(al->input->s, (char *)uc))) ? ft_exit_malloc() : 0;
	check_over_buffer(al->input, s);
	ft_memdel((void **)&s);
	ft_memmove(al->input->s + al->input->x + nb_byte,
				al->input->s + al->input->x,
				ft_strlen(al->input->s + al->input->x) + nb_byte);
	while (x < nb_byte)
		al->input->s[al->input->x++] = (char)uc[x++];
	if (((display_sizing(0) + ft_strlen_u8(al->input->s))
		/ window_width_size()) != prev_line)
		reactualize_output(al->input, al->env);
}

void	input_u8(t_alloc *al, unsigned char k, size_t nb_byte)
{
	unsigned char	uc[5];
	size_t			prev_line;

	uc[0] = k;
	uc[4] = 0;
	if (read(0, uc + 1, nb_byte - 1) == -1)
		return ;
	prev_line = al->input->pos.l;
	put_term_rout("im");
	if (isatty(0) && isatty(1))
		write(1, uc, nb_byte);
	else if (isatty(0))
		write(2, uc, nb_byte);
	if (al->input->pos.c + 1 == (size_t)window_width_size())
	{
		al->input->pos.l += 1;
		write_str("c", 0);
		al->input->pos.c = 1;
		cursor_movement(al->input, -2);
		put_term_rout("dc");
	}
	else
		al->input->pos.c += 1;
	put_term_rout("ei");
	add_u8_buff(al, nb_byte, uc, prev_line);
}
