/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/23 20:08:47 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/24 12:39:08 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pwd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"

char		*get_history_file_path(t_alloc *alloc)
{
	char			*home_dir;
	const char		*login;
	struct passwd	*user;
	char			*tab[3];

	home_dir = get_env_value(*alloc->var, "$HOME");
	if (home_dir == NULL || home_dir[0] == '\0')
	{
		login = getlogin();
		if (login != NULL)
		{
			user = getpwnam(login);
			home_dir = user->pw_dir;
		}
	}
	if (home_dir == NULL || home_dir[0] == '\0')
		return (NULL);
	tab[0] = home_dir;
	tab[1] = ".21sh_history";
	tab[2] = NULL;
	return (ft_join(tab, home_dir[ft_strlen(home_dir) - 1] == '/' ? "" : "/"));
}

static int	read_history_last_entries(t_history *history, int fd
		, size_t file_size)
{
	char	*buffer;
	char	*buffer_pos;
	int		count;

	buffer = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (buffer == MAP_FAILED)
		return (0);
	buffer_pos = buffer + file_size;
	count = 0;
	while (--buffer_pos >= buffer && count < 500)
	{
		if (*buffer_pos != '\n')
			continue;
		*buffer_pos = '\0';
		if ((buffer_pos + 1) < (buffer + file_size))
		{
			add_history_entry(history, buffer_pos + 1);
			count++;
		}
	}
	if (count < 500)
		add_history_entry(history, buffer);
	munmap(buffer, file_size);
	return (1);
}

int			load_history_file_entries(t_alloc *alloc, t_history *history)
{
	int			ret;
	char		*file_path;
	int			fd;
	struct stat	stat;
	size_t		file_size;

	ret = 0;
	file_path = get_history_file_path(alloc);
	if (file_path == NULL)
		return (ret);
	fd = open(file_path, O_RDONLY);
	if (fd != -1)
	{
		if (fstat(fd, &stat) != -1 && S_ISREG(stat.st_mode))
		{
			file_size = (size_t)stat.st_size;
			ret = read_history_last_entries(history, fd, file_size);
		}
		close(fd);
	}
	free(file_path);
	return (ret);
}

int			save_history_entries(t_alloc *alloc, t_history *history)
{
	char			*file_path;
	int				fd;
	t_history_entry	*entry;

	file_path = get_history_file_path(alloc);
	if (file_path == NULL)
		return (0);
	fd = open(file_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd != -1)
	{
		entry = history->front;
		while (entry != NULL)
		{
			write(fd, entry->content, ft_strlen(entry->content));
			write(fd, "\n", 1);
			entry = entry->next;
		}
		close(fd);
	}
	free(file_path);
	return (fd != -1);
}
