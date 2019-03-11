/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 12:26:08 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/29 13:09:20 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_VARS_H
# define EXPAND_VARS_H

# include "shell.h"
# include "str_cmd_inf.h"

typedef struct		s_expand_inf
{
	t_str_cmd_inf	scmd;
	char			*str;
	size_t			old_pos;
}					t_expand_inf;

int					str_good_replace(char **str, size_t pos, size_t len
		, const char *replacement);

size_t				get_var_name_len(const char *str);

int					char_need_to_be_escaped(t_str_cmd_inf *scmd
		, char this_char);

char				*escape_chars_in_var(t_str_cmd_inf *scmd, const char *str);

int					replace_var_by_value(t_str_cmd_inf *scmd, char **new_str
		, size_t var_len, t_shell *shell);

/*
** Alloue et retourne une nouvelle string avec les variables d'environnement
** etendues si une variable a un nom invalide une copie de son nom sera set
** dans var_error (penser a free). Retourne NULL en cas d'erreur.
*/
char				*expand_vars(const char *str, t_shell *shell
		, char **var_error);

/*
** Alloue et retourne une nouvelle string avec le home expand si present.
** Retourne NULL en cas d'erreur.
*/
char				*expand_home(const char *str, t_shell *shell
		, int remove_home_ending_slash);

#endif
