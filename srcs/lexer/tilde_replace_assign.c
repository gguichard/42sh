/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_replace_assign.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 14:12:28 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/10 14:28:58 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pwd.h>
#include "libft.h"
#include "shell.h"
#include "str_cmd_inf.h"
#include "parser_lexer.h"
#include "vars.h"

static const char	*get_home_value(const char *s, size_t pos, t_list *vars)
{
	if (s[pos] == '~' && (s[pos + 1] == '/' || s[pos + 1] == '\0'
			|| s[pos + 1] == ':' || s[pos + 1] == '='))
		return (get_home_directory(vars));
	else if (ft_strnequ(&(s[pos]), "~-", 2) && (s[pos + 2] == '/'
			|| s[pos + 2] == '\0' || s[pos + 2] == ':' || s[pos + 2] == '='))
		return (get_var_value(vars, "OLDPWD"));
	else if (ft_strnequ("~+", &(s[pos]), 2) && (s[pos + 2] == '/'
			|| s[pos + 2] == '\0' || s[pos + 2] == ':' || s[pos + 2] == '='))
		return (get_var_value(vars, "PWD"));
	return (NULL);
}

static int			do_expand_home_assign(char **s, t_list *vars
			, t_str_cmd_inf *str_cmd, size_t *pos)
{
	size_t	i;
	size_t	len;
	char	*cst;

	i = 1;
	cst = NULL;
	len = ft_strlen(*s);
	if ((*s)[*pos - 1] == '=' && (*s)[*pos] == '~')
	{
		if ((cst = (char *)get_home_value(*s, *pos, vars)))
		{
			if ((*s)[*pos + 1] == '-' || (*s)[*pos + 1] == '+')
				i += scmd_move_to_next_char(str_cmd);
			ft_strreplace_inside(s, *pos, i, cst);
			*pos = ft_strlen(*s) - len + 1;
			scmd_move_to_next_char(str_cmd);
		}
	}
	return (1);
}

int					check_expand_home_assign(char **s, t_list *vars
			, t_str_cmd_inf *str_cmd, size_t *pos)
{
	char	*cst;
	size_t	len;
	int		i;

	len = ft_strlen(*s);
	cst = NULL;
	i = (int)(*pos);
	*pos += scmd_move_to_next_char(str_cmd);
	if (i == 0)
		return (1);
	i -= 1;
	while ((*s)[i] && i >= 0)
	{
		if (!ft_isalnum((*s)[i]) && (*s)[i] != '_')
			break ;
		i -= 1;
	}
	if ((i < 0 && !ft_isalpha((*s)[0]) && (*s)[0] != '_')
			|| (i >= 0 && !ft_isalpha((*s)[i]) && (*s)[i] != '_'))
		return (1);
	return (do_expand_home_assign(s, vars, str_cmd, pos));
}
