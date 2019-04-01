/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enter_exit_visual.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 10:57:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 20:16:22 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

int	handle_toggle_visual(t_cmdline *cmdline)
{
	static char	*vi_tcap = NULL;
	static char	*ve_tcap = NULL;
	static char	*cd_tcap = NULL;

	if (vi_tcap == NULL)
		vi_tcap = tgetstr("vi", NULL);
	if (ve_tcap == NULL)
		ve_tcap = tgetstr("ve", NULL);
	if (cd_tcap == NULL)
		cd_tcap = tgetstr("cd", NULL);
	cmdline->visual.toggle = !cmdline->visual.toggle;
	tputs(cmdline->visual.toggle ? vi_tcap : ve_tcap, 1, t_putchar);
	if (cmdline->visual.toggle)
		cmdline->visual.start_offset = cmdline->input.offset;
	else
		update_visual_select(cmdline);
	go_to_cursor_pos(cmdline, (t_cursor){0, 0});
	tputs(cd_tcap, 1, t_putchar);
	print_prompt_and_cmdline(cmdline);
	return (1);
}
