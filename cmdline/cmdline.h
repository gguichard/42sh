/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:02:53 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/11 17:16:08 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDLINE_H
# define CMDLINE_H

# include <sys/ioctl.h>

# define INPUT_SIZE_INCR 1024

typedef struct s_cmdline	t_cmdline;
typedef struct s_cursor	t_cursor;

enum	e_prompt
{
	PROMPT_DEFAULT = 0,
	PROMPT_BACKSLASH,
	PROMPT_QUOTE,
	PROMPT_DQUOTE,
	PROMPT_BRACKET,
	PROMPT_HEREDOC
};

struct	s_input
{
	char	*buffer;
	int		capacity;
	int		size;
	int		offset;
};

struct	s_cursor
{
	int	x;
	int	y;
};

struct	s_cmdline
{
	enum e_prompt	prompt_type;
	struct s_input	input;
	struct winsize	winsize;
	t_cursor		cursor;
};

int			setup_term(void);
int			reset_term(void);

void		print_prompt(t_cmdline *cmdline);
void		read_input(t_cmdline *cmdline);

int			set_cursor_pos(t_cursor *cursor);

#endif
