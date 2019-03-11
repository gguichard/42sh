/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:06:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 13:16:28 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"

static int	get_redirect_input_fd(t_redirect_inf *redirect_inf)
{
	int	fd;

	fd = redirect_inf->to_fd;
	if (fd == FD_NOTSET)
	{
		fd = open(redirect_inf->to_word, O_RDONLY);
		redirect_inf->close_to_fd = 1;
	}
	else if (fd == FD_AMPERSAND)
	{
		ft_dprintf(2, "%s: %s: Ambiguous redirect\n", ERR_PREFIX
				, redirect_inf->to_word);
		return (0);
	}
	return (fd);
}

int			redirect_input(t_redirect_inf *redirect_inf)
{
	int	from_fd;
	int	to_fd;
	int	ret;

	from_fd = redirect_inf->from_fd;
	if (from_fd == FD_DEFAULT)
		from_fd = 0;
	if (redirect_inf->close_to_fd)
	{
		close(from_fd);
		return (1);
	}
	to_fd = get_redirect_input_fd(redirect_inf);
	if (to_fd < 0)
		return (0);
	ret = 0;
	if (from_fd != to_fd)
	{
		ret = dup2_with_rc(redirect_inf, to_fd, from_fd);
		if (!ret)
			ft_dprintf(2, "%s: %d: Bad file descriptor\n", ERR_PREFIX, to_fd);
	}
	if (redirect_inf->close_to_fd)
		close(to_fd);
	return (ret);
}
