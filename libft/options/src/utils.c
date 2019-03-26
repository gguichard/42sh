/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 20:43:08 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/25 17:05:28 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "options.h"

static int	check_brackets(int *bracket_open, char cur_char)
{
	if (cur_char == '{')
	{
		if (*bracket_open)
		{
			ft_dprintf(2, "options: can not nest bracket\n");
			exit(1);
		}
		else
			*bracket_open = 1;
	}
	else if (cur_char == '}')
	{
		if (!*bracket_open)
		{
			ft_dprintf(2, "options: tried to close a"
					"bracket that is not open\n");
			exit(1);
		}
		else
			*bracket_open = 0;
	}
	else
		return (0);
	return (1);
}

void		check_optstring(const char *optstring)
{
	size_t	len;
	int		bracket_open;

	bracket_open = 0;
	len = 0;
	while (*optstring != '\0')
	{
		if (!check_brackets(&bracket_open, *optstring) && *optstring != ':')
		{
			if (ft_strchr(optstring + 1, *optstring) != NULL)
			{
				ft_dprintf(2, "options: can not have duplicated options\n");
				exit(1);
			}
			len++;
		}
		optstring++;
	}
	if (len > MAX_OPTS)
	{
		ft_dprintf(2, "options: can not have more than %d options\n", MAX_OPTS);
		exit(1);
	}
}

int			find_opt_offset(const char *optstring, char option)
{
	size_t	offset;

	offset = 0;
	while (*optstring != '\0')
	{
		if (*optstring == option)
			return (offset);
		if (*optstring != ':' && *optstring != '{' && *optstring != '}')
			offset++;
		optstring++;
	}
	return (-1);
}

int			has_opt(t_opts *opts, char option)
{
	int	offset;

	offset = find_opt_offset(opts->optstring, option);
	if (opts->value & (1 << offset))
		return (1);
	return (0);
}

const char	*get_optarg(t_opts *opts, char option)
{
	int	offset;

	offset = find_opt_offset(opts->optstring, option);
	return ((opts->args)[offset]);
}
