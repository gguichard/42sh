#include <stdlib.h>
#include "shell.h"
#include "parser_lexer.h"
#include "token_inf.h"
#include "error.h"

t_ast	*set_new_elem(void)
{
	t_ast	*new;

	if (!(new = (t_ast *)malloc(sizeof(t_ast))))
		return (0);
	new->fd[0] = -1;
	new->fd[1] = -1;
	new->type = AST_NO_TYPE;
	new->input = NULL;
	new->back = NULL;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

void	set_type(t_ast *elem, t_list *lst_tk)
{
	static char	*ope[11] = {
		">", ">>", "<", "<<",
		">&", "<&", "&",
		"|", "&&", "||", ";"
	};
	char		*str;
	int			i;

	i = 0;
	str = get_tk(lst_tk)->token;
	while (ft_strcmp(ope[i], str) != 0 && i < 11)
		i += 1;
	if (i < 6)
		elem->type = AST_REDIR;
	else if (i == 6)
		elem->type = AST_JOB;
	else if (i == 7)
		elem->type = AST_PIPE;
	else if (i < 10)
		elem->type = AST_LOGIC;
	else if (i == 10)
		elem->type = AST_CMD_SEP;
}

void	init_input(t_ast *elem, int len, t_list *lst_tk)
{
	t_token_type	type;

	type = get_tk(lst_tk)->type;
	if (type == TK_RED_LOPT_FD)
		set_type(elem, lst_tk->next);
	else if (type > 3)
		set_type(elem, lst_tk);
	else if (type == TK_ASSIGN)
		elem->type = AST_ASSIGN;
	else if (type == TK_CMD)
		elem->type = AST_CMD;
	if (!(elem->input = (char**)malloc(sizeof(char*) * (len + 1))))
		return ;
	if (!(elem->input[0] = ft_strdup(get_tk(lst_tk)->token)))
	{
		ft_memdel((void **)&(elem->input));
		return ;
	}
	elem->input[len] = NULL;
}

t_ast	*create_ast_branch(t_list **lst_tk)
{
	t_ast			*sort;
	t_ast			*elem;
	t_token_type	type;

	sort = NULL;
	elem = NULL;
	while (*lst_tk)
	{
		type = get_tk(*lst_tk)->type;
		if (type == TK_CMD_SEP && ft_strcmp(get_tk(*lst_tk)->token, ";") == 0)
			break ;
		else if (type != TK_PARAM)
		{
			if (!(elem = create_elem(lst_tk)))
			{
				del_ast(&sort);
				return (0);
			}
			sort_ast(elem, &sort);
		}
		else
			*lst_tk = (*lst_tk)->next;
	}
	return (sort);
}

t_ast	*parser(t_list *lst_tk)
{
	t_ast	*sort;
	t_ast	*elem;
	t_ast	*branch;

	sort = NULL;
	elem = NULL;
	branch = NULL;
	if (token_analyser(lst_tk, 1) == PR_ERROR)
		return (NULL);
	while (lst_tk != NULL)
	{
		branch = create_ast_branch(&lst_tk);
		if (!sort)
			sort = branch;
		if (sort && sort->type == AST_CMD_SEP && !sort->right)
		{
			sort->right = branch;
			branch->back = sort;
		}
		else if (lst_tk != NULL && get_tk(lst_tk)->type == TK_CMD_SEP)
		{
			if (!(elem = create_elem(&lst_tk)))
			{
				del_ast(&sort);
				del_ast(&branch);
				return (0);
			}
			elem->left = sort;
			sort->back = elem;
			sort = elem;
		}
		else if (lst_tk != NULL)
			lst_tk = lst_tk->next;
	}
	return (sort);
}
