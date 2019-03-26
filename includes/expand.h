#ifndef EXPAND_H
# define EXPAND_H

# include "vars.h"

int		error_expand(char *exp);
int		check_expand_syntax(const char *str);
char	*get_expand_value(const char *exp, int type, t_alloc *alloc);
char	**insert_new_tab(char **modify, int *i, char **new, t_ast *elem);
void	create_new_input(t_ast *elem, int *i, char **new);
int		expand(t_ast *elem, t_alloc *alloc, int *i, size_t *pos);
void	insert_var_input(char *str, char **input, int type, size_t start);
int		expand_var(char **str, t_alloc *alloc, const char *exp, char **input);

#endif
