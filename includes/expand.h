/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:58:38 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/08 16:40:05 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "str_cmd_inf.h"
# include "vars.h"

int		expand_var(char **str, t_alloc *alloc, const char *exp, size_t *len);
int		expand(char **input, t_alloc *alloc, size_t *pos);
char	*subcmd_exec(t_alloc *alloc, const char *cmd);

/*
************************************ TOOLS *************************************
*/

int		error_expand(const char *exp);
int		check_expand_syntax(const char *str);
void	insert_var_input(char *str, char **input, int type, size_t start);
char	*get_expand_value(const char *exp, int type, t_alloc *alloc, size_t *i);
int		do_expand(char ***array, t_alloc *alloc, size_t *pos_array
		, t_str_cmd_inf *str_cmd);
void	replace_in_tab(size_t *i, char ***replace, char **array);
char	**init_new_array(char **array, size_t save, size_t pos, size_t *len);
void	del_all_alloc(char **sub, char ***array, char ***new_array);
int		expand_var_to_tab(char ***array, size_t len, size_t *pos_array
		, size_t save);
char	**ft_splitwhitespace(char const *s);
int		sig_wait_subcmd(pid_t child, t_alloc *alloc);

#endif
