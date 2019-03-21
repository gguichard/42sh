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

static int	process_pipe(t_ast *elem, t_alloc *alloc)
{
	t_exec_opt	opt_new;
	int			ret;

	opt_new.fork = do_not_fork(elem, alloc);
	opt_new.wait_hang = false;
	ret = analyzer(elem, alloc, &opt_new);
	exit(ret);
}

static void	redir_pipe(t_ast *elem, int type)
{
	if (elem->fd[0] != -1)
	{
		dup2(elem->fd[0], STDIN_FILENO);
		close(elem->fd[0]);
		close(elem->back->fd[1]);
	}
	if (elem->fd[1] != -1)
	{
		dup2(elem->fd[1], STDOUT_FILENO);
		close(elem->fd[1]);
		if (type == 1)
			close(elem->back->back->fd[0]);
		else
			close(elem->back->fd[0]);
	}
}

void	close_pipe(t_ast *elem, int already_piped)
{
	if (elem->left->type == OPERATOR || already_piped)
	{
		if (already_piped)
		{
			close(elem->fd[1]);
			close(elem->fd[0]);
		}
		else
		{
			close(elem->left->fd[1]);
			close(elem->left->fd[0]);
		}
	}
}

pid_t		fork_shit(t_ast *elem, t_alloc *alloc, int already_piped)
{
	pid_t	child;

	child = fork();
	if (child > 0)
		;
	else if (child == -1)
		exit(127);
	else if (elem->left->type != OPERATOR && !already_piped)
	{
		redir_pipe(elem->left, 0);
		process_pipe(elem->left, alloc);
	}
	else if (already_piped)
	{
		redir_pipe(elem->right, 0);
		process_pipe(elem->right, alloc);
	}
	else
	{
		redir_pipe(elem->left->right, 1);
		process_pipe(elem->left->right, alloc);
	}
	return (child);
}

int			do_pipe(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int		already_piped;

	(void)opt;
	already_piped = 0;
	while (elem->left->type == OPERATOR)
		elem = elem->left;
	while (1)
	{
		if (!already_piped && elem->fd[0] == -1 && elem->fd[1] == -1 && !pipe(elem->fd))
		{
			if (elem->left->type == OPERATOR)
				elem->left->right->fd[1] = elem->fd[1];
			else
				elem->left->fd[1] = elem->fd[1];
			elem->right->fd[0] = elem->fd[0];
		}
		if (fork_shit(elem, alloc, already_piped) == -1)
			break ;
		close_pipe(elem, already_piped);
		if (!elem->back || elem->back->type != OPERATOR)
		{
			if (already_piped)
				break ;
			already_piped = 1;
		}
		else
			elem = elem->back;
	}
	return (0);
}
// int			do_pipe(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
// {
// 	int		pid1;
// 	int		pid2;
// 	int		fd[2];
//
// 	if (!elem->right || !elem->left || pipe(fd) == -1 || (pid1 = fork()) == -1)
// 		return (1);
// 	else if (!pid1)
// 		process_pipe_left(elem->left, alloc, fd, opt->wait_hang);
// 	else
// 	{
// 		if (elem->left && ft_strcmp("|", elem->left->input[0]))
// 		{
// 			if (!elem->back || ft_strcmp("|", elem->back->input[0]))
// 			{
// 				setpgid(pid1, 0);
// 				redirect_term_controller(pid1, 0);
// 			}
// 			else
// 				setpgid(pid1, getpgid(0));
// 		}
// 		if ((pid2 = fork()) == -1)
// 			return (1);
// 		else if (!pid2)
// 			process_pipe_right(elem->right, alloc, fd, opt->wait_hang);
// 		else
// 		{
// 			setpgid(pid2, getpgid(pid1));
// 			if (!elem->back || ft_strcmp("|", elem->back->input[0]))
// 				add_pid_lst(pid2, elem);
// 			close(fd[1]);
// 			close(fd[0]);
// 			if (opt->wait_hang == true)
// 			{
// 				waitpid(pid1, &alloc->ret_val, WNOHANG);
// 				waitpid(pid2, &alloc->ret_val, WNOHANG);
// 			}
// 			else
// 			{
// 				waitpid(pid1, &alloc->ret_val, WUNTRACED);
// 				ret_status(alloc->ret_val, pid1);
// 				waitpid(pid2, &alloc->ret_val, WUNTRACED);
// 				if (!elem->back || ft_strcmp("|", elem->back->input[0]))
// 				{
// 					redirect_term_controller(0, 1);
// 					return (ret_status(alloc->ret_val, pid2));
// 				}
// 			}
// 		}
// 	}
// 	return (ret_status(alloc->ret_val, 0));
// }
