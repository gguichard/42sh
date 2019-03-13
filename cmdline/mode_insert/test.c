/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 22:02:47 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/13 10:04:12 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdline.h"

int	handle_test_newline(t_cmdline *cmdline)
{
	add_char_to_input(&cmdline->input, '\n');
	write_char_in_cmdline(cmdline, '\n');
	return (1);
}
