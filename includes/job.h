#ifndef JOB_H
# define JOB_H

# include <unistd.h>
# include "libft.h"
# include "shell.h"
# include "options.h"

typedef enum			e_job_state
{
	RUNNING_FG,
	RUNNING_BG,
	STOPPED,
	STOPPED_PENDING,
	SIG,
	DONE
}						t_job_state;


typedef struct			s_job
{
	char				*cmd;
	t_list				*pipe;
	pid_t				pid;
	pid_t				gpid;
	int					status;
	t_job_state			state;
}						t_job;

/*
********************************** JOB CONTROL ************************************
*/

t_list		*add_pid_lst(pid_t process, t_ast *elem, int addpipe);
int			add_pid_lst_pipe(t_list *attach, pid_t process, t_ast *elem
		, int addpipe);

int			ret_status(int ret, pid_t process, t_job *job);

void		redirect_term_controller(pid_t new_controler, int type);

t_job		*get_job_pid(pid_t process);

void		refresh_jobs(void);
void		print_refreshed_jobs(t_list *tmp, int print, int stop_print
		, int index);
void		print_job(pid_t process, int after_signal);
void		display_simple_job(t_list *tmp, int index, t_opts *opts);
void		print_bg(pid_t process);

void		delete_jobs_terminated(t_list *tmp);
void		terminate_all_jobs(void);

char		*sig_str(int status);
char		*last_sig_process(t_list *tmp);
int			last_pid_exit_status(t_job *job);
char		*status_stop_str(int status);
char		*signal_stop_str(t_list *tmp);
char		*job_state_str(t_list *tmp);
char		*single_job_state_str(t_job *job);
char		*job_cmd(t_job *job);

t_job		*check_job_state(t_list *tmp, t_job_state state);
int			job_state_done(t_list *tmp);
int			job_state_run_or_done(t_list *tmp);

#endif
