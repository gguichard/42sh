/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_tests.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 16:34:17 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 14:01:36 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins_test.h"

int	test_builtin_int_eq(const char *left, const char *right)
{
	int	tab[2];

	if (!test_builtin_check_ints(tab, left, right))
		return (-1);
	else
		return (tab[0] == tab[1]);
}

int	test_builtin_int_ne(const char *left, const char *right)
{
	int	tab[2];

	if (!test_builtin_check_ints(tab, left, right))
		return (-1);
	else
		return (tab[0] != tab[1]);
}

int	test_builtin_int_ge(const char *left, const char *right)
{
	int	tab[2];

	if (!test_builtin_check_ints(tab, left, right))
		return (-1);
	else
		return (tab[0] >= tab[1]);
}

int	test_builtin_int_lt(const char *left, const char *right)
{
	int	tab[2];

	if (!test_builtin_check_ints(tab, left, right))
		return (-1);
	else
		return (tab[0] < tab[1]);
}

int	test_builtin_int_le(const char *left, const char *right)
{
	int	tab[2];

	if (!test_builtin_check_ints(tab, left, right))
		return (-1);
	else
		return (tab[0] <= tab[1]);
}
