/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_inf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 16:22:42 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/18 16:40:38 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "redirect_inf.h"
#include "cmd_inf.h"

void	del_cmd(void *content, size_t content_size)
{
	(void)content_size;
	ft_lstfree(&(((t_cmd_inf*)content)->arg_lst));
	if (((t_cmd_inf*)content)->pipe_cmd != NULL)
		del_cmd(((t_cmd_inf*)content)->pipe_cmd, 0);
	ft_lstdel(&(((t_cmd_inf*)content)->redirect_lst), del_redirect);
	free(content);
}
