#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

void	test(int sig)
{
	sig = 1;
	printf("In Handler sig %d\n", sig);
	exit(0);
}

int main()
{
	int	proc;
	int	x = 1;

	while (x < 32)
		signal(x++, test);
	printf("Sleep mode\n");
	sleep(30);
}
