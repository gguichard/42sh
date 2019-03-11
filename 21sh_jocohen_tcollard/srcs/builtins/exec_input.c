/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/07 11:10:04 by tcollard          #+#    #+#             */
/*   Updated: 2019/02/22 23:05:07 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static char	**get_path_all(t_ast *elem, char **tab_path)
{
	size_t	len;
	size_t	i;
	char	*tmp;
	char	**path_all;

	len = 0;
	i = 0;
	tmp = NULL;
	while (tab_path[len])
		len += 1;
	if (!(path_all = (char**)malloc(sizeof(char *) * (len + 1))))
		ft_exit_malloc();
	while (i < len)
	{
		if (!(tmp = ft_strjoin(tab_path[i], "/")))
			ft_exit_malloc();
		if (!(path_all[i] = ft_strjoin(tmp, elem->input[0])))
			ft_exit_malloc();
		free(tmp);
		i += 1;
	}
	path_all[i] = NULL;
	return (path_all);
}

int			exec_rights(t_ast *elem, char **tab_path, char ***path_all)
{
	int	x;

	x = 0;
	if (ft_strchr(elem->input[0], '/') && access(elem->input[0], X_OK) == -1
		&& !access(elem->input[0], F_OK))
		return (exec_right_error(2, elem->input[0], path_all));
	if (!(*path_all = get_path_all(elem, tab_path)))
		return (1);
	while (tab_path && tab_path[0] != NULL && ft_strcmp(tab_path[0], "") != 0
			&& (*path_all)[x])
	{
		if (access((*path_all)[x], X_OK) == -1 && !access((*path_all)[x], F_OK))
			return (exec_right_error(2, (*path_all)[x], path_all));
		x += 1;
	}
	return (0);
}

static void	execute_cmd(t_ast *elem, char **tab_env, char **tab_path,
		char **path_all)
{
	int		x;

	x = 0;
	if (ft_strchr(elem->input[0], '/'))
		execve(elem->input[0], elem->input, tab_env);
	if (tab_path && tab_path[0] != NULL && ft_strcmp(tab_path[0], "") != 0)
		while (path_all[x] != NULL)
		{
			execve(path_all[x], elem->input, tab_env);
			x += 1;
		}
	exit(exec_error(-1, elem->input[0]));
}

static int	clean_tab(char **tab_path, char **path_all, char **tab_env)
{
	del_double_tab(tab_path, tab_env);
	delete_str_tab(path_all);
	return (ret_status());
}

int			exec_input(t_ast *elem, t_env *lst_env, char **tab_path)
{
	pid_t	child;
	char	**tab_env;
	char	**path_all;

	path_all = NULL;
	tab_env = NULL;
	if (!tab_path)
		tab_path = (find_elem_env(&lst_env, "PATH")) ?
		ft_strsplit(get_env_value(lst_env, "$PATH"), ':') :
		ft_strsplit("/usr/bin:/bin:/usr/sbin:/sbin", ':');
	if (exec_rights(elem, tab_path, &path_all))
		return (clean_tab(tab_path, path_all, tab_env));
	convert_lst_tab(lst_env, &tab_env);
	if (!(child = fork()))
	{
		if (g_pid == -1)
			exit(130);
		g_in_exec = 1;
		execute_cmd(elem, tab_env, tab_path, path_all);
	}
	g_pid = child;
	waitpid(child, &(g_ret[0]), 0);
	g_ret[1] = 1;
	return (clean_tab(tab_path, path_all, tab_env));
}
