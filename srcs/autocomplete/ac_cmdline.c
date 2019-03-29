#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "split_cmd_token.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "autocomplete.h"

//TODO
//TODO GERER LE BACKSLASH EN FIN DE COMMANDE ?!@#$%^&*!?@>$%<^>&$#>&*!@?
//TODO
//TODO GERER LES INIBITEURS DE CHAR AVANT D'APPELER AUTOCOMPLETE
//TODO
//TODO EXPAND LES ENV VAR AVANT DE TENTER L'AUTOCOMPLETION (SAUF SI VAR ACTUELLE)
//TODO

static void				fill_cur_tk_with_new_token(t_token_inf *cur_tk
		, t_str_cmd_inf *scmd, t_alloc *alloc)
{
	char			*new_str;
	t_str_cmd_inf	new_scmd;
	t_list			*tk_list;
	t_list			*last_tk;

	cur_tk->type = TK_NOTHING;
	cur_tk->token = NULL;
	new_str = ft_strjoin(scmd->str, " H");
	if (!scmd_init(&new_scmd, new_str) || new_str == NULL)
	{
		free(new_str);
		scmd_clean(&new_scmd);
		return ;
	}
	free(new_str);
	tk_list = split_cmd_token_without_last_alias(&new_scmd, alloc->aliastable);
	last_tk = tk_list;
	while (last_tk != NULL && last_tk->next != NULL)
		last_tk = last_tk->next;
	if (last_tk != NULL)
		cur_tk->type = get_tk(last_tk)->type;
	ft_lstdel(&tk_list, del_token);
	scmd_clean(&new_scmd);
}

static size_t			get_pos_last_sub_cmd_start(t_str_cmd_inf *scmd)
{
	int		last_start_found;
	size_t	last_sub_cmd_start_pos;

	last_start_found = 0;
	last_sub_cmd_start_pos = 0;
	while (1)
	{
		if (!last_start_found && scmd->sub_str_cmd != NULL
				&& scmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_SUBCMD)
		{
			last_sub_cmd_start_pos = scmd->pos;
			last_start_found = 1;
		}
		else if (last_start_found && (scmd->sub_str_cmd == NULL
					|| scmd->sub_str_cmd->cur_str_cmd_type != SCMD_TYPE_SUBCMD))
		{
			last_sub_cmd_start_pos = 0;
			last_start_found = 0;
		}
		if (!scmd_move_to_next_char(scmd))
			break ;
	}
	return (last_sub_cmd_start_pos);
}

static void				fill_cur_tk_with_last_token(t_token_inf *cur_tk
		, t_token_inf *last_tk, t_str_cmd_inf *scmd, t_alloc *alloc)
{
	if (scmd->sub_str_cmd != NULL
			&& scmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_SUBCMD)
	{
		if (scmd_reset(scmd, last_tk->token))
		{
			scmd->pos = get_pos_last_sub_cmd_start(scmd);
			if (scmd_reset(scmd, scmd->str + scmd->pos))
			{
				set_cur_token_cmd(cur_tk, scmd, alloc);
				return ;
			}
		}
	}
	cur_tk->type = last_tk->type;
	cur_tk->token = ft_strdup(last_tk->token);
}

void					set_cur_token_cmd(t_token_inf *cur_tk_cmd
		, t_str_cmd_inf *scmd, t_alloc *alloc)
{
	t_list			*tk_list;
	t_list			*last_tk;

	tk_list = split_cmd_token_without_last_alias(scmd, alloc->aliastable);
	last_tk = tk_list;
	while (last_tk != NULL && last_tk->next != NULL)
		last_tk = last_tk->next;
	if (scmd_cur_char_is_in_nothing(scmd) && (scmd->pos == 0 || last_tk == NULL
				|| scmd_char_at_is_of(scmd, scmd->pos - 1, WORD_SEP_CHARS)
				|| get_tk(last_tk)->type == TK_RED_OPE
				|| get_tk(last_tk)->type == TK_CMD_SEP))
		fill_cur_tk_with_new_token(cur_tk_cmd, scmd, alloc);
	else
		fill_cur_tk_with_last_token(cur_tk_cmd, get_tk(last_tk), scmd, alloc);
	ft_lstdel(&tk_list, del_token);
}

static int				is_valid_var_char(char var_char, size_t idx)
{
	if (idx == 0)
	{
		return (ft_isalpha(var_char) || var_char == '_' || var_char == '\0');
	}
	else
	{
		return (ft_isalnum(var_char) || var_char == '_' || var_char == '\0');
	}
}

static void				update_last_var_inf(t_str_cmd_inf *scmd
		, const char **last_var_start, int *is_in_bracket)
{
	if (*last_var_start != NULL && !*is_in_bracket)
	{
		if (!is_valid_var_char(scmd_cur_char(scmd)
					, scmd_cur_str(scmd) - *last_var_start))
			*last_var_start = NULL;
	}
	else if (*last_var_start != NULL && *is_in_bracket)
	{
		if (scmd->sub_str_cmd == NULL
				|| scmd->sub_str_cmd->cur_str_cmd_type != SCMD_TYPE_VAR)
			*last_var_start = NULL;
	}
	if (*last_var_start == NULL && !scmd->is_in_quote
			&& scmd->sub_str_cmd == NULL && scmd_cur_char(scmd) == '$'
			&& !scmd_cur_char_is_escaped(scmd))
	{
		*last_var_start = scmd_cur_str(scmd) + 1;
		*is_in_bracket = 0;
	}
	else if (*last_var_start == NULL && scmd->sub_str_cmd != NULL
			&& scmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_VAR)
	{
		*last_var_start = scmd_cur_str(scmd);
		*is_in_bracket = 1;
	}
}

