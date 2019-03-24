/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 10:08:33 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/24 12:40:34 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

typedef struct s_history_entry	t_history_entry;

typedef struct		s_history
{
	t_history_entry	*front;
	t_history_entry	*back;
	t_history_entry	*offset;
}					t_history;

struct				s_history_entry
{
	char			*content;
	t_history_entry	*prev;
	t_history_entry	*next;
};

void				add_history_entry(t_history *history, const char *content);
void				push_history_entry(t_history *history, const char *content);
const char			*peek_history_prev(t_history *history);
const char			*peek_history_next(t_history *history);

#endif
