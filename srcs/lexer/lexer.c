/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 17:54:50 by tcollard          #+#    #+#             */
/*   Updated: 2019/03/01 23:32:46 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void		read_lexer(char **lexer, t_env **lst_env, t_ast *lst,
	t_alloc **alloc)
{
	int	i;
	int	x;

	i = 0;
	while (lexer && lexer[i])
	{
		x = 0;
		while (lexer[i][x] && ft_isspace(lexer[i][x]))
			x += 1;
		(lexer[i][x]) ? clean_input(lexer[i], lst, lst_env, alloc) : 0;
		ft_memdel((void **)&(lexer[i]));
		i += 1;
	}
	(lexer != NULL) ? free(lexer) : 0;
}

void		lexer(char *input, t_env **lst_env, t_alloc *alloc)
{
	int		i;
	char	**lexer;
	t_ast	*lst;

	i = 0;
	lexer = NULL;
	lst = NULL;
	if (!check_opening_quote(&input, alloc) || !check_cmd_pipe(&input, alloc))
	{
		ft_memdel((void **)&input);
		return ;
	}
	historic_entry(ft_strdup(input), alloc->history, *lst_env);
	i = (input[i] == ';' && input[i + 1] != ';') ? 1 : 0;
	if ((lexer = ft_strsplit_shell(&input[i], ';')) == NULL)
	{
		ft_memdel((void **)&input);
		return ;
	}
	set_terminal(1);
	read_lexer(lexer, lst_env, lst, &alloc);
	set_terminal(0);
	ft_memdel((void **)&input);
}

void		clean_input(char *str, t_ast *lst, t_env **lst_env,
	t_alloc **alloc)
{
	char	**split;

	split = NULL;
	if ((split = ft_splitwhitespace_shell(str)) == NULL)
		return ;
	parser(split, lst, lst_env, alloc);
}
