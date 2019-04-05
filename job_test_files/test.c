#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	sleep(5);
	kill(getpid(), SIGSEGV);
}
