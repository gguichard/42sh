/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token_cmd_redirect.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 17:17:36 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/29 11:08:26 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include "libft.h"
#include "redirect_inf.h"
#include "token_inf.h"
#include "cmd_inf.h"
#include "join_token_cmd.h"

static void		process_lopt_redirect(t_redirect_inf *new_red
		, t_token_inf *tk_lopt)
{
	long int	tmp_fd;

	if (tk_lopt == NULL)
		new_red->from_fd = FD_DEFAULT;
	else if (ft_strequ(tk_lopt->token, "&"))
		new_red->from_fd = FD_AMPERSAND;
	else
	{
		if ((tmp_fd = ft_strtol(tk_lopt->token, NULL, 10)) > INT_MAX)
			new_red->from_fd = FD_ERROR;
		else
			new_red->from_fd = tmp_fd;
	}
}

static int		process_ropt_str_redirect(t_redirect_inf *new_red
		, t_token_inf *token_inf)
{
	long int		tmp_fd;
	char			*tmp_fd_end;

	new_red->to_word = NULL;
	if ((tmp_fd = ft_strtol(token_inf->token + 1, &tmp_fd_end, 10)) > INT_MAX)
		new_red->to_fd = FD_ERROR;
	else if (tmp_fd_end == (token_inf->token + 1))
		new_red->to_fd = FD_DEFAULT;
	else
		new_red->to_fd = tmp_fd;
	new_red->close_to_fd = (*tmp_fd_end == '-');
	return (1);
}

static int		process_ropt_redirect(t_redirect_inf *new_red
		, t_list *tkl_ropt, t_shell *shell)
{
	if (tkl_ropt == NULL)
		return (0);
	new_red->to_fd = FD_NOTSET;
	if (get_tk(tkl_ropt)->type == TK_STR_OPT
			&& ft_strequ(get_tk(tkl_ropt)->token, "&"))
	{
		new_red->to_fd = FD_AMPERSAND;
		if ((tkl_ropt = tkl_ropt->next) == NULL)
			return (0);
	}
	if (get_tk(tkl_ropt)->type == TK_STR_OPT)
		return (process_ropt_str_redirect(new_red, get_tk(tkl_ropt)));
	else
	{
		new_red->close_to_fd = 0;
		return ((new_red->to_word = process_escape_arg(get_tk(tkl_ropt)->token
						, shell)) != NULL);
	}
}

int				add_redirect_inf_to_cmd(t_cmd_inf *cmd_inf, int have_lopt
		, t_list *opt_lst_start, t_shell *shell)
{
	t_redirect_inf	new_red;
	t_list			*new_elem;

	if (opt_lst_start == NULL)
		return (0);
	process_lopt_redirect(&new_red, (have_lopt ? get_tk(opt_lst_start) : NULL));
	opt_lst_start = (have_lopt ? opt_lst_start->next : opt_lst_start);
	new_red.red_type = redirection_str_to_type(get_tk(opt_lst_start)->token);
	if (!process_ropt_redirect(&new_red, opt_lst_start->next, shell))
		return (0);
	new_red.heredoc = NULL;
	new_red.red_save = NULL;
	if ((new_elem = ft_lstnew(&new_red, sizeof(t_redirect_inf))) == NULL)
	{
		free(new_red.to_word);
		return (0);
	}
	ft_lstpush(&(cmd_inf->redirect_lst), new_elem);
	return (1);
}
