/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token_cmd_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 17:24:28 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/28 17:34:41 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "expand_vars.h"
#include "apply_escape.h"
#include "shell.h"
#include "token_inf.h"
#include "redirect_inf.h"
#include "join_token_cmd.h"

char	*process_escape_arg(const char *arg, t_shell *shell)
{
	char	*home_expanded_arg;
	char	*escaped_arg;

	if ((home_expanded_arg = expand_home(arg, shell, 0)) == NULL)
		return (NULL);
	if ((escaped_arg = apply_escape(home_expanded_arg)) == NULL)
	{
		free(home_expanded_arg);
		return (NULL);
	}
	free(home_expanded_arg);
	return (escaped_arg);
}

int		add_arg(t_cmd_inf *cmd_inf, const char *arg, t_shell *shell)
{
	t_list	*new_elem;
	char	*escaped_arg;

	if ((escaped_arg = process_escape_arg(arg, shell)) == NULL)
		return (0);
	if ((new_elem = ft_lstnew(escaped_arg
					, sizeof(char) * (ft_strlen(escaped_arg) + 1))) == NULL)
	{
		free(escaped_arg);
		return (0);
	}
	ft_lstpush(&(cmd_inf->arg_lst), new_elem);
	free(escaped_arg);
	return (1);
}

t_list	*get_ropt_elem(t_list *tk_lst)
{
	if (get_tk(tk_lst)->type == TK_STR_OPT
			&& ft_strequ(get_tk(tk_lst)->token, "&"))
		return (tk_lst->next);
	else
		return (tk_lst);
}

int		del_cur_cmd(t_cmd_inf *cmd_inf)
{
	ft_lstfree(&(cmd_inf->arg_lst));
	ft_lstdel(&(cmd_inf->redirect_lst), del_redirect);
	return (0);
}
