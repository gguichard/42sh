#include "shell.h"
#include "builtins.h"
#include "error.h"

static void		add_env(t_var **lst_env, char *env, int x)
{
	t_var	*tmp;
	t_var	*new;
	size_t	len;

	tmp = NULL;
	if (!(new = (t_var*)malloc(sizeof(t_var))))
		ft_exit_malloc();
	len = ft_strlen(env);
	new->key = ft_strsub(env, 0, x);
	new->value = ft_strsub(env, x + 1, len - x - 1);
	new->is_env = 1;
	new->next = NULL;
	insert_new_elem(lst_env, new);
}

int				env_cp(char **env, t_var **lst_env)
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
			add_env(lst_env, env[i], x);
			i += 1;
		}
	else if (!lst_env)
		return (0);
	add_shlvl(lst_env);
	return (1);
}

void		convert_lst_tab(t_var *lst_env, char ***tab_str)
{
	t_var	*tmp;
	int		i;

	tmp = lst_env;
	i = 0;
	while (tmp)
	{
		i += 1;
		tmp = tmp->next;
	}
	if (!(*tab_str = (char**)malloc(sizeof(char*) * (i + 1))))
		ft_exit_malloc();
	tmp = lst_env;
	i = 0;
	while (tmp)
	{
		if (!((*tab_str)[i] = (char*)malloc(sizeof(char) *
		(ft_strlen(tmp->key) + ft_strlen(tmp->value) + 2))))
			ft_exit_malloc();
		ft_strcat(ft_strcat(ft_strcpy((*tab_str)[i], tmp->key), "="),
		tmp->value);
		i += 1;
		tmp = tmp->next;
	}
	(*tab_str)[i] = NULL;
}

char		*get_env_value(t_var *lst_env, char *str)
{
	int		i;
	size_t	len;
	t_var	*tmp;

	i = 0;
	tmp = lst_env;
	len = ft_strlen(str) - 1;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, &str[1]) == 0)
			break ;
		else
			tmp = tmp->next;
	}
	return ((tmp) ? tmp->value : "");
}
