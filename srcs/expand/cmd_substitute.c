#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "shell.h"
#include "job.h"
#include "parser_lexer.h"

static char	*read_pipe(int fd)
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
	if (!output)
		return (NULL);
	len = ft_strlen(output);
	while (len > 0 && output[--len] == '\n')
		output[len] = '\0';
	return (output);
}

static void	wait_sub_shell(pid_t child, t_alloc *alloc)
{
	t_list				*tmp;
	int					ret;
	t_list				*prev;

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
	while (prev->next && prev->next != tmp)
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
	while (tmp && ((t_job *)tmp->content)->state != SUB_CMD)
		tmp = tmp->next;
	if (!tmp)
		return ;
	kill(((t_job *)tmp->content)->pid, sig);
	write(1, "\n", 1);
	g_sig = sig;
}

static int	sig_wait_sub_cmd(pid_t child, t_alloc *alloc)
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
	if (g_sig == SIGINT)
		return (1);
	return (0);
}

char		*sub_cmd_exec(t_alloc *alloc, char *cmd)
{
	pid_t		child;
	int			fd[2];
	char		*value;

	value = NULL;
	if (pipe(fd) == -1)
		return (NULL);
	else if ((child = fork()) == -1)
		return (NULL);
	if (child == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		lexer_parser(cmd, alloc, 1);
		exit(alloc->ret_val);
	}
	close(fd[1]);
	if (!sig_wait_sub_cmd(child, alloc))
		value = read_pipe(fd[0]);
	close(fd[0]);
	return (value);
}
