/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_paste_clipboard.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:04:49 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 15:32:49 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "cmdline.h"

int	handle_cut_key(t_cmdline *cmdline)
{
	return (vm_copy(cmdline, 1));
}

int	handle_copy_key(t_cmdline *cmdline)
{
	return (vm_copy(cmdline, 0));
}

int	handle_paste_key(t_cmdline *cmdline)
{
	return (vm_paste(cmdline, 1));
}

int	handle_paste_before_key(t_cmdline *cmdline)
{
	return (vm_paste(cmdline, 0));
}
