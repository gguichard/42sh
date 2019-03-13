#include "libft.h"
#include "shell.h"
#include "builtins.h"
#include "expand_vars.h"

char			*expand_home(const char *str, t_var *var_lst
		, int remove_home_ending_slash)
{
	char	*home;
	size_t	home_len;

	if (str[0] == '~' && (str[1] == '/' || str[1] == '\0'))
	{
		home = get_env_value(var_lst, "HOME");
		if (home == NULL)
			return (NULL);
		if (remove_home_ending_slash)
		{
			home_len = ft_strlen(home);
			if (home_len > 0 && home[home_len - 1] == '/')
				home[home_len - 1] = '\0';
		}
		return (ft_strjoin_free(home, str + 1));
	}
	return (ft_strdup(str));
}
