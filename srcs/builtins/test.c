/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 14:36:10 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 17:23:21 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"
#include "shell.h"
#include "builtins.h"
#include "builtins_test.h"
#include "error.h"

static const t_test_ope	g_test_operators[] = {
	{"=", test_builtin_str_eq, 0},
	{"!=", test_builtin_str_ne, 0},
	{"-eq", test_builtin_int_eq, 1},
	{"-ne", test_builtin_int_ne, 1},
	{"-ge", test_builtin_int_ge, 1},
	{"-lt", test_builtin_int_lt, 1},
	{"-le", test_builtin_int_le, 1},
	{NULL, NULL, 0}
};

static int				count_str_tab(char **tab)
{
	int	idx;

	idx = 0;
	while (tab[idx] != NULL)
		idx++;
	return (idx);
}

static int				exec_binary_primary(char **args)
{
	int	idx;

	idx = 0;
	while (g_test_operators[idx].opt != NULL)
	{
		if (g_test_operators[idx].is_binary
				&& ft_strequ(args[1], g_test_operators[idx].opt))
			return (g_test_operators[idx].fn(args[0], args[2]));
		idx++;
	}
	return (-2);
}

static int				exec_unary_primary(char **args)
{
	int	idx;

	idx = 0;
	while (g_test_operators[idx].opt != NULL)
	{
		if (!g_test_operators[idx].is_binary
				&& ft_strequ(args[0], g_test_operators[idx].opt))
			return (g_test_operators[idx].fn(NULL, args[1]));
		idx++;
	}
	return (-2);
}

static int				exec_test(char **args, int count)
{
	if (count == 0)
		return (0);
	else if (count == 1)
		return (args[0][0] != '\0');
	else if (count == 2)
		return (exec_unary_primary(args));
	else
		return (exec_binary_primary(args));
}

int						test_builtins(t_ast *elem, t_alloc *alloc)
{
	int	idx;
	int	count;
	int	not_result;
	int	result;

	(void)alloc;
	idx = 1;
	count = count_str_tab(elem->input + 1);
	not_result = ft_strequ(elem->input[1], "!");
	if (not_result)
	{
		idx++;
		count--;
	}
	if (count > 3)
	{
		ft_dprintf(STDERR_FILENO, "42sh: test: too many arguments\n");
		return (2);
	}
	result = exec_test(elem->input + idx, count);
	if (result == -1)
		return (2);
	else if (result == -2)
		return (error_test(elem->input + idx, count));
	else
	{
		ft_printf("debug result from test %d\n", !(not_result ? !result : result));
		return (!(not_result ? !result : result));
	}
}
