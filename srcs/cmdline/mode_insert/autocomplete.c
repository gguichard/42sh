/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:42:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/08 15:56:33 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include "str_cmd_inf.h"
#include "autocomplete.h"
#include "cmdline.h"

static char	*get_full_input(char *full_input, char *buffer)
{
	char	*tmp[3];

	if (full_input == NULL)
		return (ft_strdup(buffer));
	else
	{
		tmp[0] = full_input;
		tmp[1] = buffer;
		tmp[2] = NULL;
		return (ft_join(tmp, "\n"));
	}
}

static int	init_autocomplete_stuff(t_cmdline *cmdline, t_ac_suff_inf **acs_inf
		, t_str_cmd_inf *scmd, int *ret)
{
	char	*buffer;

	buffer = get_full_input(cmdline->alloc->full_input, cmdline->input.buffer);
	if (buffer == NULL)
		return (0);
	buffer[ft_strlen(buffer)
		- (cmdline->input.size - cmdline->input.offset)] = '\0';
	*ret = scmd_init(scmd, buffer);
	free(buffer);
	if (!*ret)
		return (0);
	else if ((*acs_inf = autocomplete_cmdline(scmd, cmdline->alloc)) == NULL)
	{
		scmd_clean(scmd);
		return (0);
	}
	*ret = (*acs_inf)->choices != NULL && (cmdline->ac_flag
			|| ((*acs_inf)->suff != NULL && (*acs_inf)->suff[0] != '\0'));
	return (1);
}

int			handle_autocomplete(t_cmdline *cmdline)
{
	int				ret;
	t_ac_suff_inf	*acs_inf;
	t_str_cmd_inf	scmd;

	if (!init_autocomplete_stuff(cmdline, &acs_inf, &scmd, &ret))
		return (0);
	if (!cmdline->ac_flag)
		cmdline->ac_flag += (acs_inf->choices != NULL);
	else
	{
		write(STDOUT_FILENO, "\n", 1);
		ac_print_list(acs_inf->choices, cmdline);
		print_prompt_and_cmdline(cmdline);
	}
	if (acs_inf->suff != NULL)
	{
		ac_append_to_cmdline(cmdline, acs_inf, &scmd
				, cmdline->input.offset == cmdline->input.size);
		if (acs_inf->suff[0] != '\0')
			cmdline->ac_flag = 0;
	}
	delete_ac_suff_inf(acs_inf);
	scmd_clean(&scmd);
	return (ret);
}
