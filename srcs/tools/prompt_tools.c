/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 14:34:12 by jocohen           #+#    #+#             */
/*   Updated: 2019/01/30 11:00:18 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	change_quote_op(char *input)
{
	int	x;

	x = 0;
	while (input[x] && !ft_isquote(input[x]))
		x += 1;
	if (input[x])
	{
		while (input[x] && input[x] != '\n')
			x += 1;
		input[x] = 0;
		return ;
	}
	x = 0;
	while (input[x])
	{
		if (input[x] == '\n')
			ft_memmove(input + x, input + x + 1, ft_strlen(input + x + 1));
		else
			x += 1;
	}
}
