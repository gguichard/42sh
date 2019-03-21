/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_tests_rights.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 13:26:20 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 14:01:12 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "builtins_test.h"

int	test_builtin_file_r_rights(const char *left, const char *right)
{
	(void)left;
	return (access(right, R_OK) == 0);
}

int	test_builtin_file_w_rights(const char *left, const char *right)
{
	(void)left;
	return (access(right, W_OK) == 0);
}

int	test_builtin_file_x_rights(const char *left, const char *right)
{
	(void)left;
	return (access(right, X_OK) == 0);
}
