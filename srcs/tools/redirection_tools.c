/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_tools.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/07 17:32:04 by tcollard          #+#    #+#             */
/*   Updated: 2019/03/01 19:04:14 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	redir_to_file(t_ast *elem, t_alloc *alloc, int fd[3], int i)
{
	int		fd_redir;
	int		fd_open;
	int		dig;

	fd_redir = -1;
	fd_open = -1;
	dig = (ft_isdigit(elem->input[0][0]) == 1) ? 1 : 0;
	fd_redir = (dig == 1) ? ft_atoi(elem->input[0]) : 1;
	fd_open = open((dig == 1) ? elem->input[2] :
	elem->input[1], O_WRONLY | O_CREAT | ((i == 0) ? O_TRUNC :
	O_APPEND), 0644);
	if (fd_redir < 3 && fd_redir >= 0 && fd[fd_redir] == -1)
	{
		fd[fd_redir] = fd_open;
		alloc->fd[fd_redir] = dup(fd_redir);
		dup2(fd_open, fd_redir);
	}
	else
		close(fd_open);
}

int			ft_is_redir(t_ast *elem, int fd[3], t_alloc *alloc)
{
	static char		*tab_redir[6] = {">", ">>", ">>&", "<", "<<<", "<>"};
	int				i;

	i = -1;
	if (!elem || !elem->input)
		return (0);
	while (i++ < 6)
		if (ft_strcmp(elem->input[0], tab_redir[i]) == 0
			|| ft_strcmp(elem->input[1], tab_redir[i]) == 0)
		{
			redir_to_file(elem, alloc, fd, i);
			return (1);
		}
	return (0);
}

void		reinit_fd(int fd[3], t_alloc *alloc)
{
	if (fd[1] != -1)
	{
		dup2(alloc->fd[1], 1);
		if (fd[1] != 0 && fd[1] != 1 && fd[1] != 2)
			close(fd[1]);
	}
	if (fd[2] != -1)
	{
		dup2(alloc->fd[2], 2);
		if (fd[2] != 0 && fd[2] != 1 && fd[2] != 2)
			close(fd[2]);
	}
}
