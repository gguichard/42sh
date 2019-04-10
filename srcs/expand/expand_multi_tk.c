/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_multi_tk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:43:30 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/09 21:31:34 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"
#include "error.h"

void	replace_in_tab(size_t *i, char ***replace, char **array)
{
	size_t	index;

	index = get_pos_in_array(array);
	while (array[*i] && *i < index)
	{
		(*replace)[*i] = ft_strdup(array[*i]);
		*i += 1;
	}
}

char	**init_new_array(char **array, size_t save, size_t pos, size_t *len)
{
	char	**new_array;
	char	*sub;
	size_t	index;

	new_array = NULL;
	sub = NULL;
	index = get_pos_in_array(array);
	sub = ft_strsub(array[index], save, pos - save);
	new_array = ft_splitwhitespace(sub);
	ft_memdel((void *)&sub);
	*len = get_pos_in_array(new_array) + 1;
	*len += index;
	if (*len == 1)
	{
		ft_strtab_free(new_array);
		return (NULL);
	}
	return (new_array);
}

void	del_all_alloc(char **sub, char **array, char **new_array)
{
	ft_memdel((void *)sub);
	ft_strtab_free(array);
	ft_strtab_free(new_array);
}

int		expand_var_to_tab(char ***array, size_t len, size_t *pos_array
		, size_t save)
{
	char	*sub;
	char	**new_array;
	char	**replace;
	size_t	i;
	size_t	x;

	i = 0;
	x = 1;
	sub = NULL;
	if (!(new_array = init_new_array(*array, save, *pos_array, &len)))
		return (1);
	if (!(replace = malloc(sizeof(char*) * (len + 1))))
		return (!!ft_strtab_free(new_array));
	replace_in_tab(&i, &replace, *array);
	sub = ft_strsub((*array)[get_pos_in_array(*array)], 0, save);
	replace[i++] = ft_strjoin(sub, new_array[0]);
	while (new_array[x] && i < (len - 1))
		replace[i++] = ft_strdup(new_array[x++]);
	replace[i++] = ft_strjoin(new_array[x]
			, &((*array)[get_pos_in_array(*array)][*pos_array]));
	replace[i] = NULL;
	(new_array[x]) ? *pos_array = ft_strlen(new_array[x]) : 0;
	del_all_alloc(&sub, *array, new_array);
	*array = replace;
	return (1);
}
