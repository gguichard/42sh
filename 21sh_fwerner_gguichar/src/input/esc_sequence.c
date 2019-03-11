/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   esc_sequence.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:03:22 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 13:54:55 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "history.h"
#include "utils.h"

const t_seq		g_seqs[] = {
	{ESC_DEL_KEY, handle_del_key},
	{ESC_SEQ_LEFT, move_cursor_left},
	{ESC_SEQ_RIGHT, move_cursor_right},
	{ESC_SEQ_UP, history_up},
	{ESC_SEQ_DOWN, history_down},
	{ESC_SEQ_PAGEUP, history_up},
	{ESC_SEQ_PAGEDOWN, history_down},
	{ESC_HOME_KEY, move_cursor_home},
	{ESC_END_KEY, move_cursor_end},
	{ESC_SHIFT_LEFT, move_cursor_prev_word},
	{ESC_SHIFT_RIGHT, move_cursor_next_word},
	{ESC_SHIFT_UP, move_cursor_up},
	{ESC_SHIFT_DOWN, move_cursor_down},
	{ESC_FN_F1, vm_toggle},
	{NULL, NULL}
};

const t_seq		*get_valid_esc_sequence(t_term *term)
{
	int	index;

	index = 0;
	while (g_seqs[index].str != NULL)
	{
		if (ft_strnequ(term->seq, g_seqs[index].str, term->seq_off))
			return (&(g_seqs[index]));
		index++;
	}
	return (NULL);
}

int				handle_esc_key(t_shell *shell, t_term *term, char key)
{
	const t_seq	*seq;
	int			is_valid;

	if (!(term->esc_seq))
	{
		if (key != 27)
			return (0);
		term->esc_seq = 1;
	}
	(term->seq)[term->seq_off] = key;
	(term->seq_off)++;
	seq = get_valid_esc_sequence(term);
	is_valid = seq != NULL && (seq->str)[term->seq_off] == '\0';
	if (is_valid)
		handle_esc_sequence(shell, term, seq);
	if (seq == NULL || is_valid || term->seq_off == MAX_ESC_SEQ_BYTES)
	{
		term->esc_seq = 0;
		term->seq_off = 0;
		return (is_valid ? 1 : 0);
	}
	return (1);
}

void			handle_esc_sequence(t_shell *shell, t_term *term
		, const t_seq *seq)
{
	if (!seq->f(shell, term))
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	if (term->visual_mode)
	{
		if (term->size > 0 && term->cursor >= term->size)
		{
			tputs(tgetstr("bl", NULL), 1, t_putchar);
			term->cursor = term->size - 1;
		}
		term->select.end = term->cursor;
		go_to_prompt(term);
		print_input(shell, term);
	}
}
