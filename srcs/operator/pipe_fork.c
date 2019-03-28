#include "shell.h"
#include "parser_lexer.h"
#include "job.h"

static int		do_not_fork(t_ast *elem, t_alloc *alloc)
{
	int		x;

	while (elem && elem->type != CMD)
	{
		if (elem->type == PIPE)
			break ;
		elem = elem->left;
	}
	if (elem && elem->type == CMD)
	{
		x = 0;
		while (alloc->builtins[x].name && ft_strcmp(elem->input[0], alloc->builtins[x].name))
			x += 1;
	}
	if (!elem || elem->type == PIPE || alloc->builtins[x].name)
		return (false);
	return (true);
}

static int		process_pipe(t_ast *elem, t_alloc *alloc)
{
	t_exec_opt	opt_new;
	int			ret;

	opt_new.fork = do_not_fork(elem, alloc);
	opt_new.wait_hang = false;
	ret = analyzer(elem, alloc, &opt_new);
	exit(ret);
}

static void		redir_pipe(t_ast *elem, int type)
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

static pid_t	add_pid_pipe(t_ast *elem, int already_piped, pid_t child, bool wait_hang)
{
	static t_list	*first_cmd;
	int				ret;

	if (elem->left->type != PIPE && !already_piped)
	{
		ret = setpgid(child, 0);
		if (wait_hang == false)
			redirect_term_controller(child, 0);
		first_cmd = add_pid_lst(child, elem->left, true);
	}
	else if (already_piped)
		ret = add_pid_lst_pipe(first_cmd, child, elem->right, false);
	else
		ret = add_pid_lst_pipe(first_cmd, child, elem->left->right, true);
	if (!ret)
		return (child);
	else
		return (-1);
}

pid_t		process_fork(t_ast *elem, t_alloc *alloc, int already_piped, bool wait_hand)
{
	pid_t	child;

	child = fork();
	if (child > 0)
		child = add_pid_pipe(elem, already_piped, child, wait_hand);
	else if (!child && elem->left->type != PIPE && !already_piped)
	{
		redir_pipe(elem->left, 0);
		process_pipe(elem->left, alloc);
	}
	else if (!child && already_piped)
	{
		redir_pipe(elem->right, 0);
		process_pipe(elem->right, alloc);
	}
	else if (!child)
	{
		redir_pipe(elem->left->right, 1);
		process_pipe(elem->left->right, alloc);
	}
	return (child);
}
