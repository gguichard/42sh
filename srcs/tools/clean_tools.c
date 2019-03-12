#include "shell.h"

void	delete_str_tab(char **tab_str)
{
	int	i;

	i = 0;
	if (!tab_str)
		return ;
	while (tab_str[i])
	{
		ft_memdel((void **)&(tab_str[i]));
		i += 1;
	}
	free(tab_str);
	tab_str = NULL;
}

void	del_lst_env(t_var **lst)
{
	t_var	*tmp;

	if (!lst || !(*lst))
		return ;
	tmp = *lst;
	while (tmp)
	{
		*lst = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = *lst;
	}
	*lst = NULL;
}

void	del_lst_ast(t_ast **lst)
{
	t_ast	*tmp;
	int		i;

	i = 0;
	tmp = *lst;
	if (!lst || !tmp)
		return ;
	while (tmp)
	{
		*lst = tmp->next;
		ft_memdel((void **)&(tmp->heredoc));
		delete_str_tab(tmp->input);
		free(tmp);
		tmp = *lst;
	}
	*lst = NULL;
	lst = NULL;
}

void	del_double_tab(char **tab1, char **tab2)
{
	if (tab1)
		delete_str_tab(tab1);
	if (tab2)
		delete_str_tab(tab2);
}

void	del_alloc(t_alloc **alloc)
{
	if (!(*alloc))
		return ;
	// if ((*alloc)->history)
	// {
	// 	ft_del_hist((*alloc)->history);
	// 	(*alloc)->history = NULL;
	// }
	if ((*alloc)->input)
	{
		((*alloc)->input->s) ? free((*alloc)->input->s) : 0;
		(*alloc)->input = NULL;
	}
	if ((*alloc)->var)
	{
		del_lst_env((*alloc)->var);
		(*alloc)->var = NULL;
	}
	if ((*alloc)->ast)
	{
		del_lst_ast((*alloc)->ast);
		(*alloc)->ast = NULL;
	}
	*alloc = NULL;
}
