/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_tests.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 15:17:33 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 14:01:45 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins_test.h"

int	test_builtin_str_eq(const char *left, const char *right)
{
	return (ft_strequ(left, right));
}

int	test_builtin_str_ne(const char *left, const char *right)
{
	return (!test_builtin_str_eq(left, right));
}

int	test_builtin_str_empty(const char *left, const char *right)
{
	(void)left;
	return (right[0] == '\0');
}
