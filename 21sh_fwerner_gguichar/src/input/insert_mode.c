/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:28:03 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 10:21:40 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str_cmd_inf.h"
#include "shell.h"
#include "input.h"
#include "utils.h"
#include "history.h"

static void	ac_append(t_shell *shell, t_ac_suff_inf *result
		, t_str_cmd_inf *scmd, int at_end_of_line)
{
	char	*curr;

	curr = result->suff;
	while (*curr != '\0')
	{
		add_char_and_escape_if_needed(*curr
				, (curr == result->suff && scmd_cur_char_is_escaped(scmd))
				, shell, scmd);
		++curr;
	}
	if (result->suff_type == ACS_TYPE_VAR_IN_BRACKETS)
		insert_cmdline(shell, &(shell->term), '}');
	if (at_end_of_line && (result->suff_type == ACS_TYPE_FILE
				|| result->suff_type == ACS_TYPE_VAR_IN_BRACKETS))
	{
		if (scmd->is_in_quote)
			insert_cmdline(shell, &(shell->term), '\'');
		else if (scmd->is_in_doublequote)
			insert_cmdline(shell, &(shell->term), '\"');
		insert_cmdline(shell, &(shell->term), ' ');
	}
	else if (result->suff_type == ACS_TYPE_DIR)
		insert_cmdline(shell, &(shell->term), '/');
}

int			handle_ac(t_shell *shell, t_term *term)
{
	t_ac_suff_inf	*result;
	t_str_cmd_inf	scmd;
	char			*real_line;
	int				at_eol;

	if ((real_line = init_scmd_with_realline(&scmd, term, &at_eol)) == NULL)
		return (0);
	result = autocomplete_cmdline(&scmd, shell);
	if (result == NULL || result->suff == NULL || result->choices == NULL)
	{
		scmd_delete(scmd.sub_var_bracket);
		free(real_line);
		return (!!delete_ac_suff_inf(result));
	}
	ac_append(shell, result, &scmd, at_eol);
	if ((term->ac_flag)++)
	{
		tputs(tparm(tgetstr("do", NULL), term->rows - term->row), 1, t_putchar);
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		ac_print_list(result->choices, term);
		print_input(shell, term);
	}
	scmd_delete(scmd.sub_var_bracket);
	free(real_line);
	return (delete_ac_suff_inf(result) == NULL);
}

int			handle_eot_key(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->size == 0)
		return (1);
	tputs(tgetstr("bl", NULL), 1, t_putchar);
	return (0);
}

int			handle_key(t_shell *shell, t_term *term, char key)
{
	int	ret;
	int	ac;

	ret = 1;
	ac = term->ac_flag;
	if (key >= 32 && key < 127)
		insert_cmdline(shell, term, key);
	else if (key == EOT_KEY)
		return (handle_eot_key(shell, term) ? -1 : 1);
	else if (key == BACKSPACE_KEY || key == DELETE_KEY)
		ret = handle_bs_key(shell, term);
	else if (key == CTRL_R_KEY)
		ret = history_search(shell, term);
	else if (key == '\t')
		ret = handle_ac(shell, term);
	else if (key == '\n')
	{
		move_cursor_end(shell, term);
		ft_putchar_fd('\n', 2);
		return (0);
	}
	if (!ret)
		tputs(tgetstr("bl", NULL), 1, t_putchar);
	term->ac_flag = (ac && ac == term->ac_flag) ? 0 : term->ac_flag;
	return (1);
}
