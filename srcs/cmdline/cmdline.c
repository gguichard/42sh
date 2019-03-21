/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 15:22:21 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 19:45:19 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <term.h>
#include "parser_lexer.h"
#include "split_cmd_token.h"
#include "str_cmd_inf.h"
#include "cmdline.h"

static char	*join_command(t_cmdline *cmdline, char *full_input, t_prompt type)
{
	char	*new_line;
	char	*tmp[3];

	new_line = wstr_to_mbstr(cmdline->input.buffer, cmdline->input.size);
	if (new_line != NULL && full_input != NULL)
	{
		tmp[0] = full_input;
		tmp[1] = new_line;
		tmp[2] = NULL;
		new_line = ft_join(tmp, type == PROMPT_OPERATOR ? " " : "\n");
		free(tmp[1]);
	}
	free(full_input);
	return (new_line);
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
		else if (scmd_cur_char_is_escaped(scmd_inf))
			return (PROMPT_BACKSLASH);
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

char		*read_cmdline(t_alloc *alloc, t_cmdline *cmdline)
{
	char			*full_input;
	t_str_cmd_inf	scmd_inf;
	t_list			*tokens;
	t_prompt		type;
	t_recall_prompt	ret;
	int				input_ret;

	(void)alloc;
	full_input = NULL;
	type = PROMPT_DEFAULT;
	ret = PR_SUCCESS;
	while ((full_input == NULL || type != PROMPT_DEFAULT)
			&& (input_ret = read_input(cmdline, get_prompt(cmdline, type))))
	{
		full_input = join_command(cmdline, full_input, type);
		if (full_input == NULL)
			break ;
		scmd_init(&scmd_inf, full_input);
		tokens = split_cmd_token(&scmd_inf);
		if (tokens != NULL)
		{
			ret = token_analyser(tokens);
			type = change_prompt_type(&scmd_inf, ret);
		}
		scmd_delete(scmd_inf.sub_str_cmd);
		if (tokens == NULL || ret == PR_ERROR)
			ft_strdel(&full_input);
	}
	if (input_ret == 0 && full_input == NULL)
	{
		reset_term(cmdline);
		ft_printf("exit\n");
		exit(0); // TODO: exit with alloc ret value
	}
	else if (input_ret == 0)
	{
		ft_dprintf(STDERR_FILENO
			, "42sh: syntax error: unexpected end of file\n");
		ft_strdel(&full_input);
	}
	return (full_input);
}
