/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 09:50:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 13:37:47 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include "shell.h"

void		clear_history_entry(t_history *elem);
void		clear_history(t_shell *shell);
void		add_history_entry(t_shell *shell, const char *command);
const char	*peek_history_prev(t_shell *shell);
const char	*peek_history_next(t_shell *shell);

/*
** HOOKS.
*/
int			history_up(t_shell *shell, t_term *term);
int			history_down(t_shell *shell, t_term *term);
int			history_search(t_shell *shell, t_term *term);

#endif
