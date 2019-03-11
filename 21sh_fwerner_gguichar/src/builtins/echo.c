/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 15:20:59 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 14:50:13 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"

static size_t	len_sub_parts(int index, int argc, char **argv)
{
	size_t	len;

	len = 0;
	while (index < argc)
	{
		len += ft_strlen(argv[index]);
		index++;
		if (index < argc)
			len++;
	}
	return (len);
}

static char		*concat_args(int print_nl, int index, int argc, char **argv)
{
	size_t	total_len;
	char	*tmp;
	size_t	offset;
	size_t	len;

	total_len = len_sub_parts(index, argc, argv);
	tmp = (char *)malloc((total_len + print_nl + 1) * sizeof(char));
	if (tmp == NULL)
		return (NULL);
	offset = 0;
	while (index < argc)
	{
		len = ft_strlen(argv[index]);
		ft_memcpy(tmp + offset, argv[index], len);
		offset += len;
		if (++index < argc)
		{
			tmp[offset++] = ' ';
			len++;
		}
	}
	if (print_nl)
		tmp[offset++] = '\n';
	tmp[offset] = '\0';
	return (tmp);
}

int				builtin_echo(t_shell *shell, int argc, char **argv)
{
	int		index;
	int		print_nl;
	int		ret;
	char	*tmp;

	(void)shell;
	index = 1;
	print_nl = !(argc > 1 && ft_strequ("-n", argv[1]));
	if (!print_nl)
		index++;
	ret = 0;
	if ((tmp = concat_args(print_nl, index, argc, argv)) == NULL)
	{
		ft_dprintf(2, "%s: %s: Unexpected error\n", ERR_PREFIX, argv[0]);
		return (1);
	}
	else if (write(STDOUT_FILENO, tmp, ft_strlen(tmp)) < 0)
	{
		ret = 1;
		ft_dprintf(2, "%s: %s: Write error\n", ERR_PREFIX, argv[0]);
	}
	free(tmp);
	return (ret);
}
