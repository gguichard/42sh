/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 21:06:40 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/05 21:06:41 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include <unistd.h>
#include "shell.h"
#include "vars.h"
#include "builtins.h"

int	builtin_set(t_ast *elem, t_alloc *alloc)
{
	t_list	*cur;
	t_var	*var;

	if (elem->input[1] != NULL)
	{
		ft_dprintf(STDERR_FILENO, "42sh: set: %s: invalid usage\n"
				"set: usage: set\n", elem->input[1]);
		return (1);
	}
	cur = alloc->vars;
	while (cur != NULL)
	{
		var = (t_var *)cur->content;
		if (var->value != NULL)
			ft_printf("%s=%s\n", var->key, var->value);
		cur = cur->next;
	}
	return (0);
}
