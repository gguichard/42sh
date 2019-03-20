/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   konami_code.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 11:05:29 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 11:17:58 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include "cmdline.h"

int	handle_konami_code(t_cmdline *cmdline)
{
	cmdline->konami_code = !cmdline->konami_code;
	handle_clear(cmdline);
	return (1);
}
