#include "shell.h"
#include "parser_lexer.h"
#include "error.h"
#include "token_inf.h"


/*
** TODO:
**	 - Check coherence token
**	 - create ast:
**			- get length input
**			- fill INPUT
**			- Add type
**			- insertion sort new elem
**	type:
**	TK_NOTHING,	0
**	TK_WORD,	1
**	TK_OPE,		2
**	TK_LRED_OPT,3
**	TK_RRED_OPT,4
**	TK_CMD_SEP	5
*/


t_ast	*set_new_elem(void)
{
	t_ast	*new;

	if (!(new = (t_ast *)malloc(sizeof(t_ast))))
		ft_exit_malloc();
	new->print = 0;
	new->type = NO_TYPE;
	new->input = NULL;
	new->heredoc = NULL;
	new->next = NULL;
	new->back = NULL;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

void		fill_elem(t_ast *elem, t_list *lst, int len)
{
	int i;

	i = 0;
	if (!(elem->input = (char **)malloc(sizeof(char *) * (len + 1))))
		ft_exit_malloc();
	while (i < len)
	{
		if (!(elem->input[i] = ft_strdup(get_tk(lst)->token)))
			ft_exit_malloc();
		i += 1;
		lst = lst->next;
	}
	elem->input[len] = NULL;
}

static void	set_type_elem(t_ast *elem, t_list **lst_tk, int *len)
{
	static char	*ope[10] = {">", ">>", "<", "<<", ">&", "<&", "&", "|",
	"&&", "||"};
	int			i;

	i = 0;
	while (i < 10)
	{
		if (ft_strcmp(get_tk(*lst_tk)->token, ope[i]) == 0)
			break ;
		i += 1;
	}
	if (i < 6)
	{
		*len += 1;
		elem->type = REDIR;
		*lst_tk = (*lst_tk)->next;
	}
	else if (i == 10)
	{
		// PRINT ERROR
		ft_dprintf(2, "ERROR IN PARSER.C SET_ELEM_TYPE\n");
	}
}

t_ast	*create_elem(t_list **lst_tk)
{
	t_ast	*elem;
	t_list	*start;
	int		type;
	int		len;

	start = *lst_tk;
	len = 0;
	elem = set_new_elem();
	type = get_tk(*lst_tk)->type;
	if (type == 1)
	{
		elem->type = 1;
		while (*lst_tk && get_tk(*lst_tk)->type == type)
		{
			*lst_tk = (*lst_tk)->next;
			len += 1;
		}
	}
	else if (type > 1)
	{
		elem->type = 2;
		while (*lst_tk && get_tk(*lst_tk)->type > 1)
		{
			//HAVE TO UPDATE POUR DIFFERENCIE L'OPE
			if (get_tk(*lst_tk)->type == 4)
				set_type_elem(elem, lst_tk, &len);
				*lst_tk = (*lst_tk)->next;
				len += 1;
		}
	}
	fill_elem(elem, start, len);
	return (elem);
}

t_ast			*parser(t_list *lst_tk, t_alloc *alloc)
{
	t_ast	*sort;
	t_ast	*elem;
	t_list	*tmp;
	int		j;

	(void)alloc;
	sort = NULL;
	elem = NULL;
	tmp = lst_tk;
	j = 0;

	//ADD FUNCTION TO SYNTAX ANALYSE HERE OR DURING TOKENISATION
	if (!token_analyser(tmp))
		return (NULL);


	while (tmp)
	{
		elem = create_elem(&tmp);
		sort_ast(elem, &sort);

		// ft_printf("last tmp: |%s|\n\n", get_tk(tmp)->token);


		// j = 0;
		// while (elem->input && elem->input[j])
		// {
		// 	ft_printf("input[%d]: |%s|\n", j, elem->input[j]);
		// 	j += 1;
		// }
		// ft_printf("END\n");
	}

	// while (lst_tk)
	// {
	// 	ft_printf("type[%d]: %d\nvalue[%d]: |%s|\n\n", j, get_tk(lst_tk)->type, j, get_tk(lst_tk)->token);
	// 	lst_tk = lst_tk->next;
	// 	j += 1;
	// }
	return (sort);
}
