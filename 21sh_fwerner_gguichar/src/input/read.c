/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/25 18:28:21 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 09:37:40 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"
#include "shell.h"
#include "input.h"

static int	handle_key_mode(t_shell *shell, t_term *term, char key)
{
	if (key == FORM_FEED_KEY)
		return (handle_screen_clear(shell, term));
	if (key == BELL_KEY)
		return (handle_bell(shell, term));
	if (term->visual_mode)
		return (handle_vm_key(shell, term, key));
	else
		return (handle_key(shell, term, key));
}

static int	read_advanced(t_shell *shell)
{
	int		ret;
	char	buf;
	int		handle;

	while ((ret = read(STDIN_FILENO, &buf, 1)) > 0)
	{
		if (handle_esc_key(shell, &(shell->term), buf))
		{
			if (!(shell->term.visual_mode))
				shell->term.ac_flag = 0;
			continue ;
		}
		handle = handle_key_mode(shell, &(shell->term), buf);
		if (handle <= 0)
			return (handle < 0 ? 0 : ret);
	}
	return (ret);
}

int			read_input(t_shell *shell)
{
	int	ret;

	if (!(shell->term.legacy_mode))
		ret = read_advanced(shell);
	else
	{
		ft_strdel(&(shell->term.line));
		ret = get_next_line(STDIN_FILENO, &(shell->term.line));
		shell->term.size = !shell->term.line ? 0 : ft_strlen(shell->term.line);
	}
	return (ret);
}
