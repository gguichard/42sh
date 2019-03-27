#ifndef INHIBITOR_H
# define INHIBITOR_H

void	inhib_in_db(t_str_cmd_inf *str_cmd, size_t *pos_elem, char **input,
		t_alloc *alloc);
void	inhib_quote(t_str_cmd_inf *str_cmd, size_t *pos_elem,
		char **input, t_alloc *alloc);
int		inhib_all(t_str_cmd_inf *str_cmd, char **str, t_alloc *alloc);
int		inhibitor(t_ast *elem, t_alloc *alloc);

/*
************************************ TOOLS *************************************
*/

void	remove_escaped_char(t_str_cmd_inf *str_cmd, char **input,
		size_t *pos_elem);
void	remove_escaped_char_quote(t_str_cmd_inf *str_cmd, char **input,
		size_t *pos_elem);
void	go_to_end_quote(t_str_cmd_inf *str_cmd, char **input, size_t *pos_elem);
void	update_pos_index(t_str_cmd_inf *str_cmd);

#endif
