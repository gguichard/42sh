#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "shell.h"
#include "job.h"
#include "parser_lexer.h"

static void	wait_sub_shell(pid_t child, t_alloc *alloc)
{
	t_list	*tmp;
	int		ret;
	t_list	*prev;

	if (!(tmp = add_pid_lst(child, 0, 0)))
		return ;
	((t_job *)tmp->content)->state = SUB_CMD;
	waitpid(child, &ret, WUNTRACED);
	if (WIFEXITED(ret))
		alloc->ret_val = WEXITSTATUS(ret);
	else if (WIFSTOPPED(ret))
		alloc->ret_val = WSTOPSIG(ret) + 128;
	else if (WIFSIGNALED(ret))
		alloc->ret_val = WTERMSIG(ret) + 128;
	prev = g_jobs;
	while (prev->next != NULL && prev->next != tmp)
		prev = prev->next;
	if (prev == tmp)
		g_jobs = NULL;
	ft_memdel((void **)&(tmp->content));
	ft_memdel((void **)&tmp);
	if (g_jobs)
		prev->next = NULL;
}

static void	handler_subcmd(int sig)
{
	t_list	*tmp;

	tmp = g_jobs;
	while (tmp != NULL && ((t_job *)tmp->content)->state != SUB_CMD)
		tmp = tmp->next;
	if (tmp != NULL)
	{
		kill(((t_job *)tmp->content)->pid, sig);
		write(STDOUT_FILENO, "\n", 1);
		g_sig = sig;
	}
}

static int	sig_wait_subcmd(pid_t child, t_alloc *alloc)
{
	sigset_t			mask;
	struct sigaction	act;

	sigaction(SIGINT, 0, &act);
	act.sa_handler = handler_subcmd;
	sigaction(SIGINT, &act, 0);
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_UNBLOCK, &mask, 0);
	wait_sub_shell(child, alloc);
	sigprocmask(SIG_BLOCK, &mask, 0);
	return (g_sig == SIGINT);
}

static char	*read_subcmd_file(int fd)
{
	char	buffer[4097];
	char	*output;
	char	*prev;
	ssize_t	len;

	output = NULL;
	while ((len = read(fd, buffer, 4096)) > 0)
	{
		buffer[len] = '\0';
		prev = output;
		output = ft_strjoin_free(prev, buffer);
	}
	if (output == NULL)
		return (NULL);
	len = ft_strlen(output);
	while (len > 0 && output[--len] == '\n')
		output[len] = '\0';
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
