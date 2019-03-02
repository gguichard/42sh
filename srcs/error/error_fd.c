/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/06 14:53:50 by tcollard          #+#    #+#             */
/*   Updated: 2019/02/15 12:46:31 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	ft_fd_exist(char *str_fd)
{
	int		fd;
	size_t	len;

	if (!str_fd)
		return (0);
	fd = ft_atoi(str_fd);
	if (fd >= 0 && fd < 3)
		return (fd);
	len = ft_strlen(str_fd);
	len = (str_fd[len - 1] == '-') ? len - 1 : len;
	write(2, "21sh: ", 6);
	write(2, str_fd, len);
	write(2, ": bad file descriptor\n", 22);
	return (-1);
}
