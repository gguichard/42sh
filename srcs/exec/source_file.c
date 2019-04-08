/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 14:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 16:37:52 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "libft.h"
#include "shell.h"
#include "error.h"
#include "check_path.h"
#include "str_cmd_inf.h"
#include "parser_lexer.h"

static void	remove_nul_inside(char *buffer, char *buffer_end)
{
	char	*end_of_string;

	while ((end_of_string = ft_memchr(buffer, '\0', buffer_end - buffer))
			!= NULL)
	{
		if (buffer_end == end_of_string)
			break ;
		ft_memcpy(end_of_string, end_of_string + 1
				, buffer_end - end_of_string);
		buffer_end -= 1;
	}
}

static void	move_scmd_cursors(t_str_cmd_inf *scmd_inf)
{
	t_str_cmd_inf	*cur;

	if (!scmd_move_to_next_char(scmd_inf))
	{
		cur = scmd_inf;
		while (cur != NULL)
		{
			cur->pos += 1;
			cur = cur->sub_str_cmd;
		}
	}
}

static void	read_n_execute(t_alloc *alloc, char *buffer, size_t buff_size)
{
	t_str_cmd_inf	scmd_inf;
	char			*start;

	ft_memset(&scmd_inf, 0, sizeof(t_str_cmd_inf));
	scmd_inf.str = buffer;
	start = buffer;
	while (scmd_inf.pos < buff_size)
	{
		if (scmd_cur_char(&scmd_inf) == '\n'
				&& !scmd_cur_char_is_escaped(&scmd_inf)
				&& scmd_cur_char_is_in_nothing(&scmd_inf))
		{
			buffer[scmd_inf.pos] = '\0';
			remove_nul_inside(start, buffer + scmd_inf.pos);
			lexer_parser(start, alloc, 0);
			start = buffer + scmd_inf.pos + 1;
		}
		move_scmd_cursors(&scmd_inf);
	}
	scmd_delete_sub_str(scmd_inf.sub_str_cmd);
}

t_error		source_file(t_alloc *alloc, const char *path)
{
	t_error		error;
	int			fd;
	struct stat	stat;
	char		*buffer;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (ERRC_UNEXPECTED);
	error = ERRC_NOERROR;
	if (fstat(fd, &stat) == -1 || !S_ISREG(stat.st_mode))
		error = ERRC_UNEXPECTED;
	else
	{
		buffer = mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE
				, fd, 0);
		if (buffer == NULL)
			error = ERRC_UNEXPECTED;
		else
			read_n_execute(alloc, buffer, stat.st_size);
		munmap(buffer, stat.st_size);
	}
	close(fd);
	return (error);
}

void		source_rc_file(t_alloc *alloc)
{
	const char	*home_dir;
	char		*rc_path;

	home_dir = get_home_directory(alloc->vars);
	rc_path = join_path(home_dir, ".42shrc");
	if (rc_path == NULL)
	{
		ft_dprintf(STDERR_FILENO, "42sh: warning: unable to load 42shrc file\n"
				, home_dir);
		return ;
	}
	source_file(alloc, rc_path);
	free(rc_path);
}
