#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

void	handle(int sig)
{
	int i = 0;

	while (i < 100000)
		printf("HANDLER %d\n", i++);
}

int main(int ac, char **av)
{
	int i = 0;

	signal(SIGINT, handle);
	while (i < 10000000)
		printf("MAIN %d\n", i++);
}
