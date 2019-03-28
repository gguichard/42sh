#ifndef SPLIT_CMD_TOKEN_H
# define SPLIT_CMD_TOKEN_H

# include "libft.h"
# include "hashtable.h"
# include "token_inf.h"
# include "str_cmd_inf.h"

# define WORD_SEP_CHARS " \t\n"
# define CMD_SPE_CHARS "|<>&;"
# define DOUBLED_SPE_CHARS "|<>&"
# define RED_TYPE_SPE_CHARS "<>"

typedef struct	s_split_cmd_inf
{
	char			disable_last_alias_expand;
	t_hashtable		*aliastable;
	t_list			*forbidden_aliases;
	size_t			alias_recur_lvl;
	t_list			*tk_lst;
	t_list			*last_start_cmd;
	t_list			*last_tk_added;
	int				pos_alias_can_start;
	char			last_alias_ended_with_blank;
	char			alias_has_expanded;
	t_str_cmd_inf	*scmd;
	char			last_char_was_spe;
	const char		*tk_start;
	t_token_type	cur_tk_type;
	char			last_tk_end_by_and;
}				t_split_cmd_inf;

t_token_type	get_tk_type_before_cur_char(t_split_cmd_inf *sp_cmd);

t_token_type	get_next_word_tk_type(t_split_cmd_inf *sp_cmd);

int				get_cur_spe_char_token_len_and_set_type(
		t_split_cmd_inf *sp_cmd);

int				cur_token_is_number(t_split_cmd_inf *sp_cmd);

int				is_a_spe_char(char c);

int				add_cur_token_to_lst(t_split_cmd_inf *sp_cmd);

int				add_whole_token_to_lst(t_split_cmd_inf *sp_cmd
		, const char *token_str, t_token_type token_type);

t_list			*split_cmd_token_with_lst(t_str_cmd_inf *str_cmd_inf
		, t_hashtable *aliastable, t_list *cur_forbidden_aliases,
		size_t cur_alias_recur_lvl);

t_list			*split_cmd_token_without_last_alias(t_str_cmd_inf *str_cmd_inf
		, t_hashtable *aliastable);

/*
** Retourne la liste des tokens de la commande passee en parametre. Retourne
** NULL en cas d'erreur de malloc.
*/
t_list			*split_cmd_token(t_str_cmd_inf *str_cmd_inf
		, t_hashtable *aliastable);

#endif
