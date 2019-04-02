#include <unistd.h>
#include <signal.h>
#include "shell.h"
#include "job.h"

void	redirect_term_controller(pid_t new_controller, int type)
{
	if (type == 0)
		tcsetpgrp(STDIN_FILENO, getpgid(new_controller));
	else if (type == 1)
		tcsetpgrp(STDIN_FILENO, getpgid(0));
}
