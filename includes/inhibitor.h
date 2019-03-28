#ifndef INHIBITOR_H
# define INHIBITOR_H

/*
************************************ NEW FUNCTION ******************************
*/

char	**inhib_expand_str(const char *str, t_alloc *alloc, int opt);
int		inhib_expand_tab(t_ast *elem, t_alloc *alloc);
int		inhib_in_db(t_str_cmd_inf *str_cmd, size_t *pos, char **input, t_alloc *alloc);
char		**insert_new_tab(char **modify, int *i, char **new, t_ast *elem);
void		create_new_input(t_ast *elem, int *i, char **new);

/*
************************************ TOOLS *************************************
*/

size_t	get_pos_in_array(char **array);
void	remove_escaped_char(t_str_cmd_inf *str_cmd, char **input, size_t *pos);
void	remove_escaped_char_autocomplete(t_str_cmd_inf *str_cmd, char **input,
		size_t *pos);
int		go_to_end_quote(t_str_cmd_inf *str_cmd, char **input, size_t *pos);
int		inhib_expand_in_quote(t_str_cmd_inf *str_cmd, char **array,
		size_t *pos_array, t_alloc *alloc);
void	update_pos_index(t_str_cmd_inf *str_cmd);

#endif
