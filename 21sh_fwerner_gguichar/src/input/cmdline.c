/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 09:53:37 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "input.h"
#include "utils.h"

void	reset_cmdline(t_shell *shell, t_prompt prompt)
{
	ft_strdel(&(shell->term.def_line));
	if (shell->term.legacy_mode)
		ft_strdel(&(shell->term.line));
	if (shell->term.line != NULL)
		ft_memset(shell->term.line, 0, shell->term.size);
	shell->term.prompt = prompt;
	if (shell->is_atty)
		show_prompt(shell);
	shell->term.cursor = 0;
	shell->term.size = 0;
	shell->term.row = 0;
	shell->term.col = shell->term.offset % shell->term.win_cols;
	shell->term.rows = 1;
	shell->term.esc_seq = 0;
	shell->term.seq_off = 0;
}

int		realloc_cmdline(t_term *term)
{
	char	*tmp;

	tmp = (char *)ft_memalloc(term->capacity + CMDLINE_DEF_CAPACITY + 1);
	if (tmp == NULL)
		return (0);
	if (term->line != NULL)
	{
		ft_memcpy(tmp, term->line, term->size);
		free(term->line);
	}
	term->line = tmp;
	term->capacity += CMDLINE_DEF_CAPACITY;
	return (1);
}

void	insert_cmdline(t_shell *shell, t_term *term, char key)
{
	(void)shell;
	if (term->size >= term->capacity && !realloc_cmdline(term))
		return ;
	if (term->size > term->cursor)
	{
		ft_memmove(term->line + term->cursor + 1
				, term->line + term->cursor, term->size - term->cursor);
	}
	(term->size)++;
	(term->line)[term->size] = '\0';
	(term->line)[term->cursor] = key;
	(term->cursor)++;
	go_to_prompt(term);
	print_input(shell, term);
}

void	go_to_prompt(t_term *term)
{
	int	prompt_height;

	tputs(tgetstr("cr", NULL), 1, t_putchar);
	prompt_height = term->offset / term->win_cols;
	if (term->row + prompt_height > 0)
		tputs(tparm(tgetstr("UP", NULL), term->row + prompt_height), 1
				, t_putchar);
	tputs(tgetstr("cd", NULL), 1, t_putchar);
}

void	print_input(t_shell *shell, t_term *term)
{
	size_t	row;

	show_prompt(shell);
	update_pos_data(term);
	if (term->visual_mode)
		print_select_line(term);
	else
		write(STDERR_FILENO, term->line, term->size);
	ft_putchar_fd(' ', 2);
	row = term->rows - (term->row + 1);
	if (row > 0)
		tputs(tparm(tgetstr("UP", NULL), row), 1, t_putchar);
	move_cursor_to_col(term);
}
