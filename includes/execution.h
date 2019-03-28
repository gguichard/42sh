/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 15:29:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/28 15:30:26 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "shell.h"

int		exec_input(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);

#endif
