/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:48:21 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/08 21:02:37 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

static int		is_special_char(const char *s, int i)
{
	if (s[i] == '_' || s[i] == '?' || s[i] == '!' || s[i] == '$')
		return (1);
	if ((s[i] == '\'' && s[i + 1] == '\'') || (s[i] == '"' && s[i + 1] == '"'))
		return (1);
	return (0);
}

int				expand_var(char **str, t_alloc *alloc, const char *exp
				, size_t *len)
{
	if (ft_strncmp(exp, "${", 2) == 0)
	{
		if (!check_expand_syntax(exp + 2))
			return (error_expand(exp));
		*str = get_expand_value(exp + 2, 1, alloc, len);
	}
	else
		*str = get_expand_value(exp + 1, 0, alloc, len);
	return (1);
}

static size_t	subcmd_length(t_str_cmd_inf *scmd_inf)
{
	size_t			len;
	int				was_in_subcmd;

	len = 0;
	while (scmd_cur_char(scmd_inf))
	{
		was_in_subcmd = (scmd_inf->sub_str_cmd != NULL
			&& scmd_inf->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_SUBCMD);
		len += scmd_move_to_next_char(scmd_inf);
		if (was_in_subcmd
				&& (scmd_inf->sub_str_cmd == NULL
					|| scmd_inf->sub_str_cmd->cur_str_cmd_type
					!= SCMD_TYPE_SUBCMD))
			break ;
	}
	return (len);
}

static int		subcmd_expand(t_alloc *alloc, char **input, size_t *pos)
{
	t_str_cmd_inf	scmd_inf;
	size_t			len;
	char			*value;
	char			*cmd;

	if (!scmd_init(&scmd_inf, *input + *pos))
		return (0);
	len = subcmd_length(&scmd_inf);
	scmd_clean(&scmd_inf);
	cmd = ft_strsub(*input, *pos + 2, len - 3);
	value = subcmd_exec(alloc, cmd);
	ft_strdel(&cmd);
	ft_strreplace_inside(input, *pos, len, (value == NULL ? "" : value));
	if (value != NULL)
		*pos += ft_strlen(value);
	ft_strdel(&value);
	return ((g_sig == SIGINT) ? 0 : 1);
}

int				expand(char **input, t_alloc *alloc, size_t *pos)
{
	char		*str;
	size_t		len;

	str = NULL;
	if ((*input)[*pos + 1] == '(')
		return (subcmd_expand(alloc, input, pos));
	else if (!expand_var(&str, alloc, *input + *pos, &len))
		return (0);
	if (str && (*input)[*pos + 1] == '{')
		ft_strreplace_inside(input, *pos, len + 3, str);
	else if (str && !ft_isalnum((*input)[*pos + 1])
			&& is_special_char(*input + *pos, 1) == 0)
	{
		free(str);
		*pos += 1;
		return (1);
	}
	else if (str)
		ft_strreplace_inside(input, *pos, len + 1, str);
	if (str)
	{
		*pos += ft_strlen(str);
		free(str);
	}
	return (1);
}
