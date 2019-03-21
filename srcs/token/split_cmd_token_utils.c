#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "split_cmd_token.h"

int				is_a_spe_char(char c)
{
	if (c == '\0')
		return (0);
	else if (ft_strchr(WORD_SEP_CHARS, c) != NULL)
		return (1);
	else if (ft_strchr(CMD_SPE_CHARS, c) != NULL)
		return (1);
	return (0);
}

int				cur_token_is_number(t_split_cmd_inf *sp_cmd)
{
	const char	*token_start;

	token_start = sp_cmd->tk_start;
	while (token_start != scmd_cur_str(sp_cmd->scmd))
	{
		if (!ft_isdigit(*token_start))
			return (0);
		++token_start;
	}
	return (1);
}

int				get_cur_spe_char_token_len_and_set_type(
		t_split_cmd_inf *sp_cmd)
{
	size_t	len;

	len = 0;
	if (is_a_spe_char(*sp_cmd->tk_start))
	{
		++len;
		if (scmd_cur_is_of(sp_cmd->scmd, DOUBLED_SPE_CHARS)
				&& scmd_cur_char(sp_cmd->scmd)
				== sp_cmd->scmd->str[sp_cmd->scmd->pos + 1])
			++len;
		if (scmd_cur_is_of(sp_cmd->scmd, RED_TYPE_SPE_CHARS))
			sp_cmd->cur_tk_type = TK_RED_OPE;
		else
			sp_cmd->cur_tk_type = TK_CMD_SEP;
	}
	return (len);
}

/*
** Retourne le type du token avant le char actuel dans le cas ou le char actuel
** est un spe char.
*/

t_token_type	get_tk_type_before_cur_char(t_split_cmd_inf *sp_cmd)
{
	sp_cmd->last_tk_end_by_and = 0;
	if (scmd_cur_is_of(sp_cmd->scmd, RED_TYPE_SPE_CHARS))
	{
		if (cur_token_is_number(sp_cmd))
			return (TK_LRED_OPT);
		else if (sp_cmd->scmd->pos > 0
				&& sp_cmd->scmd->str[sp_cmd->scmd->pos] == '>'
				&& !scmd_char_at_is_escaped(sp_cmd->scmd, sp_cmd->scmd->pos - 1)
				&& sp_cmd->scmd->str[sp_cmd->scmd->pos - 1] == '&')
		{
			sp_cmd->last_tk_end_by_and = 1;
			return (TK_PARAM); //TK_WORD
		}
	}
	return (TK_PARAM); //TK_WORD
}
