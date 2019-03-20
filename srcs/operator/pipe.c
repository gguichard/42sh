#include "shell.h"
#include "operator.h"
#include "parser_lexer.h"

int			do_not_fork(t_ast *elem, t_alloc *alloc)
{
	int		x;

	while (elem && elem->type != CMD)
	{
		if (elem->type == OPERATOR && !ft_strcmp(elem->input[0], "|"))
			break ;
		elem = elem->left;
	}
	if (elem && elem->type == CMD)
	{
		x = 0;
		while (alloc->builtins[x].name && ft_strcmp(elem->input[0], alloc->builtins[x].name))
			x += 1;
	}
	if (!elem || elem->type == OPERATOR || alloc->builtins[x].name)
		return (false);
	return (true);
}

static int	process_pipe_left(t_ast *elem, t_alloc *alloc, int *fd, bool hang)
{
	int	ret;
	t_exec_opt	opt_new;

	opt_new.fork = do_not_fork(elem, alloc);
	opt_new.wait_hang = hang;
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	ret = analyzer(elem, alloc, &opt_new);
	exit(ret);
}

static int	process_pipe_right(t_ast *elem, t_alloc *alloc, int *fd, bool hang)
{
	t_exec_opt	opt_new;
	int			ret;

	opt_new.fork = do_not_fork(elem, alloc);
	opt_new.wait_hang = hang;
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	ret = analyzer(elem, alloc, &opt_new);
	exit(ret);
}

int			do_pipe(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int		pid1;
	int		pid2;
	int		fd[2];

	if (!elem->right || !elem->left || pipe(fd) == -1 || (pid1 = fork()) == -1)
		return (1);
	else if (!pid1)
		process_pipe_left(elem->left, alloc, fd, opt->wait_hang);
	else
	{
		if (elem->left && ft_strcmp("|", elem->left->input[0]))
		{
			if (!elem->back || ft_strcmp("|", elem->back->input[0]))
			{
				setpgid(pid1, 0);
				redirect_term_controller(pid1, 0);
			}
			else
				setpgid(pid1, getpgid(0));
		}
		if ((pid2 = fork()) == -1)
			return (1);
		else if (!pid2)
			process_pipe_right(elem->right, alloc, fd, opt->wait_hang);
		else
		{
			setpgid(pid2, getpgid(pid1));
			if (!elem->back || ft_strcmp("|", elem->back->input[0]))
				add_pid_lst(pid2, elem);
			close(fd[1]);
			close(fd[0]);
			if (opt->wait_hang == true)
			{
				waitpid(pid1, &alloc->ret_val, WNOHANG);
				waitpid(pid2, &alloc->ret_val, WNOHANG);
			}
			else
			{
				waitpid(pid1, &alloc->ret_val, WUNTRACED);
				ret_status(alloc->ret_val, pid1);
				waitpid(pid2, &alloc->ret_val, WUNTRACED);
				if (!elem->back || ft_strcmp("|", elem->back->input[0]))
				{
					redirect_term_controller(0, 1);
					return (ret_status(alloc->ret_val, pid2));
				}
			}
		}
	}
	return (ret_status(alloc->ret_val, 0));
}
