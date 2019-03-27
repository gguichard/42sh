/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:42:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/27 13:56:51 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "autocomplete.h"
#include "cmdline.h"

static void	add_str_to_cmdline(t_cmdline *cmdline, const char *str)
{
	while (*str != '\0')
	{
		add_char_to_input(cmdline, *str);
		str++;
	}
}

int			handle_autocomplete(t_cmdline *cmdline)
{
	char			*old_char_pos;
	char			old_char;
	t_ac_suff_inf	*acs_inf;
	int				ret;

	old_char_pos = cmdline->input.buffer + cmdline->input.offset;
	old_char = *old_char_pos;
	*old_char_pos = '\0';
	acs_inf = autocomplete_cmdline(cmdline->input.buffer, cmdline->alloc);
	*old_char_pos = old_char;
	if (acs_inf == NULL)
		return (0);
	ret = 1;
	if (acs_inf->choices == NULL)
		ret = 0;
	if (acs_inf->suff != NULL)
		add_str_to_cmdline(cmdline, acs_inf->suff);
	delete_ac_suff_inf(acs_inf);
	return (ret);
}
