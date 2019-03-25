/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 15:22:21 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/25 15:54:13 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <term.h>
#include "shell.h"
#include "vars.h"
#include "parser_lexer.h"
#include "split_cmd_token.h"
#include "str_cmd_inf.h"
#include "cmdline.h"

static char	*join_command(t_cmdline *cmdline, char *full_input, t_prompt type)
{
	char	*new_line;
	char	*tmp[3];

	if (cmdline->input.buffer[0] == '\0')
		return (full_input);
	new_line = ft_strdup(cmdline->input.buffer);
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

int			init_cmdline(t_alloc *alloc, t_cmdline *cmdline)
{
	const char	*term;

	g_cmdline = cmdline;
	term = get_var_value(alloc->vars, "TERM");
	if (term == NULL || term[0] == '\0')
		term = "xterm-256color";
	if (tgetent(NULL, term) == -1)
		return (0);
	ft_memset(cmdline->input.buffer, 0, sizeof(cmdline->input.buffer));
	cmdline->input.capacity = sizeof(cmdline->input.buffer) - 1;
	update_winsize(cmdline);
	signal(SIGINT, handle_sig);
	signal(SIGWINCH, handle_sigwinch);
	return (1);
}

static char	*read_full_input(t_cmdline *cmdline, int *ret)
{
	char			*full_input;
	t_prompt		type;
	t_recall_prompt	analyser_ret;
	t_str_cmd_inf	scmd_inf;
	t_list			*tokens;

	full_input = NULL;
	type = PROMPT_DEFAULT;
	while ((full_input == NULL || type != PROMPT_DEFAULT)
			&& (*ret = read_input(cmdline, get_prompt(cmdline, type))) == 1)
	{
		if ((full_input = join_command(cmdline, full_input, type)) == NULL)
			break ;
		if (!scmd_init(&scmd_inf, full_input))
			return (ft_memdel((void **)&full_input));
		if ((tokens = split_cmd_token(&scmd_inf)) == NULL)
			ft_strdel(&full_input);
		else if ((analyser_ret = token_analyser(tokens)) == PR_ERROR)
			ft_strdel(&full_input);
		else
			type = change_prompt_type(&scmd_inf, analyser_ret);
		ft_lstdel(&tokens, del_token);
		scmd_clean(&scmd_inf);
	}
	return (full_input);
}

char		*read_cmdline(t_alloc *alloc, t_cmdline *cmdline)
{
	int		ret;
	char	*full_input;

	(void)alloc;
	ret = 1;
	full_input = read_full_input(cmdline, &ret);
	if (ret)
	{
		push_history_entry(&cmdline->history, full_input);
		cmdline->history.offset = NULL;
	}
	else
	{
		if (full_input == NULL)
		{
			reset_term(cmdline);
			save_history_entries(alloc, &cmdline->history);
			ft_printf("exit\n");
			exit(0); // TODO: exit with alloc ret value
		}
		ft_dprintf(STDERR_FILENO
				, "42sh: syntax error: unexpected end of file\n");
		ft_strdel(&full_input);
	}
	return (full_input);
}
