#include <stdlib.h>
#include "libft.h"
#include "error.h"

void	ft_insert(char **source, char *insert, int pos1, int pos2)
{
	char	*begin;
	char	*end;
	char	*tmp;

	begin = ft_strsub(*source, 0, pos1);
	end = ft_strsub(*source, pos2 + 1, (ft_strlen(&((*source)[pos2]) - 1)));
	ft_memdel((void **)source);
	if (!(tmp = ft_strjoin(begin, insert)))
		ft_exit_malloc();
	if (!(*source = ft_strjoin(tmp, end)))
		ft_exit_malloc();
	ft_memdel((void **)&begin);
	ft_memdel((void **)&tmp);
	ft_memdel((void **)&end);
}

void	ft_delete_inside(char **source, int start, int len)
{
	char	*begin;
	char	*end;

	begin = ft_strsub(*source, 0, start);
	end = ft_strsub(*source, start + len, ft_strlen(&(*source)[len + start]));
	free(*source);
	if (!(*source = ft_strjoin(begin, end)))
		ft_exit_malloc();
	free(begin);
	free(end);
}
