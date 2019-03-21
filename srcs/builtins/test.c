/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 14:36:10 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 13:50:59 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"
#include "shell.h"
#include "builtins.h"
#include "builtins_test.h"
#include "error.h"

static const t_test_ope	g_test_operators[] = {
	{"-b", test_builtin_file_block, 0},
	{"-c", test_builtin_file_character, 0},
	{"-d", test_builtin_file_directory, 0},
	{"-e", test_builtin_file_exists, 0},
	{"-f", test_builtin_file_regular, 0},
	{"-g", test_builtin_file_setgidflag, 0},
	{"-L", test_builtin_file_link, 0},
	{"-p", test_builtin_file_fifo, 0},
	{"-r", test_builtin_file_r_rights, 0},
	{"-S", test_builtin_file_socket, 0},
	{"-s", test_builtin_file_zero, 0},
	{"-u", test_builtin_file_setuidflag, 0},
	{"-w", test_builtin_file_w_rights, 0},
	{"-x", test_builtin_file_x_rights, 0},
	{"-z", test_builtin_str_empty, 0},
	{"=", test_builtin_str_eq, 1},
	{"!=", test_builtin_str_ne, 1},
	{"-eq", test_builtin_int_eq, 1},
	{"-ne", test_builtin_int_ne, 1},
	{"-ge", test_builtin_int_ge, 1},
	{"-lt", test_builtin_int_lt, 1},
	{"-le", test_builtin_int_le, 1},
	{NULL, NULL, 0}
};

static int				exec_binary_primary(char **argv)
{
	int	idx;

	idx = 0;
	while (g_test_operators[idx].opt != NULL)
	{
		if (g_test_operators[idx].is_binary
				&& ft_strequ(argv[1], g_test_operators[idx].opt))
			return (g_test_operators[idx].fn(argv[0], argv[2]));
		idx++;
	}
	return (-2);
}

static int				exec_unary_primary(char **argv)
{
	int	idx;

	idx = 0;
	while (g_test_operators[idx].opt != NULL)
	{
		if (!g_test_operators[idx].is_binary
				&& ft_strequ(argv[0], g_test_operators[idx].opt))
			return (g_test_operators[idx].fn(NULL, argv[1]));
		idx++;
	}
	return (-2);
}

static int				exec_test(int argc, char **argv)
{
	if (argc == 0)
		return (0);
	else if (argc == 1)
		return (argv[0][0] != '\0');
	else if (argc == 2)
		return (exec_unary_primary(argv));
	else
		return (exec_binary_primary(argv));
}

int						test_builtins(t_ast *elem, t_alloc *alloc)
{
	size_t	argc;
	size_t	start_idx;
	int		not_result;
	int		result;

	(void)alloc;
	argc = ft_strtab_count(elem->input + 1);
	start_idx = 1;
	not_result = ft_strequ(elem->input[1], "!");
	if (not_result)
	{
		start_idx++;
		argc--;
	}
	if (argc > 3)
	{
		ft_dprintf(STDERR_FILENO, "42sh: test: too many arguments\n");
		return (2);
	}
	result = exec_test(argc, elem->input + start_idx);
	if (result >= 0)
	{
		ft_printf("debug result from test %d\n", !(not_result ? !result : result));
		return (!(not_result ? !result : result));
	}
	else
	{
		if (result != -1)
			error_test(argc, elem->input + start_idx);
		return (2);
	}
}
