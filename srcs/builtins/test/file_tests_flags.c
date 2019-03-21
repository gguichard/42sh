/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_tests_flags.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 13:35:48 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 14:00:56 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "builtins_test.h"

int	test_builtin_file_setuidflag(const char *left, const char *right)
{
	(void)left;
	return (test_builtin_file_mode(right, S_ISUID));
}

int	test_builtin_file_setgidflag(const char *left, const char *right)
{
	(void)left;
	return (test_builtin_file_mode(right, S_ISGID));
}
