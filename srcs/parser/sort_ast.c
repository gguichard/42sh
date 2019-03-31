#include "shell.h"
#include "parser_lexer.h"

static t_ast	*get_available_node(t_ast *sort, t_ast *elem)
{
	t_ast	*tmp;

	tmp = sort;
	if (tmp && tmp->type >= elem->type)
	{
		while (tmp->right && tmp->right->type >= elem->type)
			tmp = tmp->right;
		if (elem->type == AST_REDIR && tmp->type != AST_PIPE)
			while (tmp->left && tmp->left->type == AST_REDIR)
				tmp = tmp->left;
		else if (elem->type == AST_PIPE || elem->type == AST_JOB)
			while (tmp->right && tmp->right->type >= elem->type)
				tmp = tmp->right;
	}
	return (tmp);
}

static void		redir_ast(t_ast *node, t_ast *tmp)
{
	if (!node->left)
		node->left = tmp;
	else if (!node->right)
		node->right = tmp;
	tmp->back = node;
}

static void		insert_node(t_ast **sort, t_ast *tmp, t_ast *node)
{
	while (node->type > tmp->type && node->left)
		node = node->left;
	if (node->back && node->back->right)
	{
		tmp->left = node->back->right;
		node->back->right = tmp;
		tmp->back = node->back;
	}
	else if (node->back && !node->back->right)
	{
		tmp->left = node->back->left;
		node->back->left = tmp;
		tmp->back = node->back;
	}
	else
	{
		tmp->left = node;
		node->back = tmp;
	}
	((*sort)->type <= tmp->type) ? (*sort) = tmp : 0;
}

void			link_new_node(t_ast **sort, t_ast *tmp, t_ast *node)
{
	if (node->type >= tmp->type)
	{
		if (node->right)
		{
			tmp->left = node->right;
			node->right->back = tmp;
		}
		node->right = tmp;
		tmp->back = node;
	}
	else if (tmp->type > AST_REDIR)
		insert_node(sort, tmp, node);
	else
	{
		tmp->left = (*sort);
		(*sort)->back = tmp;
		*sort = tmp;
	}
}

void			sort_ast(t_ast *lst, t_ast **sort)
{
	t_ast	*tmp;
	t_ast	*node;

	tmp = lst;
	if (!(*sort))
		*sort = lst;
	else
	{
		node = get_available_node(*sort, tmp);
		if (tmp->type == AST_LOGIC)
		{
			tmp->left = *sort;
			(*sort)->back = tmp;
			*sort = tmp;
		}
		else if (node->type == AST_ASSIGN)
			redir_ast(node, tmp);
		else if (tmp->type != AST_REDIR)
			link_new_node(sort, tmp, node);
		else if (tmp->type == AST_REDIR)
			redir_ast(node, tmp);
	}
}
