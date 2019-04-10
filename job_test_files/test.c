#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

void	handle(int sig)
{
	printf("HANDLER %d\n", sig);
}

int main(int ac, char **av)
{
	int i = 1;

	while (i < 32)
	{
		if (i == 18)
			i += 1;
		signal(i++, handle);
	}
	sleep(30);
}
