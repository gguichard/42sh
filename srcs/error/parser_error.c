/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 14:47:08 by tcollard          #+#    #+#             */
/*   Updated: 2019/03/02 00:01:42 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	error_redir_type_1(char **ope, int x)
{
	if (!ope[x + 1])
	{
		write(2, "21sh: parse error near `\\n'\n", 28);
		return (1);
	}
	return (0);
}

static int	error_redir_type_2(char **ope, int x, int y, int len)
{
	if (!ope[x + 1])
	{
		write(2, "21sh: parse error near `;'\n", 27);
		return (1);
	}
	if (x == 0)
	{
		write(2, "21sh: parse error near `", 24);
		write(2, &ope[x][y], len);
		write(2, "'\n", 2);
		return (1);
	}
	return (0);
}

static int	error_redir_type_3(char **ope, int x, int y)
{
	if (x == 0 && y == 0 && !ope[1])
	{
		write(2, "cat: stdin: Bad file descriptor\n", 32);
		return (1);
	}
	return (0);
}

static int	check_operator_error(char **ope, int x, int y)
{
	int			i;
	size_t		len;
	static char	*operator[16] = {">>", ">>&", ">&", ">", "<<<", "<<", "<>", "<",
	"&>>", "&>", "&&", "&", "||", "|", ">&-", "<&"};

	i = 0;
	len = 0;
	while (ft_isoperator(ope[x][y + len]) == 1)
		len += 1;
	len += (ope[x][y + len] == '-' && ope[x][y + len - 1] == '&' &&
	(ft_isspace(ope[x][y + len + 1]) == 0 || !ope[x][y + len + 1])) ? 1 : 0;
	while (i < 16)
	{
		if (ft_strlen(operator[i]) == len && ft_strncmp(operator[i], &ope[x][y],
			len) == 0)
			break ;
		i += 1;
	}
	if (i < 10 || i == 15)
		return (error_redir_type_1(ope, x));
	if (i < 14)
		return (error_redir_type_2(ope, x, y, len));
	else
		return (error_redir_type_3(ope, x, y));
	return (0);
}

int			ft_error_parse_redir(char **input)
{
	int			x;
	int			y;
	int			i;

	x = 0;
	i = 0;
	while (input[x])
	{
		y = 0;
		while (input[x][y])
		{
			if (ft_isoperator(input[x][y]) == 1
			&& check_operator_error(input, x, y) == 1)
			{
				delete_str_tab(input);
				return (1);
			}
			y += 1;
		}
		x += 1;
	}
	return (0);
}
