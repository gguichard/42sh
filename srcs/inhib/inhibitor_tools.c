/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inhibitor_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:44:38 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/05 17:48:43 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"
#include "error.h"

size_t	get_pos_in_array(char **array)
{
	size_t	i;

	i = 0;
	if (!array || !(*array))
		return (i);
	while (array[i])
		i += 1;
	return (i - 1);
}

void	update_pos_index(t_str_cmd_inf *str_cmd)
{
	if (scmd_cur_char(str_cmd) == '?' || scmd_cur_char(str_cmd) == '!'
			|| scmd_cur_char(str_cmd) == '$')
	{
		scmd_move_to_next_char(str_cmd);
		return ;
	}
	if (scmd_cur_char(str_cmd) != '{')
	{
		while (scmd_cur_is_of(str_cmd, DBQUOTE_SPE_CHAR) == 0
				&& scmd_cur_char(str_cmd)
				&& (ft_isalnum(scmd_cur_char(str_cmd))
				|| scmd_cur_char(str_cmd) == '_'))
			scmd_move_to_next_char(str_cmd);
	}
	else if (scmd_cur_char(str_cmd) == '{')
	{
		while (scmd_cur_char(str_cmd) && scmd_cur_char(str_cmd) != '}')
			scmd_move_to_next_char(str_cmd);
		scmd_move_to_next_char(str_cmd);
	}
}

char	**insert_new_tab(char **modify, int *i, char **new, t_ast *elem)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (x < *i)
	{
		if (!(modify[x] = ft_strdup(elem->input[x])))
			return (ft_strtab_free(modify));
		x += 1;
	}
	while (new[y])
		if (!(modify[x++] = ft_strdup(new[y++])))
			return (ft_strtab_free(modify));
	y = *i + 1;
	while (elem->input[y])
		if (!(modify[x++] = ft_strdup(elem->input[y++])))
			return (ft_strtab_free(modify));
	modify[x] = NULL;
	return (modify);
}

void	create_new_input(t_ast *elem, int *i, char **new)
{
	size_t	len_new;
	size_t	len_input;
	char	**modify;

	modify = NULL;
	len_new = ft_strtab_count(new);
	len_input = ft_strtab_count(elem->input);
	if (!(modify = (char**)malloc(sizeof(char*) * (len_input + len_new))))
	{
		ft_strtab_free(elem->input);
		ft_strtab_free(new);
		return ;
	}
	modify = insert_new_tab(modify, i, new, elem);
	ft_strtab_free(elem->input);
	ft_strtab_free(new);
	elem->input = modify;
	*i += len_new;
}

int		error_inhib_expand(t_str_cmd_inf *str_cmd, char **array)
{
	if (str_cmd)
	{
		scmd_clean(str_cmd);
		free(str_cmd);
	}
	if (array)
		ft_strtab_free(array);
	return (0);
}
