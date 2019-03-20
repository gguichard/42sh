/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_tests_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 20:46:59 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 21:01:41 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "builtins_test.h"

int	test_builtin_file_block(const char *op_left, const char *op_right)
{
	(void)op_left;
	return (test_builtin_file_type(op_right, S_IFBLK));
}

int	test_builtin_file_character(const char *op_left, const char *op_right)
{
	(void)op_left;
	return (test_builtin_file_type(op_right, S_IFCHR));
}

int	test_builtin_file_directory(const char *op_left, const char *op_right)
{
	(void)op_left;
	return (test_builtin_file_type(op_right, S_IFDIR));
}

int	test_builtin_file_regular(const char *op_left, const char *op_right)
{
	(void)op_left;
	return (test_builtin_file_type(op_right, S_IFREG));
}

int	test_builtin_file_fifo(const char *op_left, const char *op_right)
{
	(void)op_left;
	return (test_builtin_file_type(op_right, S_IFIFO));
}
