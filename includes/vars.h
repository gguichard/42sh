/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 17:53:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/02 14:37:29 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARS_H
# define VARS_H

# include "shell.h"

typedef struct	s_var
{
	char		*key;
	char		*value;
	int			is_env;
	char		*tmp_value;
}				t_var;

void			free_var(void *content, size_t content_size);
t_var			*get_var(t_list *lst, const char *key);
int				create_var(t_list **lst, const char *key, const char *value
		, int is_env);
int				update_var(t_list **lst, const char *key, const char *value);
int				unset_var(t_list **lst, const char *key);

const char		*get_var_value(t_list *vars, const char *key);
char			*get_var_for_expand(t_alloc *alloc, const char *key);
int				is_var_valid_identifier(const char *key);
void			insert_new_elem(t_list **lst, t_list *new);

t_list			*parse_env(char **environ);
char			**get_environ_from_list(t_list *env);

#endif
