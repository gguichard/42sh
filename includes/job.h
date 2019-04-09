/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:58:55 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/09 17:00:02 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_H
# define JOB_H

# include <unistd.h>
# include "libft.h"
# include "shell.h"
# include "execution.h"
# include "options.h"

typedef enum	e_job_state
{
	RUNNING_FG,
	RUNNING_BG,
	STOPPED,
	STOPPED_PENDING,
	SIG,
	DONE,
	SUB_CMD
}				t_job_state;

typedef struct	s_job
{
	char				*cmd;
	t_list				*pipe;
	pid_t				pid;
	pid_t				gpid;
	int					status;
	t_job_state			state;
}				t_job;

/*
********************************** JOB CONTROL ********************************
*/

t_list			*add_pid_lst(pid_t process, t_ast *elem, int addpipe);
int				add_pid_lst_pipe(t_list *attach, pid_t process, t_ast *elem
				, int addpipe);

int				ret_status(int ret, pid_t process, t_job *job, t_exec_opt *opt);

void			redirect_term_controller(pid_t new_controller, int type);

t_job			*get_job_pid(pid_t process);
t_job			*get_job_allpid(pid_t process);
int				get_job_id(const char *arg, t_list **process
				, const char *bltin);
int				job_id_nb(t_list **tmp, int value);
int				look_for_recent_job(t_list **tmp, int (*valid_job)(t_list **));
int				current_job_id(t_list **tmp);
int				previous_job_id(t_list **tmp);

void			refresh_state(t_list *tmp, int print_state);
void			refresh_jobs(void);
void			print_refreshed_jobs(t_list *tmp, int print, int stop_print
				, int index);
int				last_job_status(t_job *job);
void			display_simple_job(t_list *tmp, int index);
void			print_bg(pid_t process);
void			print_job(pid_t process, int after_signal);

void			delete_jobs_terminated(t_list *tmp);
void			terminate_all_jobs(int sig);
void			kill_zombie_boy(pid_t boy, int display_err);

char			*sig_str(int status);
char			*last_sig_process(t_list *tmp, int foreground);
t_job			*last_job(t_job *job);
char			*status_stop_str(int status);
char			*signal_stop_str(t_list *tmp);
char			*job_state_str(t_list *tmp);
char			*single_job_state_str(t_job *job);
char			*job_cmd(t_job *job);

void			update_job_state(t_list *tmp, t_job_state state);
t_job			*check_job_state(t_list *tmp, t_job_state state);
int				job_state_done(t_list *tmp);
int				job_state_run_or_done(t_list *tmp);
int				check_stopped_job(void);

#endif
