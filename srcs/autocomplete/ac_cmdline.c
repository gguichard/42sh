#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "split_cmd_token.h"
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
//TODO CHERCHER LES DOSSIERS DANS . SI EXECUTABLE NON TROUVE DANS LE PATH
//TODO
//TODO AUTOCOMPLETER LES ALIAS AUSSI (PSK C COOL)
//TODO
//TODO SUPPORT AUTOCOMPLETION APRES = OU : (PSK C MIEUX)
//TODO
//TODO SUPPORTER L'AUTOCOMPLETION DANS SOUS COMMANDE (FONCTION RECURSIVE AVEC BASE FUNC LOL)
//TODO

static void			fill_cur_tk_with_new_token(t_token_inf *cur_tk
		, t_str_cmd_inf *scmd, t_alloc *alloc)
{
	char			*new_str;
	t_str_cmd_inf	new_scmd;
	t_list			*tk_list;
	t_list			*last_tk;;

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

static void			fill_cur_tk_with_last_token(t_token_inf *cur_tk
		, t_token_inf *last_tk, t_str_cmd_inf *scmd, t_alloc *alloc)
{
	(void)scmd;
	(void)alloc;
	cur_tk->type = last_tk->type;
	cur_tk->token = ft_strdup(last_tk->token);
	//analyser le dernier token pour completer en consequence
	//aller jusqu'au bout des sub_str_cmd et retokeniser si besoin (SUB_CMD)
	//gerer correctement le cas du LRED_OPT (si contient "& text" ou "&-" etc
	return ;
}

/*
** Alloue et retourne le dernier token de la commande. Renvoie NULL si erreur.
*/

static t_token_inf	*get_cur_token_cmd(const char *str, t_alloc *alloc)
{
	t_token_inf		*cur_tk_cmd;
	t_list			*tk_list;
	t_list			*last_tk;
	t_str_cmd_inf	scmd;

	if (!scmd_init(&scmd, str)
			|| (cur_tk_cmd = (t_token_inf*)malloc(sizeof(t_token_inf))) == NULL)
	{
		scmd_clean(&scmd);
		return (NULL);
	}
	tk_list = split_cmd_token_without_last_alias(&scmd, alloc->aliastable);
	last_tk = tk_list;
	while (last_tk != NULL && last_tk->next != NULL)
		last_tk = last_tk->next;
	if (scmd_cur_char_is_in_nothing(&scmd) && (scmd.pos == 0 || last_tk == NULL
				|| scmd_char_at_is_of(&scmd, scmd.pos - 1, WORD_SEP_CHARS)
				|| get_tk(last_tk)->type == TK_RED_OPE
				|| get_tk(last_tk)->type == TK_CMD_SEP))
		fill_cur_tk_with_new_token(cur_tk_cmd, &scmd, alloc);
	else
		fill_cur_tk_with_last_token(cur_tk_cmd, get_tk(last_tk), &scmd, alloc);
	ft_lstdel(&tk_list, del_token);
	scmd_clean(&scmd);
	return (cur_tk_cmd);
}

static int			is_valid_var_char(char var_char, size_t idx)
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

static void			update_last_var_inf(t_str_cmd_inf *scmd
		, const char **last_var_start, int *is_inline_var)
{
	if (*last_var_start != NULL && *is_inline_var)
	{
		if (!is_valid_var_char(scmd_cur_char(scmd)
					, scmd_cur_str(scmd) - *last_var_start))
			*last_var_start = NULL;
	}
	else if (*last_var_start != NULL && !*is_inline_var)
	{
		if (scmd->sub_str_cmd == NULL
				|| scmd->sub_str_cmd->cur_str_cmd_type != SCMD_TYPE_VAR)
			*last_var_start = NULL;
	}
	else if (!scmd->is_in_quote && scmd->sub_str_cmd == NULL
			&& scmd_cur_char(scmd) == '$' && !scmd_cur_char_is_escaped(scmd)
			&& is_valid_var_char(scmd->str[scmd->pos + 1], 0))
	{
		*last_var_start = scmd_cur_str(scmd) + 1;
		*is_inline_var = 1;
	}
	else if (scmd->sub_str_cmd != NULL
			&& scmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_VAR)
	{
		*last_var_start = scmd_cur_str(scmd);
		*is_inline_var = 0;
	}
}

static const char	*find_last_var_start(const char *str)
{
	t_str_cmd_inf	scmd;
	const char		*last_var_start;
	int				is_inline_var;

	if (str == NULL || !scmd_init(&scmd, str))
		return (NULL);
	last_var_start = NULL;
	is_inline_var = 0;
	while (1)
	{
		update_last_var_inf(&scmd, &last_var_start, &is_inline_var);
		if (!scmd_move_to_next_char(&scmd))
			break ;
	}
	scmd_clean(&scmd);
	return (last_var_start);
}

t_ac_suff_inf		*autocomplete_cmdline(const char *str, t_alloc *alloc)
{
	t_token_inf		*cur_tk;
	t_ac_suff_inf	*acs_inf;
	const char		*var_start;

	cur_tk = get_cur_token_cmd(str, alloc);
	var_start = find_last_var_start(cur_tk->token);
	if (var_start == NULL)
	{
		acs_inf = autocomplete_word(*alloc->var
				, (cur_tk->token == NULL ? "" : cur_tk->token)
				, cur_tk->type == TK_CMD, alloc->builtins);
	}
	else
	{
		acs_inf = autocomplete_var(*alloc->var, var_start);
	}
	del_token(cur_tk, 0);
	return (acs_inf);
}
