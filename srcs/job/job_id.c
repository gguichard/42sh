/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_id.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 12:25:42 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/09 12:53:08 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	look_for_str_job(t_list **tmp, const char *arg
			, char *(*str_check)(const char *, const char *))
{
	char	*cmd;
	int		index;
	t_list	*voyager;

	index = 1;
	voyager = g_jobs;
	while (voyager)
	{
		cmd = job_cmd(voyager->content);
		if (str_check(cmd, (arg[1] == '?') ? arg + 2 : arg + 1))
		{
			ft_strdel(&cmd);
			if (!(*tmp))
				(*tmp) = voyager;
			else
				break ;
		}
		else if (!(*tmp))
			index += 1;
		ft_strdel(&cmd);
		voyager = voyager->next;
	}
	if (voyager)
		(*tmp) = NULL;
	return (index);
}

static int	job_id_string(const char *arg, t_list **tmp, const char *bltin)
{
	int		index;

	index = 1;
	(*tmp) = NULL;
	if (arg[1] == '?')
		index = look_for_str_job(tmp, arg, ft_strstr);
	else
		index = look_for_str_job(tmp, arg, ft_strstr_start);
	if (!(*tmp))
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: %s: ambiguous job spec\n"
					, bltin, (arg[1] == '?') ? arg + 2 : arg + 1);
		return (0);
	}
	return (index);
}

static int	check_job_id_nb_str(const char *arg, t_list **tmp
			, const char *bltin)
{
	char	*endptr;
	int		value;
	int		index;

	endptr = NULL;
	index = 0;
	value = ft_strtol((arg[0] == '%') ? arg + 1 : arg, &endptr, 10);
	if (*endptr != '\0' && arg[0] == '%')
		index = job_id_string(arg, tmp, bltin);
	else if (*endptr == '\0')
		index = job_id_nb(tmp, value);
	else
		*tmp = NULL;
	return (index);
}

int			get_job_id(const char *arg, t_list **process, const char *bltin)
{
	t_list	*tmp;
	int		index;

	index = 0;
	tmp = g_jobs;
	if (!tmp && process)
		*process = NULL;
	if (!tmp)
		return (0);
	if (ft_strequ(arg, "%%") || ft_strequ(arg, "%+"))
		index = look_for_recent_job(&tmp, current_job_id);
	else if (ft_strequ(arg, "%-"))
		index = look_for_recent_job(&tmp, previous_job_id);
	else
		index = check_job_id_nb_str(arg, &tmp, bltin);
	if (process)
		*process = tmp;
	else if (!tmp)
		index = 0;
	return (index);
}
