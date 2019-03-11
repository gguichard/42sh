/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 00:58:42 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "input.h"
#include "error.h"
#include "history.h"
#include "expand_vars.h"
#include "apply_escape.h"
#include "lexer.h"
#include "execute.h"
#include "split_cmd_token.h"

static char	*read_input_chunk(t_shell *shell, t_error *error)
{
	char			*input;
	t_list			*token_lst;
	t_str_cmd_inf	scmd_inf;

	input = get_command_line(&(shell->term));
	ft_strdel(&(shell->term.prev_lines));
	if (input == NULL && (*error = ERRC_UNEXPECTED) == ERRC_UNEXPECTED)
		return (NULL);
	scmd_init(&scmd_inf, input);
	*error = ERRC_NOERROR;
	if ((token_lst = split_cmd_token(&scmd_inf)) != NULL)
	{
		if (!lex_commands(token_lst))
			*error = ERRC_LEXERROR;
		else if (!is_command_complete(&scmd_inf, token_lst))
		{
			*error = ERRC_INCOMPLETECMD;
			shell->term.prev_lines = input;
			reset_cmdline(shell, get_prompt_type(&scmd_inf));
		}
		ft_lstdel(&token_lst, del_token);
	}
	scmd_delete(scmd_inf.sub_var_bracket);
	return (input);
}

static char	*get_full_input(t_shell *shell, t_error *error)
{
	int		ret;
	char	*input;

	input = NULL;
	while ((ret = read_input(shell)) == 1)
	{
		input = read_input_chunk(shell, error);
		if (*error != ERRC_INCOMPLETECMD)
			break ;
	}
	shell->term.prev_lines = NULL;
	if (ret < 0)
	{
		*error = ERRC_UNEXPECTED;
		ft_strdel(&input);
	}
	return (input);
}

static int	expand_line_with_execute(t_shell *shell, const char *line)
{
	char			*var_error;
	char			*expand_line;
	t_str_cmd_inf	scmd_inf;
	t_list			*token_lst;

	var_error = NULL;
	expand_line = expand_vars(line, shell, &var_error);
	if (expand_line == NULL && var_error == NULL)
		return (0);
	else if (var_error != NULL)
	{
		ft_dprintf(2, "%s: %s: Bad substitution\n", ERR_PREFIX, var_error);
		free(var_error);
	}
	else
	{
		scmd_init(&scmd_inf, expand_line);
		token_lst = split_cmd_token(&scmd_inf);
		if (token_lst != NULL)
			execute_all(shell, token_lst);
		ft_lstdel(&token_lst, del_token);
		scmd_delete(scmd_inf.sub_var_bracket);
		free(expand_line);
	}
	return (1);
}

static int	handle_input(t_shell *shell, char *input, t_error error)
{
	char			*line;

	line = apply_only_newline_escape(input);
	free(input);
	if (line == NULL)
		return (-1);
	if (error == ERRC_INCOMPLETECMD)
		ft_dprintf(2, "%s: Unexpected end of file\n", ERR_PREFIX);
	else if (error == ERRC_NOERROR)
	{
		if (!expand_line_with_execute(shell, line))
		{
			free(line);
			return (0);
		}
	}
	if (ft_strlen(line) > 0)
		add_history_entry(shell, line);
	free(line);
	return (1);
}

int			wait_for_command(t_shell *shell)
{
	int		ret;
	t_error	error;
	char	*input;

	if (!realloc_cmdline(&(shell->term)))
		return (0);
	ret = 1;
	while (ret > 0)
	{
		reset_cmdline(shell, PROMPT_DEF);
		setup_term(shell);
		error = ERRC_NOERROR;
		input = get_full_input(shell, &error);
		reset_term(shell);
		ret = (input == NULL) ? 0 : handle_input(shell, input, error);
	}
	if (shell->is_atty)
		ft_putendl_fd("exit", 2);
	return (ret);
}
