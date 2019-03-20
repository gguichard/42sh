/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 16:08:29 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 16:16:26 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "printf.h"
#include "error.h"

int	error_test(char **input, int count)
{
	if (count == 2)
		ft_dprintf(STDERR_FILENO, "42sh: test: %s: "
				"unary operator expected\n", input[0]);
	else
		ft_dprintf(STDERR_FILENO, "42sh: test: %s: "
				"binary operator expected\n", input[1]);
	return (2);
}
