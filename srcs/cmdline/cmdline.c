/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 15:22:21 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/06 13:34:48 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "parser_lexer.h"
#include "split_cmd_token.h"
#include "str_cmd_inf.h"
#include "builtins.h"
#include "cmdline.h"
#include "error.h"

static char		*join_command(t_cmdline *cmdline, char *full_input
		, char *new_line)
{
	char	*tmp[3];

	if (new_line == NULL)
		return (full_input);
	else if (!expand_history_events(&cmdline->history, &new_line))
		ft_strdel(&full_input);
	else
	{
		if (full_input == NULL)
			return (new_line);
		else
		{
			tmp[0] = full_input;
			tmp[1] = new_line;
			tmp[2] = NULL;
			full_input = ft_join(tmp, "\n");
			free(tmp[0]);
		}
	}
	free(new_line);
	return (full_input);
}

static t_error	change_prompt_type(t_str_cmd_inf *scmd_inf, t_recall_prompt ret
		, t_prompt *type)
{
	if (ret == PR_PIPE || ret == PR_AND || ret == PR_OR)
		*type = PROMPT_OPERATOR;
	else
	{
		while (scmd_inf->sub_str_cmd != NULL)
			scmd_inf = scmd_inf->sub_str_cmd;
		if (scmd_inf->is_in_quote)
			*type = PROMPT_QUOTE;
		else if (scmd_inf->is_in_dbquote)
			*type = PROMPT_DQUOTE;
		else if (scmd_inf->cur_str_cmd_type == SCMD_TYPE_VAR)
			*type = PROMPT_BRACKET;
		else if (scmd_inf->cur_str_cmd_type == SCMD_TYPE_SUBCMD)
			*type = PROMPT_SUBCMD;
		else if (scmd_cur_char_is_escaped(scmd_inf))
			*type = PROMPT_BACKSLASH;
		else
		{
			*type = PROMPT_DEFAULT;
			return (ERRC_NOERROR);
		}
	}
	return (ERRC_INCOMPLETECMD);
}

static t_error	analyse_full_input(t_alloc *alloc, t_prompt *type)
{
	t_error			error;
	t_str_cmd_inf	scmd_inf;
	t_list			*tokens;
	t_recall_prompt	analyser_ret;

	if (!scmd_init(&scmd_inf, alloc->full_input))
		return (ERRC_UNEXPECTED);
	tokens = split_cmd_token(&scmd_inf, alloc->aliastable);
	if (tokens == NULL)
		error = ERRC_UNEXPECTED;
	else
	{
		analyser_ret = token_analyser(tokens, 0);
		if (analyser_ret == PR_ERROR)
			error = ERRC_LEXERROR;
		else
			error = change_prompt_type(&scmd_inf, analyser_ret, type);
	}
	ft_lstdel(&tokens, del_token);
	scmd_clean(&scmd_inf);
	return (error);
}

static t_error	read_complete_command(t_alloc *alloc, t_cmdline *cmdline
		, t_rstate *state)
{
	t_error		error;
	t_prompt	type;
	char		*new_line;

	error = ERRC_INCOMPLETECMD;
	type = PROMPT_DEFAULT;
	while (error == ERRC_INCOMPLETECMD)
	{
		new_line = create_prompt_and_read_input(cmdline, type, state);
		if (*state != RSTATE_END)
		{
			free(new_line);
			break ;
		}
		alloc->full_input = join_command(cmdline, alloc->full_input, new_line);
		if (alloc->full_input == NULL)
			error = ERRC_UNEXPECTED;
		else
			error = analyse_full_input(alloc, &type);
	}
	return (error);
}

char			*read_cmdline(t_alloc *alloc, t_cmdline *cmdline)
{
	t_rstate	state;
	t_error		error;

	state = RSTATE_END;
	error = read_complete_command(alloc, cmdline, &state);
	if (alloc->full_input != NULL)
		push_history_entry(&cmdline->history, alloc->full_input);
	if (state == RSTATE_ETX)
		alloc->ret_val = 1;
	else if (state == RSTATE_EOT)
	{
		if (alloc->full_input == NULL)
			builtin_exit(NULL, alloc);
		else
			ft_dprintf(2, "42sh: syntax error: unexpected end of file\n");
	}
	cmdline->history.offset = NULL;
	if (error != ERRC_NOERROR)
	{
		ft_strdel(&alloc->full_input);
		if (error == ERRC_LEXERROR
				|| (error == ERRC_INCOMPLETECMD && state == RSTATE_EOT))
			alloc->ret_val = 2;
	}
	return (alloc->full_input);
}
