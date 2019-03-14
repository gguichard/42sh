/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_cmd_inf.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:27:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 14:19:35 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_CMD_INF_H
# define STRUCT_CMD_INF_H

enum			e_prompt
{
	PROMPT_DEFAULT = 0,
	PROMPT_BACKSLASH,
	PROMPT_QUOTE,
	PROMPT_DQUOTE,
	PROMPT_BRACKET,
	PROMPT_HEREDOC
};

typedef struct	s_cursor
{
	int	x;
	int	y;
}				t_cursor;

struct			s_input
{
	char	*buffer;
	int		capacity;
	int		size;
	int		offset;
};

typedef enum	e_seq_mode
{
	MODE_INSERT,
	MODE_VISUAL,
	MODE_COMMON
}				t_seq_mode;

typedef struct	s_seq_keys
{
	char	buffer[8];
	int		size;
	int		offset;
}				t_seq_keys;

struct			s_prompt
{
	enum e_prompt	type;
	int				offset;
};

struct			s_visual
{
	int		toggle;
	int		start_offset;
	char	*clipboard;
};

typedef struct	s_cmdline
{
	struct s_prompt	prompt;
	struct s_input	input;
	struct winsize	winsize;
	t_cursor		cursor;
	t_seq_keys		seq_keys;
	int				row;
	int				saved_col;
	struct s_visual	visual;
}				t_cmdline;

typedef struct	s_seq
{
	char		*str;
	t_seq_mode	mode;
	int			(*fn)(t_cmdline *cmdline);
}				t_seq;

#endif
