/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 10:46:34 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 10:57:44 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "libft.h"
#include "redirect_inf.h"
#include "execution.h"

int	dup2_with_rc(t_redirect_inf *redirect_inf, int lopt_fd, int ropt_fd)
{
/*	t_redirect_save	redirect_save;
	t_list			*elem;

	redirect_save.original_fd = fcntl(ropt_fd, F_DUPFD, 10);
	redirect_save.lopt_fd = lopt_fd;
	redirect_save.ropt_fd = ropt_fd;
	elem = ft_lstnew(&redirect_save, sizeof(t_redirect_save));
	if (elem == NULL || dup2(lopt_fd, ropt_fd) < 0)
		return (0);
	ft_lstadd(&(redirect_inf->red_save), elem);
	return (1);*/
	(void)redirect_inf;
	return (dup2(lopt_fd, ropt_fd) != -1);
}

int	close_with_rc(t_redirect_inf *redirect_inf, int fd)
{
/*	t_redirect_save	redirect_save;
	t_list			*elem;

	redirect_save.original_fd = fcntl(fd, F_DUPFD, 10);
	redirect_save.lopt_fd = -1;
	redirect_save.ropt_fd = fd;
	elem = ft_lstnew(&redirect_save, sizeof(t_redirect_save));
	if (elem == NULL)
		return (0);
	close(fd);
	ft_lstadd(&(redirect_inf->red_save), elem);
	return (1);*/
	(void)redirect_inf;
	return (close(fd) != -1);
}
