/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 11:13:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/26 17:12:34 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "cmdline.h"
#include "history.h"
#include "split_cmd_token.h"

static int	is_char_escaped(const char *str, size_t offset)
{
	int	backslash_count;

	backslash_count = 0;
	while (offset > 0)
	{
		offset -= 1;
		if (str[offset] != '\\')
			break ;
		backslash_count++;
	}
	return (backslash_count % 2 == 1);
}

static int	expand_event(t_history *history, char **input, size_t *offset)
{
	int		ret;
	size_t	len;
	char	*event;

	ret = 1;
	len = 1;
	while ((*input)[*offset + len] != '\0'
			&& !ft_strchr(WORD_SEP_CHARS, (*input)[*offset + len]))
		len++;
	if (len > 1)
	{
		event = ft_strsub(*input, *offset, len);
		if (event == NULL)
			return (0);
		if (!replace_event(history, input, offset, event))
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
	size_t	offset;
	int		is_in_quote;
	int		has_been_expanded;

	offset = 0;
	is_in_quote = 0;
	has_been_expanded = 0;
	while ((*input)[offset] != '\0')
	{
		if ((*input)[offset] == '\'' && !is_char_escaped(*input, offset))
			is_in_quote = !is_in_quote;
		if ((*input)[offset] == '!' && !is_in_quote
				&& !is_char_escaped(*input, offset))
		{
			if (!expand_event(history, input, &offset))
				return (0);
			has_been_expanded = 1;
		}
		offset++;
	}
	if (has_been_expanded)
		ft_printf("%s\n", *input);
	return (1);
}
