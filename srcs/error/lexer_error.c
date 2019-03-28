#include "shell.h"
#include "error.h"

int	check_error_lst(t_ast *lst)
{
	t_ast *tmp;

	tmp = lst;
	while (tmp)
	{
		if (tmp->type >= PIPE && tmp->next && tmp->next->type >= PIPE)
		{
			write(2, "21sh: syntax error near `", 25);
			write(2, tmp->next->input[0], ft_strlen(tmp->next->input[0]));
			write(2, "'\n", 2);
			return (1);
		}
		else
		{
			tmp->back = NULL;
			tmp = tmp->next;
		}
	}
	return (0);
}

int	ft_error_splitshell(void)
{
	write(2, "21sh: parse error near `;;'\n", 28);
	return (-1);
}

int	ft_error_redir_format(char *ope, int len)
{
	int			i;
	int			size;
	int			save;
	static char	*operator[11] = {">>", ">&", ">", "<<",
	"<&", "<", "&>", "&&", "&", "||", "|"};

	i = 0;
	save = -1;
	size = 1;
	while (i < 11)
	{
		if (ft_strncmp(operator[i], ope, size) == 0)
		{
			save = i;
			i = 0;
			size += 1;
		}
		i += 1;
	}
	write(2, "21sh: parse error near `", 24);
	write(2, &ope[size - 1], (len - size == 0) ? 1 : 2);
	write(2, "'\n", 2);
	return (-1);
}
