#include "shell.h"
#include "builtins.h"
#include "error.h"

void		add_var(t_var **lst_env, char *var, int x, int env)
{
	t_var	*tmp;
	t_var	*new;
	size_t	len;

	tmp = NULL;
	if (!(new = (t_var*)malloc(sizeof(t_var))))
		ft_exit_malloc();
	len = ft_strlen(var);
	new->key = ft_strsub(var, 0, x);
	if ((size_t)x != len)
		new->value = ft_strsub(var, x + 1, len - x - 1);
	else
		new->value = NULL;
	new->is_env = env;
	new->next = NULL;
	insert_new_elem(lst_env, new);
}

int			env_cp(char **env, t_var **lst_env)
{
	int		i;
	int		x;
	char	*buf;

	i = 0;
	if (env[0] == NULL)
	{
		buf = getcwd(0, 0);
		add_elem_env(lst_env, "PWD", buf);
		ft_memdel((void **)&buf);
	}
	else if (!(*lst_env) && env)
		while (env[i])
		{
			x = 0;
			while (env[i][x] && env[i][x] != '=')
				x += 1;
			add_var(lst_env, env[i], x, 1);
			i += 1;
		}
	else if (!lst_env)
		return (0);
	add_shlvl(lst_env);
	return (1);
}

static void	fill_tab_env(char ***tab_str, t_var *lst)
{
	int i;

	i = 0;
	while (lst)
	{
		if (lst->is_env == 1 && lst->value)
		{
			if (!((*tab_str)[i] = (char*)malloc(sizeof(char) *
			(ft_strlen(lst->key) + ft_strlen(lst->value) + 2))))
				ft_exit_malloc();
			ft_strcat(ft_strcat(ft_strcpy((*tab_str)[i], lst->key), "="),
			lst->value);
			i += 1;
		}
		lst = lst->next;
	}
	(*tab_str)[i] = NULL;
}

void		convert_lst_tab(t_var *lst_env, char ***tab_str)
{
	int		i;
	t_var	*tmp;

	i = 0;
	tmp = lst_env;
	while (tmp)
	{
		if (tmp->is_env == 1 && tmp->value)
			i += 1;
		tmp = tmp->next;
	}
	if (!(*tab_str = (char**)malloc(sizeof(char*) * (i + 1))))
		ft_exit_malloc();
	fill_tab_env(tab_str, lst_env);
}

char		*get_env_value(t_var *lst_env, char *str)
{
	int		i;
	t_var	*tmp;

	i = 0;
	tmp = lst_env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, &str[1]) == 0)
			break ;
		else
			tmp = tmp->next;
	}
	return ((tmp) ? tmp->value : "");
}
