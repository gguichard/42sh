#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "error.h"
#include "job.h"

static char		*create_cmd_job(t_ast *elem, int addpipe)
{
	char	*output;
	char	*actual;
	char	*prev;

	output = NULL;
	while (elem->back != NULL && elem->back->type == AST_ASSIGN)
		elem = elem->back;
	while (elem != NULL)
	{
		if ((actual = ft_join(elem->input, " ")) == NULL)
			return (NULL);
		prev = output;
		(prev == NULL && addpipe) ? prev = ft_strdup("| ") : 0;
		output = ft_strjoin(prev, actual);
		ft_strdel(&prev);
		ft_strdel(&actual);
		if (output == NULL)
			break ;
		//fix pour l'instant delete le if en dessous, quand apres les assign, les cmd sront a gauche
		if (elem->back && elem->back->type == AST_ASSIGN && elem->right)
			elem = elem->right;
		else
			elem = elem->left;
	}
	return (output);
}

static int		create_job(t_job *job, pid_t process, t_ast *elem, int addpipe)
{
	ft_memset(job, 0, sizeof(t_job));
	job->pid = process;
	job->gpid = getpgid(process);
	job->state = RUNNING_FG;
	return ((job->cmd = create_cmd_job(elem, addpipe)) != NULL);
}

t_list			*add_pid_lst(pid_t process, t_ast *elem, int addpipe)
{
	t_job	job;
	t_list	*node;

	if (!create_job(&job, process, elem, addpipe)
			|| (node = ft_lstnew(&job, sizeof(t_job))) == NULL)
	{
		ft_exit_malloc();
		return (NULL);
	}
	ft_lstpush(&g_jobs, node);
	return (node);
}

int				add_pid_lst_pipe(t_list *attach, pid_t process, t_ast *elem, int addpipe)
{
	t_job	job;
	t_list	*node;

	if (setpgid(process, ((t_job *)attach->content)->gpid) != 0)
		return (-1);
	if (!create_job(&job, process, elem, addpipe)
			|| (node = ft_lstnew(&job, sizeof(t_job))) == NULL)
	{
		ft_exit_malloc();
		return (-1);
	}
	ft_lstpush(&((t_job *)attach->content)->pipe, node);
	return (0);
}
