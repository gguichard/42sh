#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"

static size_t	len_sub_parts(int idx, char **argv)
{
	size_t	len;

	len = 0;
	while (argv[idx] != NULL)
	{
		len += ft_strlen(argv[idx]) + 1;
		idx++;
	}
	return (len);
}

static char		*concat_args(int print_nl, int idx, char **argv)
{
	size_t	total_len;
	char	*tmp;
	size_t	offset;
	size_t	len;

	total_len = len_sub_parts(idx, argv);
	tmp = (char *)malloc((total_len + print_nl + 1) * sizeof(char));
	if (tmp == NULL)
		return (NULL);
	offset = 0;
	while (argv[idx] != NULL)
	{
		len = ft_strlen(argv[idx]);
		ft_memcpy(tmp + offset, argv[idx], len);
		offset += len;
		if (argv[++idx] != NULL)
		{
			tmp[offset++] = ' ';
			len++;
		}
	}
	if (print_nl)
		tmp[offset++] = '\n';
	tmp[offset] = '\0';
	return (tmp);
}

int				builtin_echo(t_ast *elem, t_alloc *alloc)
{
	int		idx;
	int		print_nl;
	int		ret;
	char	*tmp;

	(void)alloc;
	idx = 1;
	print_nl = !(elem->input[1] != NULL && ft_strequ("-n", elem->input[1]));
	if (!print_nl)
		idx++;
	ret = 0;
	if ((tmp = concat_args(print_nl, idx, elem->input)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, "42sh: echo: unexpected error\n");
		return (2);
	}
	else if (write(STDOUT_FILENO, tmp, ft_strlen(tmp)) == -1)
	{
		ret = 1;
		ft_dprintf(STDERR_FILENO, "42sh: echo: write error\n");
	}
	free(tmp);
	return (ret);
}
