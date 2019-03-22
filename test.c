#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
	int		x = 0;
	int		child;

	if (!(child = fork()))
		exit(127000);
	printf("1 %d\n", x);
	waitpid(child, &x, 0);
	printf("2 %d\n", WEXITSTATUS(x));
	x = 0;
	printf("3 %d\n", x);
	waitpid(child, &x, 0);
	printf("4 %d\n", WEXITSTATUS(x));
}
