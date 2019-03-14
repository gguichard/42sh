#include "shell.h"
#include "parser_lexer.h"


void		clean_input(char *str, t_ast *lst, t_alloc *alloc)
{
	char	**split;

	split = NULL;
	if ((split = ft_splitwhitespace_shell(str)) == NULL)
		return ;
	parser(split, lst, alloc);
}

void		read_lexer(char **lexer, t_ast *lst, t_alloc *alloc)
{
	int	i;
	int	x;

	i = 0;
	while (lexer && lexer[i])
	{
		x = 0;
		while (lexer[i][x] && ft_isspace(lexer[i][x]))
			x += 1;
		(lexer[i][x]) ? clean_input(lexer[i], lst, alloc) : 0;
		ft_memdel((void **)&(lexer[i]));
		i += 1;
	}
	(lexer != NULL) ? free(lexer) : 0;
}

void		lexer(char *input, t_alloc *alloc)
{
	int		i;
	char	**lexer;
	t_ast	*lst;

	i = 0;
	lexer = NULL;
	lst = NULL;
	// CLOSE CHECK QUOTE TO AVOID THE RECALL PROMPT TEST
	// if (!check_opening_quote(&input, alloc) || !check_cmd_pipe(&input, alloc))
	// {
	// 	ft_memdel((void **)&input);
	// 	return ;
	// }

	// historic_entry(ft_strdup(input), alloc->history, *lst_env);

	i = (input[i] == ';' && input[i + 1] != ';') ? 1 : 0;
	if ((lexer = ft_strsplit_shell(&input[i], ';')) == NULL)
		return ;

	// set_terminal(1);

	read_lexer(lexer, lst, alloc);

	// set_terminal(0);
}
