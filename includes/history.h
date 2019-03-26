/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 10:08:33 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/26 15:00:22 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# define MAX_HISTORY_LINES 500

typedef struct s_history_entry	t_history_entry;

typedef struct		s_history
{
	t_history_entry	*front;
	t_history_entry	*back;
	t_history_entry	*offset;
	char			*buffer;
}					t_history;

struct				s_history_entry
{
	char			*content;
	t_history_entry	*prev;
	t_history_entry	*next;
};

t_history_entry		*create_history_entry(const char *content);
void				add_history_entry(t_history *history, const char *content);
void				push_history_entry(t_history *history, const char *content);
const char			*peek_history_prev(t_history *history);
const char			*peek_history_next(t_history *history);

int					replace_event(t_history *history, char **input
		, size_t *offset, char *event);
int					expand_history_events(t_history *history, char **input);

#endif
