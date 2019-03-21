/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_tests_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 20:46:59 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 14:00:38 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "builtins_test.h"

int	test_builtin_file_fifo(const char *left, const char *right)
{
	(void)left;
	return (test_builtin_file_type(right, S_IFIFO));
}

int	test_builtin_file_socket(const char *left, const char *right)
{
	(void)left;
	return (test_builtin_file_type(right, S_IFSOCK));
}

int	test_builtin_file_link(const char *left, const char *right)
{
	struct stat	info;

	(void)left;
	if (lstat(right, &info) == -1)
		return (0);
	return (S_ISLNK(info.st_mode));
}

int	test_builtin_file_zero(const char *left, const char *right)
{
	struct stat	info;

	(void)left;
	if (stat(right, &info) == -1)
		return (0);
	return (info.st_size > 0);
}
