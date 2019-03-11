/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 09:14:11 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/29 13:09:25 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vars.h"
#include "shell.h"
#include "str_cmd_inf.h"
#include "expand_vars.h"

static int		process_var_bracket_stat_changed(t_expand_inf *einf
		, size_t *old_pos, char **var_error, t_shell *shell)
{
	size_t	var_in_bracket_len;

	if (einf->scmd.is_in_var_bracket)
		*old_pos = einf->scmd.pos;
	else
	{
		var_in_bracket_len = einf->scmd.pos - 1 - *old_pos;
		einf->scmd.pos = *old_pos - 2;
		if (get_var_name_len(einf->scmd.str + *old_pos) != var_in_bracket_len
				|| var_in_bracket_len == 0)
		{
			*var_error = ft_strsub(einf->scmd.str, einf->scmd.pos
					, var_in_bracket_len + 3);
			return (0);
		}
		if (!replace_var_by_value(&(einf->scmd), &(einf->str)
					, var_in_bracket_len, shell))
			return (0);
	}
	return (1);
}

static int		process_var_bracket_stat_unchanged(t_expand_inf *einf
		, t_shell *shell)
{
	size_t			var_name_len;

	if (einf->scmd.str[einf->scmd.pos] == '$'
			&& einf->scmd.str[einf->scmd.pos + 1] != '{'
			&& !scmd_cur_char_is_escaped(&(einf->scmd)))
	{
		if ((var_name_len = get_var_name_len(einf->scmd.str
						+ einf->scmd.pos + 1)) > 0)
		{
			if (!replace_var_by_value(&(einf->scmd), &(einf->str), var_name_len
						, shell))
				return (0);
		}
	}
	return (1);
}

static int		process_var_bracket(t_expand_inf *einf, int stat_changed
		, char **var_error, t_shell *shell)
{
	if (stat_changed)
	{
		if (!process_var_bracket_stat_changed(einf, &(einf->old_pos), var_error
					, shell))
			return (0);
	}
	else if (!einf->scmd.is_in_quote && !einf->scmd.is_in_var_bracket)
	{
		if (!process_var_bracket_stat_unchanged(einf, shell))
			return (0);
	}
	return (1);
}

char			*expand_vars(const char *str, t_shell *shell, char **var_error)
{
	t_expand_inf	einf;
	int				old_is_in_var_bracket;

	*var_error = NULL;
	if ((einf.str = ft_strdup(str)) == NULL)
		return (NULL);
	scmd_init(&(einf.scmd), einf.str);
	einf.old_pos = 0;
	old_is_in_var_bracket = 0;
	while (1)
	{
		if (!process_var_bracket(&einf
					, einf.scmd.is_in_var_bracket != old_is_in_var_bracket
					, var_error, shell))
		{
			scmd_delete(einf.scmd.sub_var_bracket);
			return (ft_memdel((void**)&(einf.str)));
		}
		old_is_in_var_bracket = einf.scmd.is_in_var_bracket;
		if (einf.scmd.str[einf.scmd.pos] == '\0')
			break ;
		scmd_move_to_next_char(&(einf.scmd));
	}
	scmd_delete(einf.scmd.sub_var_bracket);
	return (einf.str);
}

char			*expand_home(const char *str, t_shell *shell
		, int remove_home_ending_slash)
{
	char	*real_home;
	char	*home;
	size_t	rhome_len;

	if (str[0] == '~' && (str[1] == '/' || str[1] == '\0'))
	{
		home = get_shell_var(shell, "HOME");
		if (home == NULL
				|| (real_home = escape_chars_in_var(NULL, home)) == NULL)
		{
			free(home);
			return (NULL);
		}
		free(home);
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
