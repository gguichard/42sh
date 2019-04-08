#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("usage : cmd [signal number]\n");
		return (0);
	}
	kill(getpid(), atoi(av[1]));
}
