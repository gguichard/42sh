#ifndef ERROR_H
# define ERROR_H

# include "shell.h"

typedef enum	e_error
{
	ERRC_NOERROR = 0,
	ERRC_UNEXPECTED = 1,
	ERRC_FILENOTFOUND,
	ERRC_CMDNOTFOUND,
	ERRC_NOTADIR,
	ERRC_ISADIR,
	ERRC_NONEEDEDRIGHTS,
	ERRC_LEXERROR,
	ERRC_INCOMPLETECMD
}				t_error;

int		ft_error_parse_redir(char **input);
int		check_error_lst(t_ast *lst);
int		ft_error_splitshell(void);
int		ft_error_redir_format(char *ope, int len);
void	ft_exit_malloc(void);

/*
** Free la variable var et retourne err.
** Fonction utilitaire ayant pour but de reduire le nombre de ligne.
*/
t_error		del_then_ret_err(void *var, t_error err);

const char	*error_to_str(t_error error);

#endif
