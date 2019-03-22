#include "shell.h"
#include "str_cmd_inf.h"
#include "parser_lexer.h"

static int			ft_word_counter(char *str, char c)
{
	t_str_cmd_inf	scmd;
	int				nb_word;
	int				i;

	scmd_init(&scmd, str);
	nb_word = 1;
	i = 1;
	while (1)
	{
		if (!scmd_cur_char_is_escaped(&scmd) && scmd.str[scmd.pos] == c)
		{
			++nb_word;
			while (ft_isspace(str[scmd.pos + i]))
				i += 1;
			if (str[scmd.pos + i] == c)
			{
				ft_dprintf(2, "42sh: syntax error near `;'\n");
				nb_word = -1;
				break ;
			}
		}
		if (!scmd_move_to_next_char(&scmd))
			break ;
	}
	scmd_delete(scmd.sub_str_cmd);
	return (nb_word);
}

static int			ft_position_word(char *str, char c, int word_n)
{
	int	i;
	int	i_word;
	t_str_cmd_inf	scmd;

	i = 0;
	i_word = 0;
	scmd_init(&scmd, str);
	while (1)
	{
		if (!scmd_cur_char_is_escaped(&scmd) && scmd.str[scmd.pos] == c)
		{
			i_word += 1;
			i = scmd.pos + 1;
		}
		if (!scmd_move_to_next_char(&scmd) || i_word == word_n)
			break ;
	}
	scmd_delete(scmd.sub_str_cmd);
	return (i);
}

static int			ft_counter_letter(char *str, char c, int word_n)
{
	int				i;
	int				i_word;
	int				len;
	t_str_cmd_inf	scmd;

	i = 0;
	i_word = 0;
	len = 0;
	scmd_init(&scmd, str);
	while (1)
	{
		if (!scmd_cur_char_is_escaped(&scmd) && scmd.str[scmd.pos] == c)
		{
			if (i_word == word_n)
				break ;
			i_word += 1;
			i = scmd.pos + 1;
		}
		if (!scmd_move_to_next_char(&scmd) || i_word == word_n + 1)
			break ;
	}
	scmd_delete(scmd.sub_str_cmd);
	return (scmd.pos - i);
}

static void			ft_fill(char *str, char c, int word_n, char **split)
{
	int		i;
	int		j;
	int		nb_letter;

	j = 0;
	nb_letter = ft_counter_letter(str, c, word_n);
	if (!(split[word_n] = (char*)malloc(sizeof(char) * (nb_letter + 1))))
		ft_exit_malloc();
	i = ft_position_word(str, c, word_n);
	while (str[i] && j < nb_letter)
		split[word_n][j++] = str[i++];
	split[word_n][j] = '\0';
}

char				**ft_strsplit_shell(char *str, char c)
{
	int				nb_word;
	int				i;
	char			**split;

	i = 0;
	if (!str)
		return (NULL);
	if ((nb_word = ft_word_counter(str, c)) == -1)
		return (NULL);
	if (!(split = (char**)malloc(sizeof(char*) * (nb_word + 1))))
		ft_exit_malloc();
	while (i < nb_word)
	{
		ft_fill(str, c, i, split);
		i += 1;
	}
	split[nb_word] = 0;
	return (split);
}
