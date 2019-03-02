/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape_analysis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 12:25:49 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/20 16:01:18 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		check_wrong_key(char *key)
{
	if ((key[0] != 91 && key[0] != 116) || (key[1] < 65 && key[1] > 68
		&& key[1] != 72 && key[1] != 70 && key[1] != 49 && key[1] != 51
		&& key[1] != 53 && key[1] != 54 && key[1]) || (key[2] != 59 && key[2])
		|| (key[3] != 53 && key[3] != 50 && key[3])
		|| (key[4] < 65 && key[4] > 67 && key[4]))
		return (1);
	return (0);
}

int		input_keys_def(t_alloc *al, char *key)
{
	if (key[0] == 91 && key[1] >= 65 && key[1] <= 68)
		arrow_move(al->input, key[1], al->history, al->env);
	else if (key[0] == 91 && (key[1] == 72 || key[1] == 70))
		home_end_move(al->input, key[1]);
	else if (key[0] == 91 && key[1] == 51 && key[2] == 126)
		del_char(al->input, 1, al->env);
	else if (key[0] == 91 && key[1] == 49 && key[2] == 59 && key[3] == 53
		&& key[4] >= 65 && key[4] <= 68)
		ctrl_arrow_move(al->input, key[4]);
	else if (key[0] == 91 && key[1] == 49 && key[2] == 59 && key[3] == 50
		&& key[4] >= 67 && key[4] <= 68)
		selection_input(al->input, key[4], al->env);
	else if (key[0] == 91 && key[2] == 126 && key[1] >= 53 && key[1] <= 54)
		page_move(al->input, key[1], al->history, al->env);
	else if (key[0] == 116 && key[1] >= 49 && key[1] <= 51)
		selection_cmd(al->input, key[1], al->env);
	else if (check_wrong_key(key))
		return (1);
	else
		return (-1);
	return (0);
}

void	escape_analysis(t_alloc *al)
{
	static char	key[6];
	char		c;
	size_t		y;

	c = 0;
	y = 0;
	ft_bzero(key, 6);
	while (y < 5)
	{
		if ((read(0, &c, 1)) == -1)
			return ;
		key[y++] = c;
		if ((key[1] && key[1] != 49 && key[1] != 50 && key[1] != 51)
			|| (key[3] && key[3] != 50))
			selec_buffer(0, al->input, al->env);
		if (input_keys_def(al, key) >= 0)
			return ;
	}
}
