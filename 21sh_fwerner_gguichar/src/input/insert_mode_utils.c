/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_mode_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 13:41:48 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/29 18:00:53 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str_cmd_inf.h"
#include "shell.h"
#include "input.h"
#include "apply_escape.h"

void	insert_spestr_cmdline(t_shell *shell, t_term *term, const char *spe_str
		, char replace)
{
	while (*spe_str != '\0')
	{
		if (*spe_str == '@')
			insert_cmdline(shell, term, replace);
		else
			insert_cmdline(shell, term, *spe_str);
		++spe_str;
	}
}

void	add_char_and_escape_if_needed(char char_to_add, int already_escaped
		, t_shell *shell, t_str_cmd_inf *scmd)
{
	if (char_to_add == '\0')
		return ;
	else if (!already_escaped)
	{
		if ((scmd_cur_char_is_in_nothing(scmd)
					&& ft_strchr(" \t|<>;$\'\"\\", char_to_add) != NULL)
				|| (scmd->is_in_doublequote
					&& ft_strchr("$\"\\", char_to_add) != NULL))
			insert_spestr_cmdline(shell, &(shell->term), "\\@", char_to_add);
		else if (scmd_cur_char_is_in_nothing(scmd) && char_to_add == '\n')
			insert_spestr_cmdline(shell, &(shell->term), "\'@\'", char_to_add);
		else if (scmd->is_in_quote && char_to_add == '\'')
			insert_spestr_cmdline(shell, &(shell->term)
					, "\'\\@\'", char_to_add);
		else
			insert_cmdline(shell, &(shell->term), char_to_add);
	}
	else if (char_to_add == '\n')
	{
		handle_bs_key(shell, &(shell->term));
		add_char_and_escape_if_needed(char_to_add, 0, shell, scmd);
	}
	else
		insert_cmdline(shell, &(shell->term), char_to_add);
}

char	*init_scmd_with_realline(t_str_cmd_inf *scmd, t_term *term
		, int *at_end_of_line)
{
	char	*line;
	char	*real_line;

	if ((line = get_command_line(term)) == NULL)
		return (NULL);
	*at_end_of_line = (line[ft_strlen(line)
			- (term->size - term->cursor)] == '\0');
	line[ft_strlen(line) - (term->size - term->cursor)] = '\0';
	if ((real_line = apply_only_newline_escape(line)) == NULL)
	{
		free(line);
		return (NULL);
	}
	free(line);
	scmd_init(scmd, real_line);
	return (real_line);
}
