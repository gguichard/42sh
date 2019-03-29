#include <unistd.h>
#include <signal.h>
#include "shell.h"

void	redirect_term_controller(pid_t new_controler, int type)
{
	if (type == 0)
		tcsetpgrp(STDIN_FILENO, getpgid(new_controler));
	else if (type == 1)
	{
		signal(SIGTTOU, SIG_IGN);
		tcsetpgrp(STDIN_FILENO, getpgid(0));
		signal(SIGTTOU, SIG_DFL);
	}
}
