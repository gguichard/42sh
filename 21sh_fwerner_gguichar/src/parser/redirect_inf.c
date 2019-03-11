/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_inf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 16:22:42 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/29 11:08:33 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "redirect_inf.h"

void				del_redirect(void *content, size_t content_size)
{
	(void)content_size;
	free(((t_redirect_inf*)content)->to_word);
	free(((t_redirect_inf*)content)->heredoc);
	ft_lstfree(&(((t_redirect_inf*)content)->red_save));
	free(content);
}

t_redirect_type		redirection_str_to_type(const char *str)
{
	if (ft_strequ(str, "<"))
		return (RD_L);
	else if (ft_strequ(str, ">"))
		return (RD_R);
	else if (ft_strequ(str, "<<"))
		return (RD_LL);
	else if (ft_strequ(str, ">>"))
		return (RD_RR);
	else
		return (RD_ERROR);
}
