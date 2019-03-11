/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:28:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/11 17:18:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "cmdline.h"

static void	add_char_to_input(struct s_input *input, char c)
{
	int		new_cap;
	char	*new_buffer;

	if (input->size == input->capacity)
	{
		new_cap = input->capacity + INPUT_SIZE_INCR;
		new_buffer = (char *)malloc((new_cap + 1) * sizeof(char));
		if (new_buffer == NULL)
			return ;
		if (input->buffer != NULL)
			ft_memcpy(new_buffer, input->buffer, input->size);
		new_buffer[input->size] = '\0';
		input->capacity = new_cap;
		input->buffer = new_buffer;
	}
	if (input->offset != input->size)
		ft_memmove(input->buffer + input->offset + 1
				, input->buffer + input->offset
				, input->size - input->offset + 1);
	input->buffer[input->offset] = c;
	input->offset += 1;
	input->size += 1;
}

void		read_input(t_cmdline *cmdline)
{
	char	c;

	while (read(STDIN_FILENO, &c, 1))
	{
		if (c == 4)
			break ;
		if (c == 10)
			ft_putchar('\n');
		if (c >= 32 && c != 127)
		{
			add_char_to_input(&cmdline->input, c);
			write(STDOUT_FILENO, &c, 1);
		}
	}
}
