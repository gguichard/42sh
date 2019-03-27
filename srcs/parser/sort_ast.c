#include "shell.h"
#include "parser_lexer.h"

static t_ast	*get_available_node(t_ast *sort)
{
	t_ast	*tmp;

	tmp = sort;
	if (tmp && tmp->type == LOGIC)
	{
		if (tmp->right)
		{
			while (tmp->right && tmp->right->type != CMD)
				tmp = tmp->right;
			return (tmp);
		}
	}
	return (tmp);
}

static void		cmd_ast(t_ast *node, t_ast *tmp)
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
	// if ((*sort)->type == LOGIC)
	if ((*sort)->type > tmp->type)
	{
		if (node->right)
		{
			tmp->left = node->right;
			node->right->back = tmp;
		}
		node->right = tmp;
		tmp->back = node;
	}
	else if (tmp->type > CMD)
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
		node = get_available_node(*sort);
		if (tmp->type == LOGIC)
		{
			tmp->left = *sort;
			(*sort)->back = tmp;
			*sort = tmp;
		}
		else if (tmp->type != CMD)
			link_new_node(sort, tmp, node);
		else if (tmp->type == CMD)
			cmd_ast(node, tmp);
	}
}
