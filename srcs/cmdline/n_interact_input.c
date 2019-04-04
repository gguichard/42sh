#include <unistd.h>
#include "get_next_line.h"
#include "cmdline.h"

char	*non_interact_input(t_cmdline *cmdline, t_rstate *state)
{
	char	*str;
	int		ret;

	(void)cmdline;
	ret = get_next_line(STDIN_FILENO, &str);
	*state = (ret <= 0 ? RSTATE_EOT : RSTATE_END);
	return (str);
}
