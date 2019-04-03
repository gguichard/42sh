#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "vars.h"
#include "cmdline.h"

static char		*shorten_home(char *str, t_list *vars)
{
	const char	*home;
	size_t		home_len;

	home = get_var_value(vars, "HOME");
	home_len = ft_strlen(home);
	if (home[home_len - 1] == '/')
		--home_len;
	if (home_len > 0 && ft_strnequ(str, home, home_len)
			&& (str[home_len] == '/' || str[home_len] == '\0'))
	{
		ft_memmove(str + 1, str + home_len, ft_strlen(str) - home_len + 1);
		str[0] = '~';
	}
	return (str);
}

static char		*get_replacement(char opt, t_list *vars)
{
	if (opt == 'w')
		return (shorten_home(ft_strdup(get_var_value(vars, "PWD")), vars));
	else if (opt == 'u')
		return (ft_strdup(getlogin()));
	else if (opt == 'a')
		return (ft_strdup("\a"));
	else
		return (NULL);
}

static int		escape_char_at_inside_and_move(char **str, size_t *idx
		, t_list *vars)
{
	char	*replacement;

	if ((*str)[*idx] == '\\')
	{
		if ((replacement = get_replacement((*str)[*idx + 1], vars)) != NULL)
		{
			if (!ft_strreplace_inside(str, *idx, 2, replacement))
			{
				free(replacement);
				return (0);
			}
			*idx += ft_strlen(replacement);
			free(replacement);
		}
		else
		{
			ft_memmove((*str + *idx), (*str + *idx + 1)
					, ft_strlen(*str + *idx));
			++(*idx);
		}
	}
	else
		++(*idx);
	return (1);
}

char			*create_prompt(t_list *vars)
{
	char	*ps1;
	size_t	idx;

	if ((ps1 = ft_strdup(get_var_value(vars, "PS1"))) == NULL)
		return (0);
	idx = 0;
	while (ps1[idx] != '\0')
	{
		if (!escape_char_at_inside_and_move(&ps1, &idx, vars))
			return (NULL);
	}
	return (ps1);
}
