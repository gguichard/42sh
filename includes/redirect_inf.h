#ifndef REDIRECT_INF_H
# define REDIRECT_INF_H

# define FD_ERROR		-1
# define FD_DEFAULT		-2
# define FD_AMPERSAND	-3
# define FD_NOTSET		-4

typedef enum			e_redirect_type
{
	RD_L,
	RD_R,
	RD_LL,
	RD_RR,
	RD_ERROR
}						t_redirect_type;

typedef struct			s_redirect_inf
{
	int					lopt_fd;
	t_redirect_type		red_type;
	int					ropt_fd;
	int					close_ropt_fd;
	char				*ropt_file;
}						t_redirect_inf;

void					clean_redirect(t_redirect_inf *red_inf);

/*
** Retourne 1 si red_inf a ete rempli correctement, 0 sinon. S'il n'a pas ete
** rempli correctement il ne faut pas le delete, car risque de free un pointeur
** non initialise.
*/
int						fill_redirect_inf(t_redirect_inf *red_inf
		, char **token_tab);

#endif
