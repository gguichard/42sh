#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "vars.h"
#include "cmdline.h"

static char		*get_hostname(int shorten_it)
{
	char	*hostname;
	long	hostname_max_len;
	char	*dot_pos;

	hostname_max_len = sysconf(_SC_HOST_NAME_MAX) + 1;
	if (hostname_max_len < 256)
		hostname_max_len = 256;
	if ((hostname = (char*)malloc(hostname_max_len)) == NULL)
		return (NULL);
	if (gethostname(hostname, hostname_max_len) < 0)
	{
		free(hostname);
		return (NULL);
	}
	if (shorten_it)
	{
		dot_pos = ft_strchr(hostname, '.');
		if (dot_pos != NULL)
			*dot_pos = '\0';
	}
	return (hostname);
}

static char		*shorten_home(char *str, int only_last_dir, t_list *vars)
{
	const char	*home;
	size_t		home_len;
	char		*last_slash_pos;

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
	if (only_last_dir)
	{
		last_slash_pos = ft_strrchr(str, '/');
		if (last_slash_pos != NULL)
			ft_memmove(str, last_slash_pos + 1, ft_strlen(last_slash_pos));
	}
	return (str);
}

static char		*get_replacement(char opt, t_list *vars)
{
	if (opt == 'a')
		return (ft_strdup("\a"));
	else if (opt == 'e')
		return (ft_strdup("\033"));
	else if (opt == 'H')
		return (get_hostname(0));
	else if (opt == 'h')
		return (get_hostname(1));
	else if (opt == 'u')
		return (ft_strdup(getlogin()));
	else if (opt == 'W')
		return (shorten_home(ft_strdup(get_var_value(vars, "PWD")), 1, vars));
	else if (opt == 'w')
		return (shorten_home(ft_strdup(get_var_value(vars, "PWD")), 0, vars));
	else
		return (NULL);
}

static size_t	count_no_printable_chars(const char *str)
{
	size_t	total;

	if (str == NULL)
		return (0);
	total = 0;
	while (*str != '\0')
	{
		if (!ft_isprint(*str))
			total++;
		str++;
	}
	return (total);
}

/*
** Retourne toujours 1.
*/

static int		remove_chars_from_str(char *str, size_t pos, size_t len)
{
	ft_memmove((str + pos), (str + pos + len), ft_strlen(str + pos + len) + 1);
	return (1);
}

static int		escape_char_at_inside_and_move(char **str, size_t *idx
		, t_list *vars, int *is_in_no_print)
{
	char	*replaced;
	int		nb_chars_readed;

	replaced = NULL;
	nb_chars_readed = 1;
	if ((*str)[*idx] == '\\')
	{
		if ((*str)[*idx + 1] == '[' && (*is_in_no_print = 1))
			return (!remove_chars_from_str(*str, *idx, 2));
		else if ((*str)[*idx + 1] == ']' && !(*is_in_no_print = 0))
			return (!remove_chars_from_str(*str, *idx, 2));
		else if ((replaced = get_replacement((*str)[*idx + 1], vars)) != NULL)
		{
			if (!ft_strreplace_inside(str, *idx, 2, replaced))
				return ((int)ft_memdel((void**)(&replaced)) - 1);
			nb_chars_readed = ft_strlen(replaced);
		}
		else
			remove_chars_from_str(*str, *idx, 1);
	}
	*idx += nb_chars_readed;
	nb_chars_readed -= count_no_printable_chars(replaced);
	free(replaced);
	return (*is_in_no_print ? 0 : nb_chars_readed);
}

char			*create_prompt(t_list *vars, size_t *tot_printable_char)
{
	char	*ps1;
	size_t	idx;
	int		is_in_no_print;
	int		nb_new_printed_char;

	if ((ps1 = ft_strdup(get_var_value(vars, "PS1"))) == NULL)
		return (NULL);
	idx = 0;
	is_in_no_print = 0;
	*tot_printable_char = 0;
	while (ps1[idx] != '\0')
	{
		if ((nb_new_printed_char = escape_char_at_inside_and_move(&ps1
						, &idx, vars, &is_in_no_print)) < 0)
			return (NULL);
		*tot_printable_char += nb_new_printed_char;
	}
	return (ps1);
}
