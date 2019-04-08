#include "libft.h"
#include "job.h"

static char	*ft_strstr_start(const char *haystack, const char *needle)
{
	size_t	i;

	i = 0;
	if (*needle == '\0')
		return (char *)(haystack);
	while (needle[i] != '\0' && needle[i] == haystack[i])
		i++;
	if (needle[i] == '\0')
		return (char *)(haystack);
	return (NULL);
}

static int	job_id_string(const char *arg, t_list **tmp, const char *bltin)
{
	t_list	*unique;
	char	*cmd;
	int		index;

	unique = NULL;
	index = 1;
	if (arg[1] == '?')
	{
		while ((*tmp))
		{
			cmd = job_cmd((*tmp)->content);
			if (ft_strstr(cmd, arg + 2))
			{
				ft_strdel(&cmd);
				if (!unique)
					unique = (*tmp);
				else
					break ;
			}
			ft_strdel(&cmd);
			index += 1;
			(*tmp) = (*tmp)->next;
		}
	}
	else
	{
		while ((*tmp))
		{
			cmd = job_cmd((*tmp)->content);
			if (ft_strstr_start(cmd, arg + 1))
			{
				ft_strdel(&cmd);
				if (!unique)
					unique = (*tmp);
				else
					break ;
			}
			ft_strdel(&cmd);
			index += 1;
			(*tmp) = (*tmp)->next;
		}
	}
	if (*tmp)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: %s: ambiguous job spec\n", bltin, (arg[1] == '?') ? arg + 2 : arg + 1);
		*tmp = NULL;
		return (0);
	}
	*tmp = unique;
	index = 1;
	unique = g_jobs;
	while (unique && unique != *tmp)
	{
		unique = unique->next;
		index += 1;
	}
	return (index);
}

int			get_job_id(const char *arg, t_list **process, const char *bltin)
{
	t_list	*tmp;
	char	*endptr;
	int		value;
	int		index;

	index = 1;
	tmp = g_jobs;
	if (!tmp)
	{
		if (process)
			*process = NULL;
		return (0);
	}
	if (ft_strequ(arg, "%%") || ft_strequ(arg, "%+"))
	{
		while (tmp->next)
		{
			index += 1;
			tmp = tmp->next;
		}
	}
	else if (ft_strequ(arg, "%-"))
	{
		while (tmp->next && tmp->next->next)
		{
			index += 1;
			tmp = tmp->next;
		}
	}
	else if (arg[0] == '%')
	{
		value = ft_strtol(arg + 1, &endptr, 10);
		if (*endptr != '\0')
			index = job_id_string(arg, &tmp, bltin);
		else
		{
			while (tmp && value > index)
			{
				index += 1;
				tmp = tmp->next;
			}
		}
	}
	if (process)
		*process = tmp;
	else if (!tmp)
		index = 0;
	return (index);
}
