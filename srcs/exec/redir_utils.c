/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 10:46:34 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 22:34:32 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"
#include "redirect_inf.h"
#include "execution.h"

static void	del_redirect_save(void *content, size_t content_size)
{
	(void)content_size;
	free(content);
}

void		use_rc_on_shell(t_exec_opt *opt)
{
	t_list			*tmp;
	t_redirect_save	*red_save;

	tmp = opt->red_save;
	while (tmp != NULL)
	{
		red_save = (t_redirect_save *)tmp->content;
		dup2(red_save->to_fd, red_save->original_fd);
		close(red_save->to_fd);
		tmp = tmp->next;
	}
	ft_lstdel(&opt->red_save, del_redirect_save);
}

int			dup2_with_rc(t_exec_opt *opt, int lopt_fd, int ropt_fd)
{
	t_redirect_save	redirect_save;
	t_list			*elem;

	if (opt->fork)
		return (dup2(lopt_fd, ropt_fd) != -1);
	redirect_save.original_fd = ropt_fd;
	redirect_save.to_fd = fcntl(ropt_fd, F_DUPFD, 10);
	if (redirect_save.to_fd == -1)
		return (0);
	elem = ft_lstnew(&redirect_save, sizeof(t_redirect_save));
	if (elem == NULL || dup2(lopt_fd, ropt_fd) == -1)
	{
		if (elem != NULL)
			ft_lstdelone(&elem, del_redirect_save);
		close(redirect_save.to_fd);
		return (0);
	}
	ft_lstadd(&opt->red_save, elem);
	return (1);
}

int			close_with_rc(t_exec_opt *opt, int fd)
{
	t_redirect_save	redirect_save;
	t_list			*elem;

	if (opt->fork)
		return (close(fd) != -1);
	redirect_save.original_fd = fd;
	redirect_save.to_fd = fcntl(fd, F_DUPFD, 10);
	if (redirect_save.to_fd == -1)
		return (0);
	elem = ft_lstnew(&redirect_save, sizeof(t_redirect_save));
	if (elem == NULL || close(fd) == -1)
	{
		if (elem != NULL)
			ft_lstdelone(&elem, del_redirect_save);
		close(redirect_save.to_fd);
		return (0);
	}
	ft_lstadd(&opt->red_save, elem);
	return (1);
}
