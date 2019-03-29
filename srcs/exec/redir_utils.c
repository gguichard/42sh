/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 10:46:34 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 14:41:17 by gguichar         ###   ########.fr       */
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

int			dup2_with_rc(t_redirect_inf *redirect_inf, int lopt_fd, int ropt_fd)
{
	t_redirect_save	redirect_save;
	t_list			*elem;

	redirect_save.original_fd = ropt_fd;
	redirect_save.to_fd = fcntl(ropt_fd, F_DUPFD, 10);
	if (redirect_save.to_fd == -1)
		return (0);
	elem = ft_lstnew(&redirect_save, sizeof(t_redirect_save));
	if (elem == NULL || dup2(lopt_fd, ropt_fd) < 0)
	{
		if (elem != NULL)
			ft_lstdelone(&elem, del_redirect_save);
		close(redirect_save.to_fd);
		return (0);
	}
	(void)redirect_inf;
	// ft_lstadd(&(redirect_inf->red_save), elem);
	return (1);
}

int			close_with_rc(t_redirect_inf *redirect_inf, int fd)
{
	t_redirect_save	redirect_save;
	t_list			*elem;

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
	(void)redirect_inf;
	//ft_lstadd(&(redirect_inf->red_save), elem);
	return (1);
}
