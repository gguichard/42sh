#include "shell.h"

void	redirect_term_controller(pid_t new_controler, int type)
{
	struct sigaction	act;

	if (type == 0)
		tcsetpgrp(STDIN_FILENO, getpgid(new_controler));
	else if (type == 1)
	{
		act.sa_handler = SIG_IGN;
		act.sa_flags = SA_RESETHAND;
		sigaction(SIGTTOU, &act, 0);
		tcsetpgrp(STDIN_FILENO, getpgid(0));
	}
}
