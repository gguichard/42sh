/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_tests_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 16:34:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 17:03:56 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>
#include "libft.h"

static int	test_builtin_check_int(int *ptr, const char *operand)
{
	long	value;
	char	*endptr;

	if (operand[0] == '\0')
		return (0);
	value = ft_strtol(operand, &endptr, 10);
	if (*endptr != '\0' || value < INT_MIN || value > INT_MAX)
	{
		ft_dprintf(STDERR_FILENO, "42sh: test: %s: "
				"integer expression expected\n", operand);
		return (0);
	}
	*ptr = (int)value;
	return (1);
}

int			test_builtin_check_ints(int *tab, const char *op_left
		, const char *op_right)
{
	return (test_builtin_check_int(tab, op_left)
			&& test_builtin_check_int(tab + 1, op_right));
}
