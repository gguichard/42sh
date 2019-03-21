#include <stdlib.h>
#include "error.h"

t_error		del_then_ret_err(void *var, t_error err)
{
	free(var);
	return (err);
}

const char	*error_to_str(t_error error)
{
	if (error == ERRC_FILENOTFOUND)
		return ("no such file or directory");
	else if (error == ERRC_CMDNOTFOUND)
		return ("command not found");
	else if (error == ERRC_NONEEDEDRIGHTS)
		return ("permission denied");
	else if (error == ERRC_ISADIR)
		return ("is a directory");
	else if (error == ERRC_NOTADIR)
		return ("not a directory");
	else
		return ("unknown error");
}
