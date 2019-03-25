/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 16:08:29 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 20:29:45 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "printf.h"
#include "error.h"

void	error_test(int argc, char **argv)
{
	if (argc == 2)
		ft_dprintf(STDERR_FILENO, "42sh: test: %s: "
				"unary operator expected\n", argv[0]);
	else
		ft_dprintf(STDERR_FILENO, "42sh: test: %s: "
				"binary operator expected\n", argv[1]);
}
