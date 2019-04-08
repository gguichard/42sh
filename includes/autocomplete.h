/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:02:04 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 15:48:57 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOCOMPLETE_H
# define AUTOCOMPLETE_H

# include <dirent.h>
# include <sys/stat.h>
# include "libft.h"
# include "shell.h"
# include "str_cmd_inf.h"
# include "token_inf.h"

typedef struct s_hashtable	t_hashtable;
typedef struct s_builtin	t_builtin;

typedef enum		e_dir_type
{
	DTYPE_NOT_A_DIR,
	DTYPE_MAY_BE_DIR,
	DTYPE_IS_A_DIR
}					t_dir_type;

typedef enum		e_ac_suff_type
{
	ACS_TYPE_NOTHING,
	ACS_TYPE_DIR,
	ACS_TYPE_VAR_IN_BRACKETS,
	ACS_TYPE_FILE
}					t_ac_suff_type;

typedef struct		s_ac_rdir_inf
{
	DIR				*dir;
	char			*dir_to_use;
	char			*file_word;
	char			*cur_file_path;
	const char		*cur_file_name;
	size_t			file_word_len;
	struct stat		stat_buf;
	int				need_to_be_cmd;
	t_dir_type		dir_type;
	int				force_exec_type;
}					t_ac_rdir_inf;

typedef struct		s_ac_suff_inf
{
	t_list			*choices;
	t_ac_suff_type	suff_type;
	char			*suff;
	long long		suff_len;
}					t_ac_suff_inf;

/*
** PUBLIC
*/

/*
** Free le contenu de l'ac_suff_inf passe en parametre puis free la structure
** en elle-meme.
** Renvoie toujours NULL.
*/
void				*delete_ac_suff_inf(t_ac_suff_inf *acs);

t_ac_suff_inf		*autocomplete_cmdline(t_str_cmd_inf *scmd, t_alloc *alloc);

/*
** PRIVATE
*/

const char			*find_last_var_start(const char *str, int *is_in_bracket);

const char			*find_last_assign_start(const char *str);

const char			*find_last_home_user(const char *str);

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter le word
** passe en parametre, si is_a_cmd est vrai le suffix ne peut etre qu'un
** executable ou un dossier.
*/
void				autocomplete_from_wordpath(const char *word, int is_a_cmd
		, t_dir_type dir_type, t_ac_suff_inf *acs);

/*
** Alloue et retourne une string contenant la concatenation de path + / + file.
*/
char				*build_path_to_file(const char *path, const char *file);

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter une
** commande builtin, alias, ou se trouvant dans le path.
*/
void				autocomplete_cmd(const char *word, char **path_tab
		, t_alloc *alloc, t_ac_suff_inf *acs);

/*
** Renvoie le nombre de premiers caracteres identiques entre les deux strings.
*/
size_t				count_same_char(const char *str1, const char *str2);

int					strlist_insert_sort(t_list **lst, t_list *elem);

/*
** Remplie cur_tk_cmd avec les infos du dernier token de la commande. La
** variable cur_tk_cmd->token pourra valoir null si vide.
*/
void				set_cur_token_cmd(t_token_inf *cur_tk_cmd
		, t_str_cmd_inf *scmd, t_alloc *alloc);

void				check_for_var_ac(const char *word, t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs, t_list *var_lst);

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter une
** commande builtin.
*/
void				check_for_builtin_ac(const char *word, t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs, const t_builtin *builtin_tab);

void				check_for_alias_ac(const char *word, t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs, t_hashtable *aliastable);

void				check_for_user_ac(const char *word, t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs);

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter depuis
** les informations de fichier contenues dans le  t_ac_rdir_inf.
** Si force_file_type est vrai et que l'autocompletion a un type, ce type sera
** force en tant que fichier.
** Retourne 0 en cas d'erreur, 1 sinon.
*/
int					build_ac_suff(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs
		, int force_file_type);

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter depuis
** les informations de fichier contenues dans le  t_ac_rdir_inf.
** Retourne 0 si l'autocompletion est vide (aucun suffix et aucun type).
** Retourne 1 sinon.
*/
int					try_ac_for_this_file(t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs);

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter depuis
** les informations de dossier contenues dans le  t_ac_rdir_inf.
*/
void				autocomplete_with_infs(t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs);

/*
** Initialise le t_ac_suff_inf passe en parametre avec les valeurs par defaut.
** Retourne 1 si l'initialisation reussi, 0 si elle rate.
*/
int					init_ac_suff_inf(t_ac_suff_inf *acs);

/*
** Malloc et retourne une structure contenant une chaine de caractere a ajouter
** a word pour arriver au plus grand prefix commun des mots possibles, une
** chaine vide s'il n'y en a pas et NULL s'il y a eu une erreur.
** La variable suff_type contient le type du suffix, un dossier ou un
** fichier (pour l'ajout d'un slash ou d'un espace final).
** Le parametre is_a_cmd doit valoir true si le word doit etre un executable,
** false s'il peut etre un fichier quelconque.
*/
t_ac_suff_inf		*autocomplete_word(t_list *var_lst, const char *word
		, int is_a_cmd, t_alloc *alloc);

t_ac_suff_inf		*autocomplete_var(t_list *var_lst, const char *word);

t_ac_suff_inf		*autocomplete_user(const char *word);

/*
** Retourne 1 si le fichier present dans le t_ac_rdir_inf peut etre une
** autocompletion valide, 0 sinon.
*/
int					valid_file_for_ac(t_ac_rdir_inf *acrd);

/*
** Alloue et retourne un nouvel element representant un choix valide pour
** l'autocompletion.
*/
t_list				*make_new_choice(t_ac_rdir_inf *acrd);

/*
** Remplie les inforamtions du prochain fichier a lire present dans le dossier
** contenu dans le t_ac_rdir_inf. Retourne 1 si un fichier a ete trouve et 0
** sinon. Met le suffix du t_ac_suff_inf a NULL s'il y a eu une erreur.
*/
int					readdir_to_dirent(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs);

/*
** Initialise le contenu du t_ac_rdir_inf avec les informations passes en
** parametre. Retourne 0 si l'initialisation rate, 1 si elle reussi.
*/
int					init_ac_rdir(const char *word, t_ac_rdir_inf *acrd
		, int need_to_be_cmd, t_dir_type dir_type);

/*
** Supprime le contenu du t_ac_rdir_inf.
*/
void				delete_ac_rdir(t_ac_rdir_inf *acrd);

#endif
