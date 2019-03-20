/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_test.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 15:20:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 17:08:18 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_TEST_H
# define BUILTINS_TEST_H

typedef struct s_test_ope	t_test_ope;

struct	s_test_ope
{
	const char	*opt;
	int			(*fn)(const char *op_left, const char *op_right);
	int			is_binary;
};

/*
** UTILS.
*/
int		test_builtin_check_ints(int *tab, const char *op_left
		, const char *op_right);

/*
** STRING based tests.
*/
int		test_builtin_str_eq(const char *op_left, const char *op_right);
int		test_builtin_str_ne(const char *op_left, const char *op_right);

/*
** INTEGER tests.
*/
int		test_builtin_int_eq(const char *op_left, const char *op_right);
int		test_builtin_int_ne(const char *op_left, const char *op_right);
int		test_builtin_int_ge(const char *op_left, const char *op_right);
int		test_builtin_int_lt(const char *op_left, const char *op_right);
int		test_builtin_int_le(const char *op_left, const char *op_right);

#endif
