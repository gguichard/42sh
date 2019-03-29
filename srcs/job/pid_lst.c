#include "shell.h"
#include "error.h"
#include "job.h"

static char		*ft_tab_to_str(char **tab)
{
	size_t	x;
	size_t	y;
	char	*out;

	if (!tab)
		return (0);
	y = 0;
	x = 0;
	while (tab[y])
		x += ft_strlen(tab[y++]) + 1;
	if (!(out = ft_memalloc(x + 1)))
		return (0);
	y = 0;
	while (tab[y])
		ft_strcat(ft_strcat(out, tab[y++]), " ");
	return (out);
}

static char		*create_cmd_job(t_ast *elem, bool addpipe)
{
	char	*output;
	char	*actual;
	char	*prev;

	output = 0;
	//UPDATE: NEW AST
	while (elem && elem->back && elem->back->type < AST_PIPE && elem->back->type > AST_CMD)
		elem = elem->back;
	while (elem)
	{
		if (!(actual = ft_tab_to_str(elem->input)))
			return (0);
		prev = output;
		(!prev && addpipe == true) ? prev = ft_strdup("| ") : 0;
		output = ft_strjoin(actual, prev);
		ft_memdel((void **)&prev);
		ft_memdel((void **)&actual);
		if (!output || elem->type == AST_CMD)
			break ;
		elem = elem->left;
	}
	if (!elem)
		ft_memdel((void **)&output);
	return (output);
}

static t_job	*create_job(pid_t process, t_ast *elem, bool addpipe)
{
	t_job	*job;

	if (!(job = ft_memalloc(sizeof(t_job))))
		return (0);
	job->pid = process;
	job->gpid = getpgid(process);
	if (!(job->cmd = create_cmd_job(elem, addpipe)))
		return (0);
	job->state = RUNNING_FG;
	return (job);
}

t_list			*add_pid_lst(pid_t process, t_ast *elem, bool addpipe)
{
	t_list		*tmp;
	t_job		*job;

	tmp = 0;
	job = 0;
	if (!(job = create_job(process, elem, addpipe)) || !(tmp = ft_lstnew(job, sizeof(t_job))))
		ft_exit_malloc();
	ft_lstpush(&g_jobs, tmp);
	ft_memdel((void **)&job);
	return (tmp);
}

int				add_pid_lst_pipe(t_list *attach, pid_t process, t_ast *elem, bool addpipe)
{
	t_list	*tmp;
	t_job	*job;
	int		ret;

	tmp = 0;
	job = 0;
	ret = setpgid(process, ((t_job *)attach->content)->gpid);
	if (!(job = create_job(process, elem, addpipe)) || !(tmp = ft_lstnew(job, sizeof(t_job))))
		ft_exit_malloc();
	ft_lstpush(&((t_job *)attach->content)->pipe, tmp);
	ft_memdel((void **)&job);
	return (ret);
}
