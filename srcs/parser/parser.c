#include "shell.h"
#include "parser_lexer.h"
#include "error.h"

static t_ast	*get_available_node(t_ast **sort)
{
	t_ast	*tmp;

	tmp = *sort;
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

static void		sort_ast(t_ast *lst, t_ast **sort)
{
	t_ast	*tmp;
	t_ast	*node;

	*sort = lst;
	tmp = lst->next;
	while (tmp)
	{
		node = get_available_node(sort);
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
		tmp = tmp->next;
	}
}

static void		clean_tab_and_ast(char **input, t_ast *lst)
{
	delete_str_tab(input);
	del_lst_ast(&lst);
}

void			parser(char **input, t_ast *lst, t_var **lst_env,
				t_alloc *alloc)
{
	int		i;
	t_ast	*sort;

	if (ft_error_parse_redir(input) == 1)
	{
		g_ret[0] = 1;
		return ;
	}
	fill_ast(input, &lst, 0, -1);
	if (check_error_lst(lst) == 1)
		return (clean_tab_and_ast(input, lst));
	sort = lst;
	while (sort)
	{
		i = -1;
		while (sort->input[++i])
			if (convert_quote(&(sort->input[i]), lst_env, alloc) == -1)
				return (clean_tab_and_ast(input, lst));
		sort = sort->next;
	}
	sort_ast(lst, &sort);
	alloc->ast = &lst;
	analyzer(sort, lst_env, alloc);

	// (complete_heredoc(lst, alloc)) ? analyzer(sort, lst_env, alloc) : 0;

	clean_tab_and_ast(input, lst);
}
