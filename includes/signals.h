#ifndef SIGNALS_H
# define SIGNALS_H

# include "shell.h"

/*
*********************************** SIGNALS FUNC **********************************
*/

void	set_signals_handlers(void);
void	sig_set_all(void(*handler)(int));
void	sig_reset(void);
void	set_sigmask(int type);
void	sigs_wait_line(t_alloc *alloc);
void	action_sigs(struct sigaction *act);
void	print_nl_sigint(t_list *tmp);

#endif
