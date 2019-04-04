#include <unistd.h>
#include "get_next_line.h"
#include "shell.h"
#include "cmdline.h"

t_rstate	non_interact_input(t_cmdline *cmdline)
{
	t_rstate	state;
	int			ret;

	ret = 0;
	ret = get_next_line(STDIN_FILENO, &cmdline->input_str);
	if (ret <= 0)
		state = RSTATE_EOT;
	else
		state = RSTATE_END;
	return (state);
}
