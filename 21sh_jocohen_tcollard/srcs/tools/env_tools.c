/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 11:19:18 by tcollard          #+#    #+#             */
/*   Updated: 2019/03/15 15:08:10 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_env	*find_elem_env(t_env **lst, char *key)
{
	t_env *tmp;

	tmp = *lst;
	while (tmp && ft_strcmp(key, tmp->key) != 0)
		tmp = tmp->next;
	return (tmp);
}

void	add_elem_env(t_env **lst, char *key, char *value)
{
	t_env	*new;
	t_env	*last;

	if (!(new = (t_env*)malloc(sizeof(t_env))))
		ft_exit_malloc();
	last = get_last_elem_env(lst);
	if (!(new->key = ft_strdup(key)))
		ft_exit_malloc();
	if (!(new->value = ft_strdup(value)))
		ft_exit_malloc();
	new->next = NULL;
	if (!(*lst))
		*lst = new;
	else
		last->next = new;
}

void	add_shlvl(t_env **lst)
{
	t_env	*shlvl;
	int		lvl;

	if ((shlvl = find_elem_env(lst, "SHLVL")) != NULL)
	{
		lvl = (shlvl->value[0] == '-') ? 1 : 0;
		while (ft_isdigit(shlvl->value[lvl]) == 1)
			lvl += 1;
		if (shlvl->value[lvl])
		{
			free(shlvl->value);
			(!(shlvl->value = ft_strdup("1"))) ? ft_exit_malloc() : 0;
		}
		else
		{
			lvl = ft_atoi(shlvl->value);
			free(shlvl->value);
			if (lvl < 0)
				(!(shlvl->value = ft_strdup("0"))) ? ft_exit_malloc() : 0;
			else
				shlvl->value = ft_itoa(lvl + 1);
		}
	}
	else
		add_elem_env(lst, "SHLVL", "1");
}
