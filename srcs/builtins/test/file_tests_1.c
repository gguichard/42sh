/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_tests_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 20:46:59 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 14:00:28 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>
#include "builtins_test.h"

int	test_builtin_file_block(const char *left, const char *right)
{
	(void)left;
	return (test_builtin_file_type(right, S_IFBLK));
}

int	test_builtin_file_character(const char *left, const char *right)
{
	(void)left;
	return (test_builtin_file_type(right, S_IFCHR));
}

int	test_builtin_file_directory(const char *left, const char *right)
{
	(void)left;
	return (test_builtin_file_type(right, S_IFDIR));
}

int	test_builtin_file_regular(const char *left, const char *right)
{
	(void)left;
	return (test_builtin_file_type(right, S_IFREG));
}

int	test_builtin_file_exists(const char *left, const char *right)
{
	(void)left;
	return (access(right, F_OK) == 0);
}