static const char		*find_last_var_start(const char *str, int *is_in_bracket)
{
	t_str_cmd_inf	scmd;
	const char		*last_var_start;

	*is_in_bracket = 0;
	if (str == NULL || !scmd_init(&scmd, str))
		return (NULL);
	last_var_start = NULL;
	while (1)
	{
		update_last_var_inf(&scmd, &last_var_start, is_in_bracket);
		if (!scmd_move_to_next_char(&scmd))
			break ;
	}
	if (last_var_start != NULL)
		last_var_start = str + (last_var_start - scmd.str);
	scmd_clean(&scmd);
	return (last_var_start);
}

//NE PAS GERER LES INIHIENVBETIRS ICI
//MAIS IHIBER CE QUI A APRES L'ASSIGN QUAND MEME APRES, EN PRENANT EN COMPOTE
//LES IBHINNFHETEIX PRECEDENTS
static const char		*find_last_assign_start(const char *str)
{
	size_t		idx;
	const char	*last_assign;

	if (str == NULL)
		return (NULL);
	idx = 0;
	while (str[idx] != '\0')
	{
		if (!is_valid_var_char(str[idx], idx))
			break ;
		++idx;
	}
	if (str[idx] != '=')
		return (NULL);
	last_assign = str + idx + 1;
	while (str[idx] != '\0')
	{
		if (str[idx] == ':')
			last_assign = str + idx + 1;
		++idx;
	}
	return (last_assign);
}

static const char		*find_last_home_user(const char *str)
{
	const char	*original_str;

	if (str == NULL)
		return (NULL);
	original_str = str;
	if (*str == '~')
	{
		++str;
		while (*str != '\0')
		{
			if (*str == '/')
				return (NULL);
			++str;
		}
		return (original_str + 1);
	}
	else
		return (NULL);
}

/*
** Alloue et retourne une nouvelle string representant la version inhibe de
** str. Retourne NULL en cas d'erreur.
*/

static char				*inhibe_this_str_for_autocomplete(const char *str
		, t_alloc *alloc)
{
	char	**str_tab;
	char	**tmp_tab;
	char	*new_str;

	new_str = NULL;
	str_tab = inhib_expand_str(str, alloc);
	tmp_tab = str_tab;
	while (tmp_tab != NULL && *tmp_tab != NULL)
	{
		new_str = *tmp_tab;
		++tmp_tab;
	}
	if (new_str == NULL)
		new_str = ft_strdup(str);
	else
		new_str = ft_strdup(new_str);
	ft_strtab_free(str_tab);
	return (new_str);
}

static t_ac_suff_inf	*autocomplete_cmdline_not_var(t_token_inf *cur_tk
		, t_alloc *alloc)
{
	const char		*real_start;
	const char		*tmp_start;
	char			*inhibed_str;
	t_ac_suff_inf	*acs_inf;

	real_start = (cur_tk->token == NULL ? "" : cur_tk->token);
	tmp_start = NULL;
	if (cur_tk->type != TK_RED_ROPT_FILE)
	{
		//TODO MIEUX GERER L'INHIB A CAUSE DE CA PSK IL PEUT Y AVOIR DES TOKENS D'IHNIB PLUS TOT
		if((tmp_start = find_last_assign_start(real_start)) != NULL)
			real_start = tmp_start;
	}
	if ((tmp_start = find_last_home_user(real_start)) != NULL)
	{
		inhibed_str = inhibe_this_str_for_autocomplete(tmp_start, alloc);
		acs_inf = autocomplete_user(inhibed_str == NULL ? "" : inhibed_str);
		if (acs_inf != NULL && acs_inf->suff_type == ACS_TYPE_FILE)
			acs_inf->suff_type = ACS_TYPE_DIR;
	}
	else
	{
		inhibed_str = inhibe_this_str_for_autocomplete(real_start, alloc);
		acs_inf = autocomplete_word(alloc->vars, (inhibed_str == NULL
					? "" : inhibed_str), cur_tk->type == TK_CMD, alloc);
	}
	free(inhibed_str);
	return (acs_inf);
}

t_ac_suff_inf			*autocomplete_cmdline(t_str_cmd_inf *scmd
		, t_alloc *alloc)
{
	t_token_inf		cur_tk;
	t_ac_suff_inf	*acs_inf;
	const char		*real_ac_start;
	int				is_in_bracket;

	set_cur_token_cmd(&cur_tk, scmd, alloc);
	real_ac_start = find_last_var_start(cur_tk.token, &is_in_bracket);
	if (real_ac_start == NULL)
	{
		acs_inf = autocomplete_cmdline_not_var(&cur_tk, alloc);
	}
	else
	{
		acs_inf = autocomplete_var(alloc->vars, real_ac_start);
		if (acs_inf != NULL && acs_inf->suff_type == ACS_TYPE_FILE
				&& is_in_bracket)
			acs_inf->suff_type = ACS_TYPE_VAR_IN_BRACKETS;
	}
	free(cur_tk.token);
	return (acs_inf);
}
