/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inhibitor.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:50:15 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/10 15:35:20 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INHIBITOR_H
# define INHIBITOR_H

# include "shell.h"
# include "str_cmd_inf.h"

int		inhib_in_db(t_str_cmd_inf *str_cmd, size_t *pos, char **input
		, t_alloc *alloc);
int		do_inhib(t_str_cmd_inf *str_cmd, char ***array, size_t *pos_array
		, t_alloc *alloc);
char	**inhib_expand_str(const char *str, t_alloc *alloc, int i);
int		inhib_expand_tab(t_ast *elem, t_alloc *alloc);
int		do_inhib_auto(t_str_cmd_inf *str_cmd, char ***array, size_t *pos_array
		, t_alloc *alloc);
int		inhib_in_db_auto(t_str_cmd_inf *str_cmd, size_t *pos, char **array
		, t_alloc *alloc);
int		inhib_expand_in_quote_auto(t_str_cmd_inf *str_cmd, char **array
		, size_t *pos, t_alloc *alloc);

char	*only_inhib_str(const char *str);

/*
************************************ TOOLS *************************************
*/

void	remove_escaped_char(t_str_cmd_inf *str_cmd, char **input, size_t *pos
		, int next);
void	remove_last_char(t_str_cmd_inf *str_cmd, size_t *pos, char **input);
void	remove_last_quote(t_str_cmd_inf *str_cmd, char **array, size_t *pos
		, size_t index);
int		remove_only_escaped_newline(char **str);
int		initialize_inhib_expand(t_str_cmd_inf **str_cmd, char ***array
		, const char *str);
int		go_to_end_quote(t_str_cmd_inf *str_cmd, char **array, size_t *pos);
int		inhib_expand_in_quote(t_str_cmd_inf *str_cmd, char **array
		, size_t *pos, t_alloc *alloc);
void	delete_line_tab(char ***array, int i);
size_t	get_pos_in_array(char **array);
void	update_pos_index(t_str_cmd_inf *str_cmd);
char	**insert_new_tab(char **modify, int *i, char **new, t_ast *elem);
void	create_new_input(t_ast *elem, int *i, char **new);
int		error_inhib_expand(t_str_cmd_inf *str_cmd, char **array);
int		do_expand_auto(char ***array, t_alloc *alloc, size_t *pos_array
		, t_str_cmd_inf *str_cmd);

#endif
