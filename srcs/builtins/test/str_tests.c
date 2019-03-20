/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_tests.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 15:17:33 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 20:42:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtins_test.h"

int	test_builtin_str_eq(const char *op_left, const char *op_right)
{
	return (ft_strequ(op_left, op_right));
}

int	test_builtin_str_ne(const char *op_left, const char *op_right)
{
	return (!test_builtin_str_eq(op_left, op_right));
}

int	test_builtin_str_empty(const char *op_left, const char *op_right)
{
	(void)op_left;
	return (op_right[0] == '\0');
}
