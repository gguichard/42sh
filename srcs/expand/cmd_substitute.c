/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_substitute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:35:09 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/08 17:24:43 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "shell.h"
#include "job.h"
#include "expand.h"
#include "parser_lexer.h"

static char	*read_subcmd_file(int fd)
{
	char		buffer[4097];
	char		*output;
	char		*prev;
	ssize_t		len;
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	output = NULL;
	while ((len = read(fd, buffer, 4096)) > 0)
	{
		buffer[len] = '\0';
		prev = output;
		output = ft_strjoin_free(prev, buffer);
		if (g_sig == SIGINT)
			break ;
	}
	sigprocmask(SIG_BLOCK, &mask, 0);
	(g_sig == SIGINT) ? ft_strdel(&output) : 0;
	if (output == NULL)
		return (NULL);
	/*len = ft_strlen(output);
	while (len > 0 && output[--len] == '\n')
		output[len] = '\0';*/
	return (output);
}

static char	*fork_subcmd(t_alloc *alloc, char *tempfile_path, const char *cmd)
{
	char	*value;
	pid_t	child;
	int		fd;

	value = NULL;
	if ((child = fork()) == 0)
	{
		fd = open(tempfile_path, O_CREAT | O_WRONLY, 0600);
		if (fd == -1)
			exit(1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		exit(lexer_parser(cmd, alloc, 1));
	}
	else if (child > 0 && !sig_wait_subcmd(child, alloc))
	{
		fd = open(tempfile_path, O_RDONLY);
		if (fd == -1)
			return (NULL);
		value = read_subcmd_file(fd);
		close(fd);
	}
	return (value);
}

char		*subcmd_exec(t_alloc *alloc, const char *cmd)
{
	char	*tempfile_path;
	char	*value;

	tempfile_path = get_tempfile_path(alloc, "sh-tcs");
	if (tempfile_path == NULL)
		return (NULL);
	value = fork_subcmd(alloc, tempfile_path, cmd);
	unlink(tempfile_path);
	free(tempfile_path);
	return (value);
}
