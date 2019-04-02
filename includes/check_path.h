#ifndef CHECK_PATH_H
# define CHECK_PATH_H

# include "error.h"

# define FT_FILE	1
# define FT_DIR		2
# define FT_ANY		3

/*
** Retourne ERRC_NOERROR (qui vaut 0) si le chemin pointe vers
** un dossier valide pour cd et a les droits demandes, ou
** un code d'erreur sinon.
*/
t_error		check_dir_rights(const char *path, int rights);

/*
** Retourne ERRC_NOERROR (qui vaut 0) si le dossier du fichier pointe par
** le chemin est valide pour cd et a les droits demandes, ou
** un code d'erreur sinon.
*/
t_error		check_dir_of_file_rights(const char *path, int rights);

/*
** Retourne ERRC_NOERROR (qui vaut 0) si le chemin pointe vers
** un fichier qui a les droits demandes et correspond au type de file_type, ou
** un code d'erreur sinon.
*/
t_error		check_file_rights(const char *path, int file_type, int rights);

/*
** Alloue et retourne le chemin du fichier avec les droits rights s'il est
** present dans  le PATH, sinon retourne NULL. Set dans tous les cas error
** avec l'erreur rencontree (ERRC_NOERROR s'il n'y a pas eu d'erreur).
** Un free du retour est necessaire.
*/
char		*search_in_path(t_list *vats, const char *file_name, int rights
		, t_error *error);

#endif
