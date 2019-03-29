/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:42:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/28 16:22:37 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "shell.h"
#include "autocomplete.h"
#include "str_cmd_inf.h"
#include "cmdline.h"

static void	ac_append_to_cmdline(t_cmdline *cmdline, t_ac_suff_inf *acs_inf
		, t_str_cmd_inf *scmd, int at_end_of_line)
{
	char	*suff;

	suff = acs_inf->suff;
	while (*suff != '\0')
	{
		add_char_to_input(cmdline, *suff);
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
