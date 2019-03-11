#include "../../includes/shell.h"
#include "../../includes/parser_lexer.h"

static void	fill_input_redir(char **s, t_ast *elem, int *pos, int nb_wd)
{
	int		i;
	int		save;
	int		wd;

	i = 0;
	save = i;
	wd = 0;
	while (s[*pos][i])
	{
		if (ft_isdigit(s[*pos][i]) == 1)
			wd += get_last_digit(&i, s[*pos], &save, &(elem->input[wd]));
		else if (ft_isoperator(s[*pos][i]) == 1)
			wd += get_last_operator(&i, s[*pos], &save, &(elem->input[wd]));
		else
			wd += get_last_index(&i, s[*pos], &save, &(elem->input[wd]));
	}
	if (wd < nb_wd)
	{
		*pos += 1;
		elem->input[wd] = ft_strdup(s[*pos]);
		elem->input[wd + 1] = NULL;
	}
	else
		elem->input[wd] = NULL;
}

static void	split_redir(char **s, t_ast *elem, int *pos)
{
	int		wd;
	int		i;

	wd = 0;
	i = 0;
	while (s[*pos][i])
		if (ft_isdigit(s[*pos][i]) == 1 && (wd += 1))
			while (ft_isdigit(s[*pos][i]) == 1)
				i += 1;
		else if (ft_isoperator(s[*pos][i]) == 1 && (wd += 1))
		{
			while (ft_isoperator(s[*pos][i]) == 1)
				i += 1;
			if (s[*pos][i] == '-' && (wd += 1))
				i += 1;
		}
		else
			get_last_index_split(&i, s[*pos], &wd);
	(elem->type <= AGREG) ? wd += 1 : 0;
	if (!(elem->input = (char**)malloc(sizeof(char*) * (wd + 1))))
		ft_exit_malloc();
	elem->input[wd] = NULL;
	fill_input_redir(s, elem, pos, wd);
}

static int	fill_operator(char **s, int x, t_ast *elem, int *i)
{
	int			z;
	size_t		l;
	static char	*ope[16] = {">", ">>", ">>&", "<", "<<<", "<>", "<<",
	"&>", "&>>", ">&", "<&-", ">&-", "&", "|", "&&", "||"};

	z = -1;
	l = 0;
	while (s[*i][x + l] && (ft_isoperator(s[*i][x + l]) == 1
	|| s[*i][x + l] == '-'))
		l += 1;
	while (z++ < 16)
		if (ft_strlen(ope[z]) == l && ft_strncmp(&s[*i][x], ope[z], l) == 0)
			break ;
	if (z < 6)
		elem->type = REDIR;
	else if (z == 6)
		elem->type = HEREDOC;
	else if (z < 12)
		elem->type = AGREG;
	else if (z < 14)
		elem->type = OPERATOR;
	else
		elem->type = LOGIC;
	split_redir(s, elem, i);
	return (1);
}

static int	go_end_quote(char **s, int i, int *x)
{
	char	c;

	c = s[i][*x];
	*x += 1;
	while (s[i][*x] && s[i][*x] != c)
		*x += 1;
	*x += 1;
	return (1);
}

void		fill_ast(char **s, t_ast **lst, int save, int i)
{
	t_ast	*tmp;
	int		x;

	tmp = create_new_elem(lst);
	while (s[++i])
	{
		x = -1;
		while (s[i][++x])
			if (ft_isoperator(s[i][0]) == 1 || (ft_isoperator(s[i][x]) == 1
			&& x - 1 >= 0 && ft_isdigit(s[i][x - 1]) == 1))
			{
				(tmp->type >= OPERATOR) ? tmp = add_new_elem(lst) : 0;
				(tmp->type > CMD) ? add_input_prev_cmd(s, i, save, tmp) :
				fill_input(s, i, save, tmp);
				(tmp->type != NO_TYPE) ? tmp = add_new_elem(lst) : 0;
				save = i + fill_operator(s, x, tmp, &i) + ((tmp->type > AGREG)
				? 0 : 1);
				break ;
			}
			else if (ft_isquote(s[i][x]) == 1 && go_end_quote(s, i, &x) == 1)
				break ;
	}
	(tmp->type >= OPERATOR && ft_strcmp(tmp->input[0], "&") != 0) ?
	tmp = add_new_elem(lst) : 0;
	fill_last_elem(s, i, save, tmp);
}
