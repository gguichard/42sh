/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_inf.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 10:20:19 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/18 16:16:25 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_INF_H
# define CMD_INF_H

typedef struct s_cmd_inf	t_cmd_inf;

struct						s_cmd_inf
{
	t_list					*arg_lst;
	t_cmd_inf				*pipe_cmd;
	t_list					*redirect_lst;
};

void						del_cmd(void *content, size_t content_size);

#endif
