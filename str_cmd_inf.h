#ifndef STR_CMD_INF_H
# define STR_CMD_INF_H

# include <stdlib.h>

# define DOUBLEQUOTE_SPE_CHAR "$`\"\\\n"
# define BACKQUOTE_SPE_CHAR "$`\\"

typedef enum	e_str_cmd_err
{
	SCMDERR_NOERROR = 0,
	SCMDERR_ENDTOOSOON,
	SCMDERR_BADCHAR
}				t_str_cmd_err;

typedef struct	s_str_cmd_inf
{
	int						is_in_quote;
	int						is_in_doublequote;
	int						is_in_backquote;
	int						is_in_var_bracket;
	size_t					pos;
	const char				*str;
	struct s_str_cmd_inf	*sub_var_bracket;
}				t_str_cmd_inf;

/*
** Initialise le str_cmd_inf en mettant les valeurs a 0.
*/
void			scmd_init(t_str_cmd_inf *str_cmd_inf, const char *str);

void			scmd_delete(t_str_cmd_inf *str_cmd_inf);

/*
** Retourne 1 si le curseur a la position actuelle n'est dans aucun enclosing
** char (",',{), 0 sinon.
*/
int				scmd_cur_char_is_in_nothing(t_str_cmd_inf *str_cmd_inf);

/*
** Retourne 1 si le curseur a la position actuelle est echappe, 0 sinon.
*/
int				scmd_cur_char_is_escaped(t_str_cmd_inf *str_cmd_inf);

/*
** Retourne 1 si le curseur a la position passee en parametre est echappe,
** 0 sinon.
*/
int				scmd_char_at_is_escaped(t_str_cmd_inf *str_cmd_inf
		, size_t at_pos);

/*
** Deplace le curseur vers la droite en modifiant si necessaire l'etat de
** str_cmd_inf (dans une quote etc).
*/
int				scmd_move_to_next_char(t_str_cmd_inf *str_cmd_inf);

#endif
