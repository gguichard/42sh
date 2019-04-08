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

	if (str == NULL)
		return (NULL);
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

static char		*get_login(t_list *vars)
{
	const char	*value;

	value = get_var_value_or_null(vars, "LOGNAME");
	if (value == NULL)
		value = getlogin();
	if (value == NULL)
		return (NULL);
	return (ft_strdup(value));
}

char			*get_replacement(char opt, t_list *vars)
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
		return (get_login(vars));
	else if (opt == 'W')
		return (shorten_home(ft_strdup(get_var_value(vars, "PWD")), 1, vars));
	else if (opt == 'w')
		return (shorten_home(ft_strdup(get_var_value(vars, "PWD")), 0, vars));
	else
		return (NULL);
}
