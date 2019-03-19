#include <unistd.h>

int main()
{
	while (1)
	{
		write(1, "sleep\n", 6);
		sleep(3);
	}
}
