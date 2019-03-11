/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 10:16:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 13:47:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "input.h"
#include "history.h"
#include "utils.h"

static int	history_copy(t_shell *shell, t_term *term, const char *line)
{
	size_t	len;
	char	*dup;

	if (line == NULL || (dup = ft_strdup(line)) == NULL)
		return (0);
	ft_strdel(&(term->line));
	len = ft_strlen(dup);
	go_to_prompt(term);
	term->line = dup;
	term->capacity = len;
	term->size = len;
	term->cursor = len;
	print_input(shell, term);
	return (1);
}

int			history_up(t_shell *shell, t_term *term)
{
	const char	*line;

	if (term->visual_mode)
		return (0);
	line = peek_history_prev(shell);
	if (line != NULL && term->def_line == NULL)
		term->def_line = ft_strdup(term->line);
	return (history_copy(shell, term, line));
}

int			history_down(t_shell *shell, t_term *term)
{
	const char	*line;
	int			should_free;
	int			ret;

	if (term->visual_mode)
		return (0);
	line = peek_history_next(shell);
	should_free = 0;
	if (line == NULL && term->def_line != NULL)
	{
		line = term->def_line;
		term->def_line = NULL;
		should_free = 1;
	}
	ret = history_copy(shell, term, line);
	if (should_free)
		free((char *)line);
	return (ret);
}

int			history_search(t_shell *shell, t_term *term)
{
	t_history	*elem;

	if (term->def_line == NULL)
		term->def_line = ft_strdup(term->line);
	if (term->def_line == NULL)
		return (0);
	elem = shell->history_off;
	if (elem != NULL)
		elem = elem->prev;
	if (elem == NULL)
		elem = shell->history;
	while (elem != NULL)
	{
		if (ft_strncmp(elem->content, term->def_line
					, ft_strlen(term->def_line)) == 0)
			break ;
		elem = elem->prev;
	}
	shell->history_off = elem;
	if (elem == NULL)
		return (0);
	history_copy(shell, term, elem->content);
	return (1);
}
