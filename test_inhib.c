#include "expand.h"
#include "shell.h"
#include "inhibitor.h"
#include "expand.h"
#include "str_cmd_inf.h"
#include <stdio.h>
#include <string.h>

// int main(int argc, char **argv)
// {
// 	char **array;
// 	int	i;
//
// 	array = NULL;
// 	i = 0;
//
// 	printf("INPUT: %s\n", argv[1]);
// 	if (argv[1])
// 		array = inhib_expand_str(argv[1], NULL);
// 	else
// 		array = inhib_expand_str(NULL, NULL);
// 	while (array && array[i])
// 	{
// 		printf("arr[%d]: |%s|\n",i ,array[i]);
// 		i += 1;
// 	}
//
// 	return (0);
// }

int	main(int argc, char **argv)
{
	char *dup;

	dup = NULL;
	if (!argv[1])
		inhib_only_str(NULL);
	else
	{
		dup = strdup(argv[1]);
		inhib_only_str(dup);
	}
	printf("STR: |%s|\n", dup);
	return (0);
}
