#ifndef EXPAND_H
# define EXPAND_H

# include "str_cmd_inf.h"
# include "vars.h"

int		expand_var(char **str, t_alloc *alloc, const char *exp, size_t *len);
int		expand(char **input, t_alloc *alloc, size_t *pos);

/*
************************************ TOOLS *************************************
*/

int		error_expand(const char *exp);
int		check_expand_syntax(const char *str);
void	insert_var_input(char *str, char **input, int type, size_t start);
char	*get_expand_value(const char *exp, int type, t_alloc *alloc, size_t *i);
int		do_expand(char **array, t_alloc *alloc, size_t *pos_array
		, t_str_cmd_inf *str_cmd_inf);

#endif
