/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 14:51:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 15:49:42 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "cmdline.h"

static char	*join_heredoc(char *heredoc, char *part)
{
	char	*tab[3];
	char	*ret;

	tab[0] = heredoc;
	tab[1] = part;
	tab[2] = NULL;
	ret = ft_join(tab, "\n");
	free(heredoc);
	return (ret);
}

char		*read_heredoc(t_cmdline *cmdline, const char *word)
{
	char		*heredoc;
	t_rstate	state;
	size_t		len;

	heredoc = ft_strdup("");
	if (heredoc == NULL)
		return (heredoc);
	setup_term(cmdline);
	while (1)
	{
		state = read_input(cmdline, get_prompt(cmdline, PROMPT_HEREDOC));
		if (state != RSTATE_END)
			break ;
		if (ft_strequ(cmdline->input.buffer, word))
			break ;
		heredoc = join_heredoc(heredoc, cmdline->input.buffer);
		if (heredoc == NULL)
			break ;
	}
	reset_term(cmdline);
	if (heredoc != NULL && heredoc[0] != '\0')
	{
		len = ft_strlen(heredoc);
		ft_memcpy(heredoc, heredoc + 1, len - 1);
		heredoc[len - 1] = '\n';
	}
	return (heredoc);
}
