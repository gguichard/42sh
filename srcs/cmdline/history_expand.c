/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 11:13:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/31 16:00:01 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "cmdline.h"
#include "history.h"
#include "split_cmd_token.h"

static int	is_char_escaped(t_hist_expand_inf *exp_inf)
{
	int		backslash_count;
	size_t	offset;

	backslash_count = 0;
	offset = exp_inf->offset;
	while (offset > 0)
	{
		offset -= 1;
		if ((*exp_inf->input)[offset] != '\\')
			break ;
		backslash_count++;
	}
	return (backslash_count % 2 == 1);
}

static int	expand_event(t_history *history, t_hist_expand_inf *exp_inf)
{
	size_t	len;
	int		ret;
	char	*event;

	len = 1;
	while ((*exp_inf->input)[exp_inf->offset + len] != '\0'
			&& !ft_strchr(WORD_SEP_CHARS
				, (*exp_inf->input)[exp_inf->offset + len])
			&& (!exp_inf->is_in_dbquote
				|| (*exp_inf->input)[exp_inf->offset + len] != '\"'))
		len++;
	ret = 1;
	if (len > 1)
	{
		event = ft_strsub(*exp_inf->input, exp_inf->offset, len);
		if (event != NULL && replace_event(history, exp_inf, event))
			exp_inf->has_been_expanded = 1;
		else
		{
			ret = 0;
			ft_dprintf(STDERR_FILENO, "42sh: %s: event not found\n", event);
		}
		free(event);
	}
	return (ret);
}

int			expand_history_events(t_history *history, char **input)
{
	t_hist_expand_inf	exp_inf;

	ft_memset(&exp_inf, 0, sizeof(t_hist_expand_inf));
	exp_inf.input = input;
	while ((*exp_inf.input)[exp_inf.offset] != '\0')
	{
		if ((*exp_inf.input)[exp_inf.offset] == '\"')
			exp_inf.is_in_dbquote = !exp_inf.is_in_dbquote;
		else if ((*exp_inf.input)[exp_inf.offset] == '\''
				&& !is_char_escaped(&exp_inf))
			exp_inf.is_in_quote = !exp_inf.is_in_quote;
		else if ((*exp_inf.input)[exp_inf.offset] == '!'
				&& !exp_inf.is_in_quote && !is_char_escaped(&exp_inf))
		{
			if (!expand_event(history, &exp_inf))
				return (0);
		}
		exp_inf.offset++;
	}
	if (exp_inf.has_been_expanded)
		ft_printf("%s\n", *exp_inf.input);
	return (1);
}
