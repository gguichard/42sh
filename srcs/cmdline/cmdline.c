/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 15:22:21 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 17:22:56 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <signal.h>
#include <term.h>
#include "parser_lexer.h"
#include "split_cmd_token.h"
#include "str_cmd_inf.h"
#include "cmdline.h"

static char	*join_command(t_cmdline *cmdline, char *full_input)
{
	char	*new_line;

	new_line = wstr_to_mbstr(cmdline->input.buffer, cmdline->input.size);
	if (full_input == NULL)
		return (new_line);
	else
		return (ft_strjoin_free(full_input, new_line));
}

static int	change_prompt_type(t_str_cmd_inf *scmd_inf, t_recall_prompt ret)
{
	if (ret == PR_PIPE || ret == PR_AND || ret == PR_OR)
		return (PROMPT_OPERATOR);
	else
	{
		while (scmd_inf->sub_str_cmd != NULL)
			scmd_inf = scmd_inf->sub_str_cmd;
		if (scmd_inf->is_in_quote)
			return (PROMPT_QUOTE);
		else if (scmd_inf->is_in_dbquote)
			return (PROMPT_DQUOTE);
		else if (scmd_inf->cur_str_cmd_type == SCMD_TYPE_VAR)
			return (PROMPT_BRACKET);
		else if (scmd_inf->cur_str_cmd_type == SCMD_TYPE_SUBCMD)
			return (PROMPT_SUBCMD);
	}
	return (PROMPT_DEFAULT);
}

int			init_cmdline(t_cmdline *cmdline)
{
	g_cmdline = cmdline;
	if (tgetent(NULL, "xterm-256color") == -1) // TODO: utiliser la variable TERM
		return (0);
	update_winsize(cmdline);
	signal(SIGWINCH, handle_sigwinch);
	return (1);
}

char		*read_cmdline(t_cmdline *cmdline)
{
	t_prompt		type;
	char			*full_input;
	t_str_cmd_inf	scmd_inf;
	t_list			*tokens;
	t_recall_prompt	ret;

	type = PROMPT_DEFAULT;
	full_input = NULL;
	while (read_input(cmdline, get_prompt(cmdline, type)))
	{
		full_input = join_command(cmdline, full_input);
		scmd_init(&scmd_inf, full_input);
		tokens = split_cmd_token(&scmd_inf);
		if (tokens != NULL)
		{
			ret = token_analyser(tokens);
			type = change_prompt_type(&scmd_inf, ret);
		}
		scmd_delete(scmd_inf.sub_str_cmd);
		if (tokens == NULL || ret == PR_ERROR)
		{
			ft_strdel(&full_input);
			break ;
		}
		if (type == PROMPT_DEFAULT)
			break ;
	}
	return (full_input);
}
