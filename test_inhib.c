#include "expand.h"
#include "shell.h"
#include "inhibitor.h"
#include "expand.h"
#include "str_cmd_inf.h"
#include "stdio.h"

int main(int argc, char **argv)
{
	char **array;
	int	i;

	array = NULL;
	i = 0;

	printf("INPUT: %s\n", argv[1]);
	if (argv[1])
		array = inhib_expand_str(argv[1], NULL);
	else
		array = inhib_expand_str(NULL, NULL);
	while (array && array[i])
	{
		printf("arr[%d]: |%s|\n",i ,array[i]);
		i += 1;
	}

	return (0);
}
