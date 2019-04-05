/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 14:51:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 17:20:46 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "cmdline.h"
#include "inhibitor.h"
#include "expand.h"

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
	char		*heredoc;
	char		*new_line;
	t_rstate	state;

	heredoc = ft_strdup("");
	if (heredoc != NULL)
	{
		setup_term(cmdline);
		while (1)
		{
			new_line = create_prompt_and_read_input(cmdline, PROMPT_HEREDOC
					, &state);
			if (state == RSTATE_ETX)
				continue ;
			if (state != RSTATE_END || ft_strequ(new_line, word))
				break ;
			heredoc = join_heredoc(heredoc, new_line);
			free(new_line);
			if (heredoc == NULL)
				break ;
		}
		reset_term(cmdline);
	}
	return (heredoc);
}

static void	expand_heredoc(t_alloc *alloc, char **heredoc)
{
	size_t	offset;
	char	*tmp;

	offset = 0;
	while ((tmp = ft_strchr(*heredoc + offset, '$')) != NULL)
	{
		offset = (size_t)(tmp - *heredoc);
		expand(heredoc, alloc, &offset);
	}
}

char		*prompt_heredoc(t_cmdline *cmdline, const char *redir_word)
{
	char	*word;
	char	*heredoc;
	size_t	len;

	word = ft_strdup(redir_word);
	if (word == NULL)
		return (NULL);
	if (!inhib_only_str(word))
	{
		free(word);
		return (NULL);
	}
	heredoc = read_heredoc(cmdline, word);
	if (heredoc != NULL && heredoc[0] != '\0')
	{
		len = ft_strlen(heredoc);
		ft_memcpy(heredoc, heredoc + 1, len - 1);
		heredoc[len - 1] = '\n';
		if (ft_strequ(redir_word, word))
			expand_heredoc(cmdline->alloc, &heredoc);
	}
	free(word);
	return (heredoc);
}
