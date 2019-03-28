#ifndef STR_CMD_INF_H
# define STR_CMD_INF_H

# include <stdlib.h>

# define DBQUOTE_SPE_CHAR "$\"\\\n"
# define SUBCMD_SPE_CHAR "$\\)"
# define SCMD_TYPE_NOTHING 0
# define SCMD_TYPE_VAR 1
# define SCMD_TYPE_SUBCMD 2

typedef struct	s_str_cmd_inf
{
	int						is_in_quote;
	int						is_in_dbquote;
	size_t					pos;
	char					*str;
	struct s_str_cmd_inf	*sub_str_cmd;
	int						cur_str_cmd_type;
}				t_str_cmd_inf;

/*
** Initialise le str_cmd_inf en mettant les valeurs a 0 et en faisant une
** copie de str, fonctionne toujours si str vaut NULL. Retourne 0 en cas
** d'erreur et 1 en cas de succes.
*/
int				scmd_init(t_str_cmd_inf *str_cmd_inf, const char *str);

/*
** Free le contenu du str_cmd_inf passe en parametre et toutes ses sub_str.
*/
void			scmd_clean(t_str_cmd_inf *str_cmd_inf);

int				scmd_reset(t_str_cmd_inf *str_cmd_inf, const char *new_str);

/*
** Retourne le char actuel du str_cmd_inf.
*/
char			scmd_cur_char(t_str_cmd_inf *str_cmd_inf);

/*
** Retourne la string dont le 1er char est le char actuel de str_cmd_inf.
*/
const char		*scmd_cur_str(t_str_cmd_inf *str_cmd_inf);

int				scmd_char_at_is_of(t_str_cmd_inf *str_cmd_inf, size_t pos
		, const char *these);

/*
** Retourne 1 si le char actuel du str_cmd_inf fait parti de la liste passe
** en parametre, 0 sinon. Retourne toujours 0 si le char actuel est '\0'.
*/
int				scmd_cur_is_of(t_str_cmd_inf *str_cmd_inf, const char *these);

/*
** Retourne 1 si le curseur a la position actuelle n'est dans aucun enclosing
** char (",',${,$(), 0 sinon.
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
