#include "../../includes/shell.h"
#include "../../includes/builtins.h"
#include "../../includes/error.h"

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
