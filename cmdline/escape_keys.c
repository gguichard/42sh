/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:16:08 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/12 22:05:12 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "cmdline.h"

t_esc_seq	*get_esc_seqs(void)
{
	static t_esc_seq	seqs[] = {
		{"\033[D", MODE_COMMON, handle_move_left},
		{"\033[C", MODE_COMMON, handle_move_right},
		{"\033[1;2D", MODE_COMMON, handle_prev_word},
		{"\033[1;2C", MODE_COMMON, handle_next_word},
		{"\033[3~", MODE_INSERT, handle_delete_key},
		{"\177", MODE_INSERT, handle_backspace_key},
		{"\033\012", MODE_INSERT, handle_test_newline},
		{NULL, MODE_COMMON, NULL}
	};

	return (seqs);
}

static void	reset_escape_keys(t_esc_keys *keys)
{
	ft_memset(keys->buffer, 0, 8);
	keys->size = 0;
	keys->offset = 0;
}

void		handle_sequence_char(t_cmdline *cmdline, const char *seq, char c)
{
	t_esc_seq	*seqs;
	int			idx;

	if (seq[cmdline->esc_keys.offset] == '\0')
	{
		reset_escape_keys(&cmdline->esc_keys);
		idx = 0;
		while (seqs[idx].fn != NULL)
		{
			if (ft_strequ(seqs[idx].str, seq))
			{
				seqs[idx].fn(cmdline);
				break ;
			}
			idx++;
		}
	}
}

const char	*get_escape_sequence(t_esc_keys *keys, char c)
{
	t_esc_seq	*seqs;
	int			idx;

	keys->buffer[keys->offset] = c;
	keys->size += 1;
	seqs = get_esc_seqs();
	idx = 0;
	while (seqs[idx].fn != NULL)
	{
		if (ft_strnequ(seqs[idx].str, keys->buffer, keys->size))
		{
			keys->offset += 1;
			return (seqs[idx].str);
		}
		idx++;
	}
	reset_escape_keys(keys);
	return (NULL);
}
