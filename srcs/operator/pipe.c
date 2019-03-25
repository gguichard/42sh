#include "shell.h"
#include "operator.h"
#include "parser_lexer.h"

//CHECK IF CMD IS A BUILTIN OR AN EXEC SO NO FORK IN EXEC

int			do_not_fork(t_ast *elem, t_alloc *alloc)
{
	int		x;

	while (elem && elem->type != CMD)
	{
		if (elem->type == OPERATOR)
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

//LAUNCH ANALYZER

static int	process_pipe(t_ast *elem, t_alloc *alloc)
{
	t_exec_opt	opt_new;
	int			ret;

	opt_new.fork = do_not_fork(elem, alloc);
	opt_new.wait_hang = false;
	ret = analyzer(elem, alloc, &opt_new);
	exit(ret);
}

//REDIRECT THE STDOUT AND STDIN FOR THE CMD

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

//CLOSE PIPE WITH A DELAY

static void	close_pipe(t_ast *elem, int already_piped)
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

//ADD CMD TO THE JOBS

static pid_t	add_pid_pipe(t_ast *elem, int already_piped, pid_t child, bool wait_hang)
{
	static t_list	*first_cmd;
	int				ret;

	if (elem->left->type != OPERATOR && !already_piped)
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

//FORK AND EXEC ONE OF THE 3 TYPES OF ELEM

pid_t		fork_shit(t_ast *elem, t_alloc *alloc, int already_piped, bool wait_hand)
{
	pid_t	child;

	child = fork();
	if (child > 0)
		child = add_pid_pipe(elem, already_piped, child, wait_hand);
	else if (!child && elem->left->type != OPERATOR && !already_piped)
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

//KILL THE LAST PID BECAUSE OF AN ERROR IN THE SETTING OF GRPS OR OF FORK

void	kill_fg_pgid(void)
{
	t_list	*tmp;
	t_job	*job;

	tmp = g_jobs;
	while (tmp)
	{
		job = tmp->content;
		if (job->state == RUNNING_FG)
		{
			kill(job->pid, SIGINT);
			job->state = DONE;
			if (job->pipe)
				tmp = job->pipe;
			else
				tmp = tmp->next;
		}
	}
}

//WAITLINE FOR CHILDS

static void	pipe_waits(t_list *lst, int wait_opt)
{
	t_job	*job;

	job = lst->content;
	waitpid(job->pid, &job->status, wait_opt);
	lst = job->pipe;
	while (lst)
	{
		job = lst->content;
		waitpid(job->pid, &job->status, wait_opt);
		lst = lst->next;
	}
}

//RETURN THE LAST CMD OF THE PIPE FOR RETURN STATUS OR IF ONE HAS STOPPED RETURN THE VALUE OF THE SIGNAL STOP

static int	get_ret_val(t_list *tmp)
{
	t_job	*job;
	int		stop;
	int		ret;

	stop = 0;
	job = tmp->content;
	ret = ret_status(job->status, job->pid, job);
	if (WIFSTOPPED(job->status))
		stop = ret;
	tmp = job->pipe;
	while (tmp)
	{
		job = tmp->content;
		ret = ret_status(job->status, job->pid, job);
		if (WIFSTOPPED(job->status))
			stop = ret;
		tmp = tmp->next;
	}
	if (!stop)
		return (ret);
	return (stop);
}

//FIRST WAIT PIPE IS FOR DELETING TERMINATED PROCESSUS (ZOMBIES) AND SECOND ONE FOR ACTUAL WAIT IN PARENT ONLY IF ITS NOT A BG PROCESS

int		waiting_line(bool wait_hang, t_list *tmp)
{
	if (!tmp)
	{
		tmp = g_jobs;
		while (tmp && ((t_job *)tmp->content)->state != RUNNING_FG)
			tmp = tmp->next;
		if (!tmp)
			return (1);
	}
	pipe_waits(tmp, WNOHANG);
	if (wait_hang == false)
	{
		pipe_waits(tmp, WUNTRACED);
		redirect_term_controller(0, 1);
	}
	return (get_ret_val(tmp));
}

int			do_pipe(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int		already_piped;
	pid_t	last_child;

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
		if ((last_child = fork_shit(elem, alloc, already_piped, opt->wait_hang)) == -1)
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
	if (last_child == -1)
		kill_fg_pgid();
	else
		return (waiting_line(opt->wait_hang, 0));
	return (1);
}
