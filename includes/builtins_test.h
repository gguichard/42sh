/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_test.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 15:20:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 13:59:56 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_TEST_H
# define BUILTINS_TEST_H

typedef struct s_test_ope	t_test_ope;

struct	s_test_ope
{
	const char	*opt;
	int			(*fn)(const char *left, const char *right);
	int			is_binary;
};

/*
** UTILS.
*/
int		test_builtin_check_ints(int *tab, const char *left
		, const char *right);
int		test_builtin_file_type(const char *file, int mode);
int		test_builtin_file_mode(const char *file, int mode);

/*
** FILE tests.
*/
int		test_builtin_file_block(const char *left, const char *right);
int		test_builtin_file_character(const char *left, const char *right);
int		test_builtin_file_directory(const char *left, const char *right);
int		test_builtin_file_regular(const char *left, const char *right);
int		test_builtin_file_fifo(const char *left, const char *right);
int		test_builtin_file_socket(const char *left, const char *right);
int		test_builtin_file_link(const char *left, const char *right);
int		test_builtin_file_exists(const char *left, const char *right);
int		test_builtin_file_setgidflag(const char *left, const char *right);
int		test_builtin_file_setuidflag(const char *left, const char *right);
int		test_builtin_file_r_rights(const char *left, const char *right);
int		test_builtin_file_w_rights(const char *left, const char *right);
int		test_builtin_file_x_rights(const char *left, const char *right);
int		test_builtin_file_zero(const char *left, const char *right);

/*
** STRING based tests.
*/
int		test_builtin_str_eq(const char *left, const char *right);
int		test_builtin_str_ne(const char *left, const char *right);
int		test_builtin_str_empty(const char *left, const char *right);

/*
** INTEGER tests.
*/
int		test_builtin_int_eq(const char *left, const char *right);
int		test_builtin_int_ne(const char *left, const char *right);
int		test_builtin_int_ge(const char *left, const char *right);
int		test_builtin_int_lt(const char *left, const char *right);
int		test_builtin_int_le(const char *left, const char *right);

#endif
