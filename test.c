#include <signal.h>

void	test(int sig)
{
	sig = 1;
}

int main()
{
	signal(SIGTSTP, test);
	sleep(2);
}
