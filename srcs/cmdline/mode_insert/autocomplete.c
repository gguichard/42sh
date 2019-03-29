/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:42:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 12:31:45 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "shell.h"
#include "autocomplete.h"
#include "str_cmd_inf.h"
#include "cmdline.h"

static void	add_spestr_to_input(t_cmdline *cmdline, const char *spe_str
		, char replace)
{
	while (*spe_str != '\0')
	{
		if (*spe_str == '@')
			add_char_to_input(cmdline, replace);
		else
			add_char_to_input(cmdline, *spe_str);
		++spe_str;
	}
}

static void	add_char_and_esc_if_needed(t_cmdline *cmdline, char char_to_add
		, t_str_cmd_inf *scmd)
{
	if (char_to_add == '\0')
		return ;
	if ((scmd_cur_char_is_in_nothing(scmd)
				&& ft_strchr(" \t|<>&;$\'\"\\=", char_to_add) != NULL)
			|| (scmd->is_in_dbquote && ft_strchr("$\"\\", char_to_add) != NULL))
		add_spestr_to_input(cmdline, "\\@", char_to_add);
	else if (scmd_cur_char_is_in_nothing(scmd) && char_to_add == '\n')
		add_spestr_to_input(cmdline, "\'@\'", char_to_add);
	else if (scmd->is_in_quote && char_to_add == '\'')
		add_spestr_to_input(cmdline, "\'\\@\'", char_to_add);
	else
		add_char_to_input(cmdline, char_to_add);
}

static void	ac_append_to_cmdline(t_cmdline *cmdline, t_ac_suff_inf *acs_inf
		, t_str_cmd_inf *scmd, int at_end_of_line)
{
	char	*suff;

	if (scmd->pos > 0 && scmd->str[scmd->pos - 1] == '\\'
			&& !scmd_char_at_is_escaped(scmd, scmd->pos - 1))
		handle_backspace_key(cmdline);
	suff = acs_inf->suff;
	while (*suff != '\0')
	{
		add_char_and_esc_if_needed(cmdline, *suff, scmd);
		suff++;
	}
	if (acs_inf->suff_type == ACS_TYPE_VAR_IN_BRACKETS)
		add_char_to_input(cmdline, '}');
	if (at_end_of_line && (acs_inf->suff_type == ACS_TYPE_FILE
				|| acs_inf->suff_type == ACS_TYPE_VAR_IN_BRACKETS))
	{
		if (scmd->is_in_quote)
			add_char_to_input(cmdline, '\'');
		else if (scmd->is_in_dbquote)
			add_char_to_input(cmdline, '\"');
		add_char_to_input(cmdline, ' ');
	}
	else if (acs_inf->suff_type == ACS_TYPE_DIR)
		add_char_to_input(cmdline, '/');
}

int			handle_autocomplete(t_cmdline *cmdline)
{
	char			*old_char_pos;
	char			old_char;
	t_ac_suff_inf	*acs_inf;
	int				ret;
	t_str_cmd_inf	scmd;

	old_char_pos = cmdline->input.buffer + cmdline->input.offset;
	old_char = *old_char_pos;
	*old_char_pos = '\0';
	if (!scmd_init(&scmd, cmdline->input.buffer))
	{
		*old_char_pos = old_char;
		return (0);
	}
	acs_inf = autocomplete_cmdline(&scmd, cmdline->alloc);
	*old_char_pos = old_char;
	if (acs_inf == NULL)
	{
		scmd_clean(&scmd);
		return (0);
	}
	ret = 1;
	if (acs_inf->choices == NULL)
		ret = 0;
	if (acs_inf->suff != NULL)
		ac_append_to_cmdline(cmdline, acs_inf, &scmd, cmdline->input.offset == cmdline->input.size);
	if (acs_inf->choices != NULL && cmdline->ac_flag)
	{
		write(STDOUT_FILENO, "\n", 1);
		ac_print_list(acs_inf->choices, cmdline);
		print_prompt_and_cmdline(cmdline);
	}
	cmdline->ac_flag += 1;
	delete_ac_suff_inf(acs_inf);
	scmd_clean(&scmd);
	return (ret);
}
