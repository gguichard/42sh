#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

static int	is_special_char(const char *s, int i)
{
	if (s[i] == '_' || s[i] == '?' || s[i] == '!' || s[i] == '$')
		return (1);
	if ((s[i] == '\'' && s[i + 1] == '\'') || (s[i] == '"' && s[i + 1] == '"'))
		return (1);
	return (0);
}

int			expand_var(char **str, t_alloc *alloc, const char *exp,
		size_t *len)
{
	if (ft_strncmp(exp, "${", 2) == 0)
	{
		if (!check_expand_syntax(exp + 2))
			return (error_expand(exp));
		*str = get_expand_value(exp + 2, 1, alloc, len);
	}
	else
		*str = get_expand_value(exp + 1, 0, alloc, len);
	return (1);
}


static int	sub_cmd_expand(t_alloc *alloc, char **input
					, size_t *pos, t_str_cmd_inf *str_cmd)
{
	char	*value;
	char	*cmd;
	int		was_in_subcmd;
	size_t	x;

	x = 0;
	while (scmd_cur_char(str_cmd))
	{
		was_in_subcmd = (str_cmd->sub_str_cmd != NULL && str_cmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_SUBCMD);
		x += scmd_move_to_next_char(str_cmd);
		if (was_in_subcmd && !((str_cmd->sub_str_cmd != NULL && str_cmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_SUBCMD)))
			break ;
	}
	cmd = ft_strsub(*input, *pos + 2, x - 3);
	value = sub_cmd_exec(alloc, cmd);
	ft_strdel(&cmd);
	ft_strreplace_inside(input, *pos, x, (value == NULL ? "" : value));
	if (value != NULL)
		*pos += ft_strlen(value);
	ft_strdel(&value);
	str_cmd->pos -= 1;
	return (1);
}

int			expand(char **input, t_alloc *alloc, size_t *pos, t_str_cmd_inf *str_cmd)
{
	char		*str;
	const char	*exp;
	size_t		len;

	str = NULL;
	exp = ft_strchr(*input + *pos, '$');
	if (exp[1] == '(')
		return (sub_cmd_expand(alloc, input, pos, str_cmd));
	else if (!expand_var(&str, alloc, *input + *pos, &len))
		return (0);
	if (str && (*input)[*pos + 1] == '{')
		ft_strreplace_inside(input, *pos, len + 3, str);
	else if (str && !ft_isalnum(exp[1]) && is_special_char(exp, 1) == 0)
	{
		free(str);
		*pos += 1;
		return (1);
	}
	else
		ft_strreplace_inside(input, *pos, len + 1, str);
	if (str)
	{
		*pos += ft_strlen(str);
		free(str);
	}
	return (1);
}
