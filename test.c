#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

void	test(int sig)
{
	sig = 1;
	write(1, "IN HANDLE\n", 10);
}

int main()
{
	int	proc;

	signal(SIGCHLD, test);
	proc = fork();
	if (!proc)
	{
		sleep(50);
		exit(0);
	}
	setpgid(proc, 0);
	tcsetpgrp(0, proc);
	waitpid(proc, 0, 0);
}
