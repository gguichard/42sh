/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 20:43:08 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/31 10:54:25 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "options.h"

void		check_optstring(const char *optstring)
{
	size_t	len;

	len = 0;
	while (*optstring != '\0')
	{
		if (*optstring != ':')
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
		if (*optstring != ':')
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
