/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:45:48 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/05 23:45:53 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"
#include "parser_lexer.h"
#include "vars.h"
#include "builtins.h"
#include "pwd.h"
#include "str_cmd_inf.h"

char	*get_path_tild(const char *log_search)
{
	size_t			i;
	char			*login;
	struct passwd	*pass;

	i = 0;
	login = NULL;
	pass = NULL;
	while (log_search[i] && log_search[i] != '/')
		i += 1;
	login = ft_strsub(log_search, 0, i);
	pass = getpwnam(login);
	if (login)
		ft_memdel((void **)&login);
	if (pass)
	{
		login = ft_strjoin(pass->pw_dir, &(log_search[i]));
		return (login);
	}
	return (NULL);
}

int		expand_home_shortcut(char **s, t_list *vars)
{
	char	*dir;
	char	*tmp;

	dir = 0;
	tmp = 0;
	if ((*s)[0] == '~' && ((*s)[1] == '/' || (*s)[1] == '\0'))
		dir = ft_strjoin(get_home_directory(vars), (*s) + 1);
	else if (ft_strnequ(*s, "~-", 2) && ((*s)[2] == '/' || (*s)[2] == '\0'))
		dir = ft_strjoin(get_var_value(vars, "OLDPWD"), (*s) + 2);
	else if (ft_strnequ("~+", *s, 2) && ((*s)[2] == '/' || (*s)[2] == '\0'))
		dir = ft_strjoin(get_var_value(vars, "PWD"), (*s) + 2);
	else if ((*s[0] == '~') && ft_isalnum(((*s)[1])))
		dir = get_path_tild(&((*s)[1]));
	if (dir)
	{
		ft_memdel((void **)&(*s));
		(*s) = dir;
		return (1);
	}
	return (0);
}

void	check_expand_home(char **s, t_list *vars, t_str_cmd_inf *str_cmd
		, size_t *pos)
{
	size_t	len;

	len = ft_strlen(*s);
	if ((*s)[0] != '~')
		return ;
	scmd_move_to_next_char(str_cmd);
	if ((*s)[1] == '/' || (*s)[1] == '-' || (*s)[1] == '+' || (*s)[1] == '-')
		scmd_move_to_next_char(str_cmd);
	if ((*s)[2] && (*s)[2] == '/')
		scmd_move_to_next_char(str_cmd);
	expand_home_shortcut(s, vars);
	*pos = ft_strlen(*s) - len;
}
