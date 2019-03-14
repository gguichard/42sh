#include "shell.h"

static void	swap_first_elem(t_var **prev, t_var **tmp, t_var **lst)
{
	*lst = *tmp;
	(*prev)->next = (*tmp)->next;
	(*tmp)->next = *prev;
	*prev = *lst;
	*tmp = (*prev)->next;
}

static void	reinit_begin(t_var **prev, t_var **tmp, t_var **next, t_var **lst)
{
	*prev = *lst;
	*tmp = (*lst)->next;
	*next = (*lst)->next;
	if (*tmp && ft_strcmp((*prev)->key, (*tmp)->key) > 0)
		swap_first_elem(prev, tmp, lst);
	*next = (*tmp)->next;
}

static void	move_elem(t_var *tmp, t_var *next)
{
	t_var *swap;

	swap = next;
	next = swap->next;
	while (next && swap && ft_strcmp(tmp->key, swap->key) > 0)
	{
		next = next->next;
		swap = swap->next;
	}
	swap->next = tmp;
	tmp->next = next;
}

void	sort_ascii(t_var **lst)
{
	t_var	*prev;
	t_var	*tmp;
	t_var	*next;

	if (!lst || !(*lst))
		return ;
	prev = *lst;
	tmp = (*lst)->next;
	if (tmp && ft_strcmp(prev->key, tmp->key) > 0)
		swap_first_elem(&prev, &tmp, lst);
	next = tmp->next;
	while (next && tmp)
		if (ft_strcmp(tmp->key, next->key) > 0)
		{
			prev->next = next;
			move_elem(tmp, next);
			reinit_begin(&prev, &tmp, &next, lst);
		}
		else
		{
			prev = prev->next;
			tmp = tmp->next;
			next = next->next;
		}
}
