#include "shell.h"
#include "parser_lexer.h"


char	**clean_input(char *str)
{
	char	**split;

	split = NULL;
	if ((split = ft_splitwhitespace_shell(str)) == NULL)
		return (NULL);
	return (split);
}

char	***read_lexer(char **lexer, char ***all_split_cmd)
{
	int	i;
	int	x;

	i = 0;
	while (lexer && lexer[i])
	{
		x = 0;
		while (lexer[i][x] && ft_isspace(lexer[i][x]))
			x += 1;
		all_split_cmd[i] = (lexer[i][x]) ? clean_input(lexer[i]) : 0;
		ft_memdel((void **)&(lexer[i]));
		i += 1;
	}
	(lexer != NULL) ? free(lexer) : 0;
	return (all_split_cmd);
}

char	***lexer(char *input, t_alloc *alloc)
{
	int		i;
	char	**lexer;
	char	***all_split_cmd;

	(void)alloc;
	i = 0;
	lexer = NULL;
	all_split_cmd = NULL;


	// CLOSE CHECK QUOTE TO AVOID THE RECALL PROMPT TEST
	// INTEGRATION CONVERTION ALIAS
	// if (!check_opening_quote(&input, alloc) || !check_cmd_pipe(&input, alloc))
	// {
	// 	ft_memdel((void **)&input);
	// 	return ;
	// }

	// historic_entry(ft_strdup(input), alloc->history, *lst_env);

	i = (input[i] == ';' && input[i + 1] != ';') ? 1 : 0;
	if ((lexer = ft_strsplit_shell(&input[i], ';')) == NULL)
		return (NULL);

	// i = 0;
	// while (lexer[i])
	// {
	// 	ft_printf("lexer[%d]: |%s|\n", i, lexer[i]);
	// 	i += 1;
	// }
	if (!(all_split_cmd = (char***)malloc(sizeof(char**) * (i + 1))))
		ft_exit_malloc();
	all_split_cmd = read_lexer(lexer, all_split_cmd);
	all_split_cmd[i] = NULL;
	return (all_split_cmd);
}
