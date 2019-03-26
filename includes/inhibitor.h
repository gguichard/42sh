#ifndef INHIBITOR_H
# define INHIBITOR_H

int		inhibitor(t_ast *elem, t_alloc *alloc);

/*
************************************ TOOLS *************************************
*/

void	remove_escaped_char(t_str_cmd_inf *str_cmd, t_ast *elem, int i,
		size_t *pos_elem);
void	go_to_end_quote(t_str_cmd_inf *str_cmd, t_ast *elem, int i,
		size_t *count_escape);
void	remove_escaped_char_db(t_ast *elem, int i, size_t *pos);



#endif
