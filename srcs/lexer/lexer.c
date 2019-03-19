#include "shell.h"
#include "str_cmd_inf.h"
#include "parser_lexer.h"

/*
** TODO: CLEAN INUTILE FONCTION
*/


int	size_of_tab(char **lexer)
{
	int	i;
	int	x;
	int	len;

	i = 0;
	x = 0;
	len = 0;
	while (lexer[i])
	{
		x = 0;
		while (ft_isspace(lexer[i][x]))
			x += 1;
		if (lexer[i][x])
			len += 1;
		i += 1;
	}
	return (len);
}

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
	int	j;

	i = 0;
	j = 0;
	while (lexer && lexer[i])
	{
		x = 0;
		while (lexer[i][x] && ft_isspace(lexer[i][x]))
			x += 1;
		if (lexer[i][x])
		{
			all_split_cmd[j] = clean_input(lexer[i]);
			j += 1;
		}
		ft_memdel((void **)&(lexer[i]));
		i += 1;
	}
	(lexer != NULL) ? free(lexer) : 0;
	return (all_split_cmd);
}

char	**lexer(t_str_cmd_inf *scmd)
{
	int		i;
	char	**lexer;

	i = 0;
	lexer = NULL;


	// CLOSE CHECK QUOTE TO AVOID THE RECALL PROMPT TEST
	// INTEGRATION CONVERTION ALIAS
	// if (!check_opening_quote(&input, alloc) || !check_cmd_pipe(&input, alloc))
	// {
	// 	ft_memdel((void **)&input);
	// 	return ;
	// }

	while (ft_isspace(scmd->str[scmd->pos]))
		scmd_move_to_next_char(scmd);
	if (scmd->str[scmd->pos] && (ft_isoperator(scmd->str[scmd->pos]) == 1
		|| scmd->str[scmd->pos] == ';'))
	{
		ft_dprintf(2, "42sh: syntax error near '%c`\n", scmd->str[scmd->pos]);
		return (NULL);
	}
	if ((lexer = ft_strsplit_shell((char*)scmd->str, ';')) == NULL)
		return (NULL);
	// i = size_of_tab(lexer);
	// if (!(all_split_cmd = (char**)malloc(sizeof(char*) * (i + 1))))
	// 	ft_exit_malloc();
	// all_split_cmd = read_lexer(lexer, all_split_cmd);
	// all_split_cmd[i] = NULL;
	return (lexer);
}
