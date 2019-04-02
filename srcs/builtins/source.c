/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 10:22:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/02 13:54:19 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "libft.h"
#include "shell.h"
#include "check_path.h"
#include "parser_lexer.h"
#include "str_cmd_inf.h"
#include "error.h"

static void		remove_nul_inside(char *buffer, char *buffer_end)
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

static void		move_scmd_cursors(t_str_cmd_inf *scmd_inf)
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

static void		read_n_execute(t_alloc *alloc, char *buffer, size_t buff_size)
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
			lexer_parser(start, alloc);
			start = buffer + scmd_inf.pos + 1;
		}
		move_scmd_cursors(&scmd_inf);
	}
	scmd_delete_sub_str(scmd_inf.sub_str_cmd);
}

static t_error	source_file(t_alloc *alloc, const char *path)
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

int				builtin_source(t_ast *elem, t_alloc *alloc)
{
	t_error	error;
	char	*path;

	if (elem->input[1] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: filename argument required\n"
				"%s: usage: %s filename\n"
				, elem->input[0], elem->input[0], elem->input[0]);
		return (2);
	}
	error = ERRC_FILENOTFOUND;
	if (ft_strchr(elem->input[1], '/') == NULL)
		path = search_in_path(alloc->vars, elem->input[1], R_OK, &error);
	if (error == ERRC_FILENOTFOUND)
	{
		error = check_file_rights(elem->input[1], FT_FILE, R_OK);
		if (error == ERRC_NOERROR && (path = ft_strdup(elem->input[1])) == NULL)
			error = ERRC_UNEXPECTED;
	}
	if (path != NULL && error == ERRC_NOERROR)
		error = source_file(alloc, path);
	free(path);
	if (error != ERRC_NOERROR)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: %s: %s\n", elem->input[0]
				, elem->input[1], error_to_str(error));
		return (1);
	}
	return (0);
}
