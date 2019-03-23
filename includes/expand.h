#ifndef EXPAND_H
# define EXPAND_H

/*
*********************************** INCLUDES ***********************************
*/

int		error_expand(char *exp);
int		check_expand_syntax(const char *str);
char	*get_expand_value(t_var *lst_var, const char *exp, int type);
char	**insert_new_tab(char **modify, int *i, char **new, t_ast *elem);
void	create_new_input(t_ast *elem, int *i, char **new);
int		expand(t_ast *elem, t_alloc *alloc);
void	insert_var_input(char *str, char **input, int type);

#endif
