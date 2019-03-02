/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 18:14:55 by tcollard          #+#    #+#             */
/*   Updated: 2019/03/01 19:05:00 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	redirection_1(t_ast *elem, t_env **lst_env, t_alloc **alloc)
{
	int	fd[3];

	fd[0] = -1;
	fd[1] = -1;
	fd[2] = -1;
	(elem->right) ? elem->right->print = 1 : 0;
	while (ft_is_redir(elem, fd, *alloc) == 1 && elem->left)
		elem = elem->left;
	if (elem->back && elem->type != REDIR)
		elem = elem->back;
	if (elem->left)
		analyzer(elem->left, lst_env, alloc);
	reinit_fd(fd, *alloc);
}

void	redirection_2(t_ast *elem, t_env **lst_env, t_alloc **alloc)
{
	int	fd[3];

	fd[0] = -1;
	fd[1] = -1;
	fd[2] = -1;
	(elem->right) ? elem->right->print = 1 : 0;
	while (ft_is_redir(elem, fd, *alloc) == 1 && elem->left)
		elem = elem->left;
	if (elem->back && elem->type != REDIR)
		elem = elem->back;
	if (elem->left)
		analyzer(elem->left, lst_env, alloc);
	reinit_fd(fd, *alloc);
}

void	redirection_3(t_ast *elem, t_env **lst_env, t_alloc **alloc)
{
	int		fd_file;
	int		fd_save;
	int		fd_redir;
	char	*tmp;

	fd_redir = 0;
	tmp = (elem->right) ? elem->right->input[0] : elem->input[1];
	if ((fd_file = open(tmp, O_RDONLY)) == -1)
	{
		if (access(tmp, F_OK) == -1)
			return (error_access(tmp));
		else
			return (error_redir(tmp));
	}
	fd_save = dup(fd_redir);
	dup2(fd_file, fd_redir);
	if (elem->left)
		analyzer(elem->left, lst_env, alloc);
	dup2(fd_save, fd_redir);
}
