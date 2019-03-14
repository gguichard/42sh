/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enter_exit_visual.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 10:57:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 11:21:15 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdline.h"

int	handle_toggle_visual(t_cmdline *cmdline)
{
	cmdline->visual_mode = !cmdline->visual_mode;
	return (1);
}
