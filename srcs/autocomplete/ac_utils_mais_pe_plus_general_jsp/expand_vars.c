#include "libft.h"
#include "shell.h"
#include "expand_vars.h"

char			*expand_home(const char *str, t_var *var_lst
		, int remove_home_ending_slash)
{
	char	*real_home;
	char	*home;
	size_t	rhome_len;

	if (str[0] == '~' && (str[1] == '/' || str[1] == '\0'))
	{
		home = get_env_value(var_lst, "HOME");
		if (home == NULL)
			return (NULL);
		//free(home);
		if (remove_home_ending_slash)
		{
			rhome_len = ft_strlen(real_home);
			if (rhome_len > 0 && real_home[rhome_len - 1] == '/')
				real_home[rhome_len - 1] = '\0';
		}
		return (ft_strjoin_free(real_home, str + 1));
	}
	return (ft_strdup(str));
}
