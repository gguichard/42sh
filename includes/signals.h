/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:01:02 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/09 11:44:12 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "shell.h"

/*
*********************************** SIGNALS FUNC *******************************
*/

void	set_signals_handlers(int is_interactive);
void	sig_set_all(void (*handler)(int));
void	sig_reset(void);
void	set_sigmask(int type);
void	sigs_wait_line(t_alloc *alloc);
void	action_sigs(struct sigaction *act);
void	print_nl_sigint(t_list *tmp);

#endif
