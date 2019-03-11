/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_spe_keys2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 12:25:49 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/21 17:17:07 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	selection_cmd(t_buf *input, char k, t_env **lst)
{
	if (k == 49 || k == 51)
	{
		ft_memdel((void **)&g_clip);
		if (!(g_clip = ft_strdup(selec_buffer(1, 0, 0)->s)))
			ft_exit_malloc();
		if (!g_clip[0])
			ft_memdel((void **)&g_clip);
	}
	else if (k == 50)
		paste_intra_clip(input, lst);
	if (k == 51)
		cut_selection(input, selec_buffer(1, 0, 0), lst);
}

void	selection_input(t_buf *input, char k, t_env **lst)
{
	if (k == 68)
		selection_init(input, -1, lst);
	else if (k == 67)
		selection_init(input, 1, lst);
}
