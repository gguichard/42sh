/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 14:51:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 14:29:20 by jocohen          ###   ########.fr       */
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

static char	*read_heredoc(t_cmdline *cmdline, const char *word)
{
	char		*prompt;
	size_t		offset;
	char		*heredoc;
	t_rstate	state;
	int			i = 0;

	if ((prompt = get_prompt(cmdline, PROMPT_HEREDOC, &offset)) == NULL)
		return (NULL);
	heredoc = ft_strdup("");
	if (heredoc != NULL)
	{
		setup_term(cmdline);
		while (i < 100)
		{
			i += 1;
			state = create_prompt_and_read_input(cmdline, PROMPT_HEREDOC);
			if (state == RSTATE_ETX)
				continue ;
			if (state != RSTATE_END || ft_strequ(cmdline->input.buffer, word))
				break ;
			heredoc = join_heredoc(heredoc, cmdline->input.buffer);
			if (heredoc == NULL)
				break ;
		}
		reset_term(cmdline);
	}
	free(prompt);
	return (heredoc);
}

char		*prompt_heredoc(t_cmdline *cmdline, const char *word)
{
	char	*heredoc;
	size_t	len;

	heredoc = read_heredoc(cmdline, word);
	if (heredoc != NULL && heredoc[0] != '\0')
	{
		len = ft_strlen(heredoc);
		ft_memcpy(heredoc, heredoc + 1, len - 1);
		heredoc[len - 1] = '\n';
	}
	return (heredoc);
}
