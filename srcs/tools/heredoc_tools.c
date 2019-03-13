#include "shell.h"

// static void		join_inputs(char **input, char *s)
// {
// 	char	*tmp;
//
// 	if (*input == NULL)
// 	{
// 		if (!(*input = ft_strdup(s)))
// 			ft_exit_malloc();
// 	}
// 	else
// 	{
// 		if (!(tmp = ft_strjoin(*input, "\n")))
// 			ft_exit_malloc();
// 		free(*input);
// 		if (!(*input = ft_strjoin(tmp, s)))
// 			ft_exit_malloc();
// 		free(tmp);
// 	}
// 	free(s);
// }

int				heredoc_content(t_alloc *alloc, t_ast *elem, char *s)
{
	// set_terminal(0);
	// while (1)
	// {
	// 	if (!(s = recall_prompt(alloc, 1)))
	// 	{
	// 		ft_memdel((void **)&(elem->heredoc));
	// 		break ;
	// 	}
	// 	if (ft_strcmp((elem->right) ? elem->right->input[0] :
	// 	elem->input[1], s))
	// 		join_inputs(&elem->heredoc, s);
	// 	else
	// 	{
	// 		free(s);
	// 		(!(s = ft_strjoin(elem->heredoc, "\n"))) ? ft_exit_malloc() : 0;
	// 		free(elem->heredoc);
	// 		elem->heredoc = s;
	// 		break ;
	// 	}
	// }
	// set_terminal(1);
	// if (elem->heredoc)
	// 	return (1);
	(void)alloc;
	(void)elem;
	(void)s;
	return (0);
}
