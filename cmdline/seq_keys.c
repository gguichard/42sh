/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   seq_keys.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 10:16:08 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/17 00:57:39 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <term.h>
#include <stdlib.h>
#include "cmdline.h"

static t_seq	*get_known_sequences(void)
{
	static t_seq	seqs[] = {
		{"\033[D", MODE_COMMON, handle_move_left},
		{"\033[C", MODE_COMMON, handle_move_right},
		{"\033[1;2D", MODE_COMMON, handle_prev_word},
		{"\033[1;2C", MODE_COMMON, handle_next_word},
		{"\033[3~", MODE_INSERT, handle_delete_key},
		{"\177", MODE_INSERT, handle_backspace_key},
		{"\033\012", MODE_INSERT, handle_test_newline},
		{"\033[H", MODE_COMMON, handle_home_key},
		{"\033[F", MODE_COMMON, handle_end_key},
		{"\033[1;2A", MODE_COMMON, handle_cursor_up},
		{"\033[1;2B", MODE_COMMON, handle_cursor_down},
		{"\033[1;2H", MODE_COMMON, handle_line_start},
		{"\033[1;2F", MODE_COMMON, handle_line_end},
		{"\033OP", MODE_COMMON, handle_toggle_visual},
		{"d", MODE_VISUAL, handle_cut_key},
		{"y", MODE_VISUAL, handle_copy_key},
		{"p", MODE_VISUAL, handle_paste_key},
		{"P", MODE_VISUAL, handle_paste_before_key},
		{"j", MODE_VISUAL, handle_cursor_down},
		{"k", MODE_VISUAL, handle_cursor_up},
		{"b", MODE_VISUAL, handle_prev_word},
		{"h", MODE_VISUAL, handle_move_left},
		{"l", MODE_VISUAL, handle_move_right},
		{"w", MODE_VISUAL, handle_next_word},
		{"gg", MODE_VISUAL, handle_home_key},
		{"G", MODE_VISUAL, handle_end_key},
		{"0", MODE_VISUAL, handle_line_start},
		{"$", MODE_VISUAL, handle_line_end},
		{":q", MODE_VISUAL, handle_toggle_visual},
		{"\033\033", MODE_VISUAL, handle_toggle_visual},
		{NULL, MODE_COMMON, NULL}
	};

	return (seqs);
}

static void		reset_sequence(t_seq_keys *keys)
{
	ft_memset(keys->buffer, 0, 8);
	keys->size = 0;
	keys->offset = 0;
}

void			handle_sequence(t_cmdline *cmdline, const char *seq)
{
	t_seq	*seqs;
	int		idx;

	if (seq[cmdline->seq_keys.offset] == '\0')
	{
		reset_sequence(&cmdline->seq_keys);
		seqs = get_known_sequences();
		idx = 0;
		while (seqs[idx].fn != NULL)
		{
			if (ft_strequ(seqs[idx].str, seq))
			{
				if (!seqs[idx].fn(cmdline))
					tputs(tgetstr("bl", NULL), 1, t_putchar);
				else if (cmdline->visual.toggle)
					update_visual_select(cmdline);
				break ;
			}
			idx++;
		}
	}
}

static int		is_right_seq_mode(t_cmdline *cmdline, t_seq *seq)
{
	return (seq->mode == MODE_COMMON
			|| (seq->mode == MODE_VISUAL && cmdline->visual.toggle)
			|| (seq->mode == MODE_INSERT && !cmdline->visual.toggle));
}

const char		*get_sequence(t_cmdline *cmdline, char c)
{
	t_seq	*seqs;
	int		idx;

	cmdline->seq_keys.buffer[cmdline->seq_keys.offset] = c;
	cmdline->seq_keys.size += 1;
	seqs = get_known_sequences();
	idx = 0;
	while (seqs[idx].fn != NULL)
	{
		if (is_right_seq_mode(cmdline, seqs + idx)
				&& ft_strnequ(seqs[idx].str, cmdline->seq_keys.buffer
					, cmdline->seq_keys.size))
		{
			cmdline->seq_keys.offset += 1;
			return (seqs[idx].str);
		}
		idx++;
	}
	reset_sequence(&cmdline->seq_keys);
	return (NULL);
}
