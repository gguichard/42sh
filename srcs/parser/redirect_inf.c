#include <limits.h>
#include <stdlib.h>
#include "libft.h"
#include "split_cmd_token.h"
#include "redirect_inf.h"

/*
** Retourne 1 si le token etait bien un LOPT, 0 sinon.
*/

static int		process_lopt_redirect(t_redirect_inf *new_red
		, const char *token)
{
	long int	tmp_fd;
	char		*endptr;

	if (token[0] == '\0' || ft_strchr(RED_TYPE_SPE_CHARS, token[0]) != NULL)
	{
		new_red->lopt_fd = FD_DEFAULT;
		return (0);
	}
	else if (ft_strequ(token, "&"))
	{
		new_red->lopt_fd = FD_AMPERSAND;
		return (1);
	}
	else
	{
		if ((tmp_fd = ft_strtol(token, &endptr, 10)) > INT_MAX
				|| *endptr != '\0')
			new_red->lopt_fd = FD_ERROR;
		else
			new_red->lopt_fd = tmp_fd;
		return (1);
	}
}

static void		process_ope_redirect(t_redirect_inf *new_red, const char *token
		, int *ended_with_ampersand)
{
	if (ft_strequ(token, "<") || ft_strequ(token, "<&"))
	{
		new_red->red_type = RD_L;
		*ended_with_ampersand = (token[1] == '&');
	}
	else if (ft_strequ(token, ">") || ft_strequ(token, ">&"))
	{
		new_red->red_type = RD_R;
		*ended_with_ampersand = (token[1] == '&');
	}
	else if (ft_strequ(token, "<<"))
		new_red->red_type = RD_LL;
	else if (ft_strequ(token, ">>"))
		new_red->red_type = RD_RR;
	else
		new_red->red_type = RD_ERROR;
}

/*
** Retourne 1 si tout est ok, 0 si une erreur (malloc) a eu lieu.
*/

static int		process_ropt_redirect(t_redirect_inf *new_red
		, const char *token, int ope_ended_with_ampersand)
{
	long int	tmp_fd;
	char		*endptr;

	if (ope_ended_with_ampersand)
	{
		tmp_fd = ft_strtol(token, &endptr, 10);
		if (endptr[0] == '\0' || (endptr[0] == '-' && endptr[1] == '\0'))
		{
			new_red->ropt_file = NULL;
			if (endptr == token)
				new_red->ropt_fd = FD_DEFAULT;
			else if (tmp_fd > INT_MAX)
				new_red->ropt_fd = FD_ERROR;
			else
				new_red->ropt_fd = tmp_fd;
			new_red->close_ropt_fd = (endptr[0] == '-');
			return (1);
		}
		else
			new_red->ropt_fd = FD_AMPERSAND;
	}
	else
		new_red->ropt_fd = FD_NOTSET;
	new_red->close_ropt_fd = 0;
	return ((new_red->ropt_file = ft_strdup(token)) != NULL);
}

void			clean_redirect(t_redirect_inf *red_inf)
{
	free(red_inf->ropt_file);
}

int				fill_redirect_inf(t_redirect_inf *red_inf
		, char **token_tab)
{
	size_t	idx;
	int		ended_with_ampersand;

	if (token_tab == NULL || token_tab[0] == NULL || token_tab[1] == NULL)
		return (0);
	idx = 0;
	if (process_lopt_redirect(red_inf, token_tab[idx]))
		++idx;
	ended_with_ampersand = 0;
	process_ope_redirect(red_inf, token_tab[idx], &ended_with_ampersand);
	++idx;
	if (token_tab[idx] == NULL || token_tab[idx + 1] != NULL)
		return (0);
	if (!process_ropt_redirect(red_inf, token_tab[idx], ended_with_ampersand))
		return (0);
	return (1);
}
