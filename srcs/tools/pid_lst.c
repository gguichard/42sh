#include "shell.h"
#include "error.h"

char			*ft_tab_to_str(char **tab)
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

static t_job	*create_job(pid_t process, char **cmd)
{
	t_job	*job;

	if (!(job = ft_memalloc(sizeof(t_job))))
		return (0);
	job->pid = process;
	job->gpid = getpgid(process);
	job->cmd = ft_tab_to_str(cmd);
	job->state = RUNNING;
	return (job);
}

void			add_pid_lst(pid_t process, t_ast *elem)
{
	t_list		*tmp;
	t_job		*job;

	if (!(job = create_job(process, elem->input)) || !(tmp = ft_lstnew(job, sizeof(t_job))))
		return ;
	ft_lstadd(&g_jobs, tmp);
	ft_memdel((void **)&job);
}
