/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token_cmd.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 14:09:00 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/28 17:35:25 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_TOKEN_CMD_H
# define JOIN_TOKEN_CMD_H

# include "libft.h"
# include "shell.h"
# include "cmd_inf.h"

char	*process_escape_arg(const char *arg, t_shell *shell);

int		add_arg(t_cmd_inf *cmd_inf, const char *arg, t_shell *shell);

t_list	*get_ropt_elem(t_list *tk_lst);

int		del_cur_cmd(t_cmd_inf *cmd_inf);

int		add_redirect_inf_to_cmd(t_cmd_inf *cmd_inf, int have_lopt
		, t_list *opt_lst_start, t_shell *shell);

/*
** Alloue et retourne une liste de commande representant la liste des tokens
** passee en parametre. Retourne NULL en cas d'erreur ou de liste vide.
*/
t_list	*join_token_cmd(t_list *token_lst, t_shell *shell);

/*
** Remplie le cmd_inf avec les infos de la prochaine commande dans token_lst,
** deplace aussi la tete de liste. Retourne 1 en cas de succes, 0 en cas
** d'echec.
*/
int		set_cur_cmd(t_cmd_inf *cmd_inf, t_list **token_lst, t_shell *shell);

#endif
