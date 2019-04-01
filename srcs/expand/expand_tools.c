#include "libft.h"
#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"
#include "error.h"

int		error_expand(const char *exp)
{
	ft_dprintf(2, "42sh: %s: bad substitution\n", exp);
	return (0);
}

int		check_expand_syntax(const char *str)
{
	int i;

	i = 0;
	if (ft_isdigit(str[i]))
	{
		while (ft_isdigit(str[i]))
			i += 1;
		if (str[i] != '}')
			return (0);
	}
	else
	{
		while (ft_isalnum(str[i]) || str[i] == '!' || str[i] == '?'
				|| str[i] == '$' || str[i] == '_')
			i += 1;
		if (str[i] != '}')
			return (0);
	}
	return (1);
}

void	insert_var_input(char *str, char **input, int type, size_t start)
{
	size_t	i;
	size_t	len;

	i = start + 1;
	len = 0;
	if (type == 1)
		while ((*input)[i] != '}')
			i += 1;
	else if ((*input)[i] == '?' || (*input)[i] == '!' || (*input)[i] == '$')
		i += 1;
	else if (!ft_isdigit((*input)[i]))
	{
		while ((*input)[i] && (ft_isalnum((*input)[i]) == 1
				|| (*input)[i] == '_'))
			i += 1;
		i -= 1;
	}
	if (start != i)
		ft_insert(input, str, start, i);
	else
	{
		len = ft_strlen(*input + start);
		ft_memmove(*input + start - 1, *input + start, len);
		(*input)[start + len - 1] = '\0';
	}
}

char	*get_expand_value(const char *exp, int type, t_alloc *alloc, size_t *i)
{
	char	*str;
	char	*value;

	*i = 0;
	str = NULL;
	if (type == 1)
		while (exp[*i] && exp[*i] != '}')
			*i += 1;
	else if (ft_isdigit(exp[*i]) || exp[*i] == '?' || exp[*i] == '!'
			|| exp[*i] == '$')
		*i += 1;
	else
		while (exp[*i] && (ft_isalnum(exp[*i]) == 1 || exp[*i] == '_'))
			*i += 1;
	if (!(str = ft_strndup(exp, *i)))
		ft_exit_malloc();
	value = get_var_for_expand(alloc, str);
	free(str);
	return (value);
}

//Recuperer la valeur de l'expand
//Spliter si white space dans l'expand
//1er mot join avec avant l'expand si il existe et que l'expand commence pas par un space
//Dernier mot de l'expand join avec suite txt apres l'expand si fin expand ne fini pas par space


int		do_expand(char ***array, t_alloc *alloc, size_t *pos_array,
		t_str_cmd_inf *str_cmd)
{
	size_t	save;
	size_t	i;
	size_t	index;
	char	*sub;
	char	*bef_sub;
	char	**new;
	char	**replace;
	size_t	len;
	size_t 	x;
	size_t	y;

	save = *pos_array;
	i = 0;
	x = 0;
	y = 1;
	sub = NULL;
	bef_sub = NULL;
	new = NULL;
	replace = NULL;
	len = 0;
	index = get_pos_in_array(*array);
	if (!expand(&((*array)[index]), alloc, pos_array))
		return (0);
	// if (!str_cmd->is_in_quote && !str_cmd->is_in_dbquote)
	// {
	// 	while ((*array)[index][save + i] && !ft_isspace((*array)[index][save + i])
	// 			&& save + i <= *pos_array)
	// 		i += 1;
	// 	if (i + save != *pos_array)
	// 	{
	// 		sub = ft_strsub((*array)[index], save, *pos_array - save);
	// 		new = ft_splitwhitespace(sub);
	// 		len = get_pos_in_array(new) + 1;
	// 		len += index;
	// 		if (!(replace = (char **)malloc(sizeof(char*) * (len + 1))))
	// 			return (0);
	// 		replace[len] = NULL;
	// 		x = 0;
	// 		while (x < index)
	// 		{
	// 			replace[x] = ft_strdup((*array)[x]);
	// 			x += 1;
	// 		}
	// 		bef_sub = ft_strsub((*array)[index], 0, save);
	// 		replace[x] = ft_strjoin(bef_sub, new[0]);
	// 		x += 1;
	// 		while (new[y] && y < len - 1)
	// 			replace[x++] = ft_strdup(new[y++]);
	// 		free(bef_sub);
	// 		replace[x] = ft_strjoin(new[y], &((*array)[index][*pos_array]));
	// 		if (new[y])
	// 			*pos_array = ft_strlen(new[y]);
	// 		ft_strtab_free(*array);
	// 		ft_strtab_free(new);
	// 		*array = replace;
	// 	}
	// }
	scmd_move_to_next_char(str_cmd);
	update_pos_index(str_cmd);
	return (1);
}
