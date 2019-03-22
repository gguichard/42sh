/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_cmdline.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:27:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/22 17:46:12 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_CMDLINE_H
# define STRUCT_CMDLINE_H

# include <termios.h>
# include <sys/ioctl.h>
# include "history.h"

typedef enum		e_prompt
{
	PROMPT_DEFAULT = 0,
	PROMPT_BACKSLASH,
	PROMPT_BRACKET,
	PROMPT_SUBCMD,
	PROMPT_QUOTE,
	PROMPT_DQUOTE,
	PROMPT_OPERATOR,
	PROMPT_HEREDOC
}					t_prompt;

typedef struct		s_cursor
{
	int				x;
	int				y;
}					t_cursor;

struct				s_input
{
	char			stdin_buffer[4097];
	char			*buffer;
	int				capacity;
	int				size;
	int				offset;
	int				reading;
};

typedef enum		e_seq_mode
{
	MODE_INSERT,
	MODE_VISUAL,
	MODE_COMMON
}					t_seq_mode;

typedef struct		s_seq_keys
{
	char			buffer[128];
	int				size;
	int				offset;
}					t_seq_keys;

struct				s_prompt
{
	const char		*str;
	int				offset;
	int				big_offset;
};

struct				s_visual
{
	int				toggle;
	int				start_offset;
	char			*clipboard;
};

typedef struct		s_cmdline
{
	struct termios	default_term;
	struct s_prompt	prompt;
	struct s_input	input;
	struct winsize	winsize;
	struct s_visual	visual;
	t_cursor		cursor;
	t_seq_keys		seq_keys;
	int				row;
	int				saved_col;
	int				konami_code;
	t_history		history;
}					t_cmdline;

typedef struct		s_seq
{
	const char		*str;
	t_seq_mode		mode;
	int				(*fn)(t_cmdline *cmdline);
}					t_seq;

#endif
