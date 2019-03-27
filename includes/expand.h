#ifndef EXPAND_H
# define EXPAND_H

# include "vars.h"

int			expand_var(char **str, t_alloc *alloc, const char *exp);
int			expand(char **input, t_alloc *alloc, size_t *pos);

/*
************************************ TOOLS *************************************
*/

int		error_expand(char *exp);
int		check_expand_syntax(const char *str);
void	insert_var_input(char *str, char **input, int type, size_t start);
char	*get_expand_value(const char *exp, int type, t_alloc *alloc);

#endif
